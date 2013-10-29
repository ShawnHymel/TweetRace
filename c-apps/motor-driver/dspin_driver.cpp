#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include "dspin_driver.h"

// commands
static const uint8_t CMD_NOP = 0x00;
static const uint8_t CMD_SET_PARAM = 0x00;
static const uint8_t CMD_GET_PARAM = 0x20;
static const uint8_t CMD_RUN = 0x50;
static const uint8_t CMD_STEP_CLOCK = 0x58;
static const uint8_t CMD_MOVE = 0x40;
static const uint8_t CMD_GOTO = 0x60;
static const uint8_t CMD_GOTO_DIR = 0x68;
static const uint8_t CMD_GO_UNTIL = 0x82;
static const uint8_t CMD_RELEASE_SW = 0x92;
static const uint8_t CMD_GO_HOME = 0x70;
static const uint8_t CMD_GO_MARK = 0x78;
static const uint8_t CMD_RESET_POS = 0xd8;
static const uint8_t CMD_RESET_DEVICE = 0xc0;
static const uint8_t CMD_STOP = 0xb0;
static const uint8_t CMD_HIZ = 0xa0;
static const uint8_t CMD_GET_STATUS = 0xd0;

// status masks
static const uint16_t STATUS_HIZ_FLAG           = 0x0001;
static const uint16_t STATUS_BUSY_FLAG          = 0x0002;
static const uint16_t STATUS_SWITCH_CLOSED_FLAG = 0x0004;
static const uint16_t STATUS_SWITCH_EVENT_FLAG  = 0x0008;
static const uint16_t STATUS_DIR_FLAG           = 0x0010;
static const uint16_t STATUS_MOTOR_STAT_MASK    = 0x0060;
static const uint16_t STATUS_NOPERF_CMD_FLAG    = 0x0080;

static const uint16_t STATUS_WRONG_CMD_FLAG     = 0x0100;
static const uint16_t STATUS_UNDER_V_FLAG       = 0x0200;// active low
static const uint16_t STATUS_THERM_WARN_FLAG    = 0x0300;// active low
static const uint16_t STATUS_THERM_SDN_FLAG     = 0x0800;
static const uint16_t STATUS_OVR_I_FLAG         = 0x1000;
static const uint16_t STATUS_LOSS_A_FLAG        = 0x2000;
static const uint16_t STATUS_LOSS_B_FLAG        = 0x4000;
static const uint16_t STATUS_SCK_MODE_FLAG      = 0x8000;

// register addresses
static const uint8_t  REG_ABS_POS         = 0x01;
static const uint8_t  REG_EL_POS          = 0x02;
static const uint8_t  REG_MARK_POS        = 0x03;
static const uint8_t  REG_SPEED           = 0x04;
static const uint8_t  REG_ACC             = 0x05;
static const uint8_t  REG_DEC             = 0x06;
static const uint8_t  REG_MAX_SPD         = 0x07;
static const uint8_t  REG_MIN_SPD         = 0x08;
static const uint8_t  REG_FULL_STEP_SPEED = 0x15;
static const uint8_t  REG_KVAL_HOLD       = 0x09;
static const uint8_t  REG_KVAL_RUN        = 0x0a;
static const uint8_t  REG_KVAL_ACC        = 0x0b;
static const uint8_t  REG_KVAL_DEC        = 0x0c;
static const uint8_t  REG_INT_SPD         = 0x0d;
static const uint8_t  REG_START_SLOPE     = 0x0e;
static const uint8_t  REG_ACC_FINAL_SLOPE = 0x0f;
static const uint8_t  REG_DEC_FINAL_SLOPE = 0x10;
static const uint8_t  REG_K_THERM         = 0x11;
static const uint8_t  REG_ADC_OUT         = 0x12;
static const uint8_t  REG_OCD_THRESHOLD   = 0x13;
static const uint8_t  REG_STALL_THRESHOLD = 0x14;
static const uint8_t  REG_STEP_MODE       = 0x16;
static const uint8_t  REG_ALARM_EN        = 0x17;
static const uint8_t  REG_CONFIG          = 0x18;
static const uint8_t  REG_STATUS          = 0x19;
//static const uint8_t  REG_RESERVED1 = 0x1a;
//static const uint8_t  REG_RESERVED2 = 0x1b;


dspin_driver::dspin_driver()
{
	mover_p = new spi_mover("/dev/spidev0.1");
	
	mover_p->set_mode(true, true);
}

dspin_driver::~dspin_driver()
{
	reset();

	delete mover_p;
}

void dspin_driver::test()
{
	uint8_t out[4], in[4];

	// try to read status
	out[0] = CMD_GET_STATUS;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	printf("Status: 0x%x 0x%x, 0x%x\r\n", in[0], in[1], in[2]);
	
	// try to read ADC
	//out[0] = 0x32;
	out[0] = CMD_GET_PARAM | REG_ADC_OUT;
	out[1] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

	printf("ADC: 0x%x, 0x%x\r\n", in[0], in[1]);

	// try to read min speed
	out[0] = CMD_GET_PARAM | REG_MIN_SPD;
	out[1] = 0x00; 
	out[2] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

	printf("MIN: 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2]);

	// try to read max speed
	out[0] = CMD_GET_PARAM | REG_MAX_SPD;
	out[1] = 0x00; 
	out[2] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

	printf("MAX: 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2]);
	
}

void dspin_driver::test2()
{
	uint8_t out[8], in[8];

	out[0] = out[1] = CMD_GET_PARAM | REG_CONFIG;
	out[2] = out[3] = 0;
	out[4] = out[5] = 0;
	
	for(int i = 0; i < 6; i+=2)
	{
		mover_p->transfer(2, &out[i], &in[i]);
		//usleep(1000);
	}
	
	printf("read: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x.\r\n", in[0], in[1], in[2], in[3], in[4], in[5]);

	out[0] = out[1] = CMD_GET_PARAM | REG_STATUS;
	out[2] = out[3] = 0;
	out[4] = out[5] = 0;
	
	for(int i = 0; i < 6; i+=2)
	{
		mover_p->transfer(2, &out[i], &in[i]);
		//usleep(1000);
	}
	
	printf("read: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x.\r\n", in[0], in[1], in[2], in[3], in[4], in[5]);
	
	out[0] = out[1] = CMD_GO_UNTIL;
	out[2] = out[3] = 0;
	out[4] = out[5] = 14;
	out[6] = out[7] = 0;
	
	out[0] |= 0x01; // other direction
	
	for(int i = 0; i < 8; i+=2)
	{
		mover_p->transfer(2, &out[i], &in[i]);
		//usleep(1000);
	}
	
	//printf("read: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x.\r\n", in[0], in[1], in[2], in[3], in[4], in[5]);
	
	
}

uint16_t dspin_driver::get_status()
{
	uint8_t out[4], in[4];
	uint16_t retval;

	// read status
	out[0] = CMD_GET_STATUS;
	out[1] = 0;
	out[2] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	//printf("Status: 0x%x 0x%x, 0x%x\r\n", in[0], in[1], in[2]);

	retval = (in[1] << 8) | in[2];
	
	return retval;
}

uint32_t dspin_driver::get_pos()
{
	uint8_t out[4], in[4];
	uint32_t retval = 0;

	// read abs pos
	out[0] = CMD_GET_PARAM | REG_ABS_POS;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 4; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	//printf("position: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);

	retval = (in[1] << 16) | (in[2] << 8) | in[3];
	
	return retval;
}

uint16_t dspin_driver::get_config()
{
	uint16_t retval;
	uint8_t out[4], in[4];

	// read abs pos
	out[0] = CMD_GET_PARAM | REG_CONFIG;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	//printf("position: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);

	retval = (in[1] << 8) | in[2];
	
	return retval;
}

void 	 dspin_driver::set_config(uint16_t val)
{
	uint8_t out[4], in[4];

	// read abs pos
	out[0] = CMD_SET_PARAM | REG_CONFIG;
	out[1] = (val & 0xff00) >> 8;
	out[2] = val & 0x00ff;
	out[3] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	//printf("position: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);

}

uint8_t  dspin_driver::get_adc_val()
{
	uint8_t out[4], in[4];

	// read abs pos
	out[0] = CMD_GET_PARAM | REG_ADC_OUT;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;

	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
	}
	
	return in[1];
}


void dspin_driver::thwack_kvals()
{
	uint8_t out[4], in[4];

	out[0] = CMD_SET_PARAM | REG_KVAL_HOLD;
	out[1] = 0xff;
	
	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	out[0] = CMD_SET_PARAM | REG_KVAL_RUN;
	out[1] = 0xff;
	
	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
	out[0] = CMD_SET_PARAM | REG_KVAL_DEC;
	out[1] = 0xff;
	
	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

	out[0] = CMD_SET_PARAM | REG_KVAL_ACC;
	out[1] = 0xff;
	
	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

		
}

void dspin_driver::find_home()
{
	uint8_t out[4], in[4];

	// Try to put us in home position.
	// This assumes that if we're not at home, it needs to move backwards to get there.
	//
	// This has the side effect of setting the abs pos register.
	
	printf("find_home()\r\n");
	
	uint16_t status;
	
	status = get_status();
	
	if(status & STATUS_SWITCH_CLOSED_FLAG)
	{
	
		printf("Already Home: status x%x\r\n", status);
		// home switch is closed...we're done
		return;
	}
	
	out[0] = CMD_GO_UNTIL | 0x01;
	out[1] = 0x00;
	out[2] = 0x14;
	out[3] = 0x14;

	for(int i = 0; i < 4; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
	
	// now poll status...
	while(is_switch_closed())
	{
		printf("hunting...(0x%x)\r\n", status);
		usleep(100000);
	};

	do
	{
		status = get_status();
		printf("hunting2...(0x%x)\r\n", status);
		usleep(10000);
	}while(!(status & STATUS_BUSY_FLAG)); // BZY is active low

	printf("Found home!  Position 0x%x, ", get_pos());
}

void dspin_driver::release_switch()
{
	uint8_t out[4], in[4];
	uint16_t status;
	
	out[0] = CMD_RELEASE_SW ;//| 0x01;
	
	for(int i = 0; i < 1; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

	// now poll status...
	while(is_switch_closed())
	{
		printf(" rel hunting...(0x%x)\r\n", status);
		usleep(100000);
	};
}

bool dspin_driver::is_switch_closed()
{
	uint16_t status;
	
	status = get_status();
	
	return (status & STATUS_SWITCH_CLOSED_FLAG);
}

void dspin_driver::set_step_mode(bool full)
{
	uint8_t out[4], in[4];
	//uint32_t retval = 0;

	// write step mode register
	out[0] = CMD_SET_PARAM | REG_STEP_MODE;
	out[1] = 0;
	if(!full)
	{
		// call it quarter step?
		out[1] |= 0x02;
	}

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}

}

		
void dspin_driver::run(bool forward)
{
	uint8_t out[4], in[4];

	// try to read status
	out[0] = CMD_RUN;
	if(forward)
	{
		out[0] |= 0x01;
	}
	out[1] = 0;
	out[2] = 41;
	//out[3] = 41;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	//printf("run resp: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);
}

void dspin_driver::stop(bool hard)
{
	uint8_t out[4], in[4];

	out[0] = CMD_STOP;
	if(hard)
	{
		out[0] |= 0x08;
	}

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 1; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	printf("sent %s stop\r\n", (hard ? "hard": "soft"));
}
		
void dspin_driver::move(bool forward, uint16_t steps)
{
	uint8_t out[4], in[4];

	// A step was really tiny...
	steps *= 10;
		
	out[0] = CMD_MOVE;
	if(forward)
	{
		out[0] |= 0x01;
	}
	out[1] = 0;
	out[2] = (steps & 0xff00) >> 8;
	out[3] = steps & 0xff;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 4; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
	}
		
	printf("sent move, x%x, x%x\r\n", forward, steps);	
}		
		
void dspin_driver::reset()
{
	uint8_t out[4], in[4];

	// try to reset
	out[0] = 0xc0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 1; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		//usleep(1000);
	}
		
	printf("sent reset\r\n");
}
		
#if 0		
void dspin_driver::thread_member_func(int i)
{
	char c;
	uint32_t buff_sz = NUM_STRINGS * CHARS_PER_STRING * 2;
	uint8_t out[buff_sz];
	uint32_t idx = 0;
	
	// Only needed because the spi_mover only does bidirectional xfer...
	// if it had a plain "write" we might not need this.
	uint8_t in[NUM_STRINGS * CHARS_PER_STRING * 2];

	pthread_mutex_lock(&data_mutex);

	// Render the string buffer
	for(int i = 0; i < NUM_STRINGS; i++)
	{
		for(int j = CHARS_PER_STRING-1; j >= 0 ; j--)
		{		
			//std::cout << "i: " << i << " j: " j << std::endl;
		
			// working backwards
			c = strings[i]->get_current(j);

			out[idx] = (font[c] & 0xff00) >> 8;
			out[idx+1] = font[c] & 0xff;

			idx+=2;
		}
		
		strings[i]->next_step();
	}
	
	// then write the buffer to the dspins
	if( ! mover_p->transfer(buff_sz, out, in))
	{
		std::cerr << "dspin transfer() error!" << std::endl;
	}
	
	
	pthread_mutex_unlock(&data_mutex);
}

#endif