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
}

void dspin_driver::test2(uint32_t chan)
{
	//uint8_t out[4], in[4];

	printf("Test2, i = x%x\r\n", chan);

	uint32_t config;
	config = reg_read(chan, REG_CONFIG, 2);
	printf("Config: 0x%x.\r\n", config);
	//config &= 0x0010;
	//reg_write(chan, REG_CONFIG, 2, config);
	
	
	uint32_t status;
	status = reg_read(chan, REG_STATUS, 2);
	printf("Status: 0x%x.\r\n", status);
	
	set_config(chan, 0xff, true);
#if 0	
	sleep(1);
	
	//reg_write(chan, REG_KVAL_HOLD, 1, 0xff);
	reg_write(chan, REG_KVAL_RUN, 1, 0xff);
	reg_write(chan, REG_KVAL_ACC, 1, 0xff);
	reg_write(chan, REG_KVAL_DEC, 1, 0xff);
	
	sleep(1);
	
	set_step_mode(chan, true);
#endif
	
	sleep(1);
	
	find_home(chan);
	
	sleep(1);
	
	release_switch(chan);
	
	sleep(1);
	
	move(chan, true, 5);
	
	sleep(10);

	move(chan, false, 4);
	
	sleep(10);
	
#if 0	
	// run fwd
	run(chan, true);
	
	sleep(5);
	
	stop(chan, false);
	//stop(chan, true);
	
	sleep(1);
	
	// run back 
	run(chan, false);
	
	sleep(3);
	
	stop(chan, false);
#endif	
	sleep(1);
#if 0	
	out[0] = CMD_GO_UNTIL;
	out[1] = 0;
	out[2] = 14;
	out[3] = 0;
	
	//out[0] |= 0x01; // other direction
	
	send_cmd_single(chan, 4, out, in);

	printf("read: 0x%x, 0x%x, 0x%x, 0x%x.\r\n", in[0], in[1], in[2], in[3]);
#endif	


}

void dspin_driver::test3()
{
	uint16_t status[2];
	
	status[0] = reg_read(0, REG_STATUS, 2);
	status[1] = reg_read(1, REG_STATUS, 2);
	
	printf("Statuses x%x, x%x\r\n\r\n", status[0], status[1]);
	
}

bool dspin_driver::set_config(uint32_t channel, uint8_t kval, bool full)
{


	if(channel >= NUM_MOTORS)
	{
		return false;
	}
	
	//reg_write(chan, REG_KVAL_HOLD, 1, 0xff);
	reg_write(channel, REG_KVAL_RUN, 1, kval);
	reg_write(channel, REG_KVAL_ACC, 1, kval);
	reg_write(channel, REG_KVAL_DEC, 1, kval);
	
	set_step_mode(channel, full);
	
	return true;
}

uint32_t dspin_driver::get_pos(uint32_t channel)
{
	uint32_t retval;
	
	retval = reg_read(channel, REG_ABS_POS, 3);
	
	return retval;
}


#if 0
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
#endif

void dspin_driver::find_home(uint32_t channel)
{
	uint8_t out[4], in[4];

	// Try to put us in home position.
	// This assumes that if we're not at home, it needs to move backwards to get there.
	//
	// This has the side effect of setting the abs pos register.
	
	uint32_t status;
	
	printf("find_home()\r\n");
	
	if(is_switch_closed(channel))
	{
	
		printf("Already Home\r\n");
		// home switch is closed...we're done
		return;
	}
	
	out[0] = CMD_GO_UNTIL;
	out[1] = 0x00;
	out[2] = 0x14;
	out[3] = 0x14;

	send_cmd_single(channel, 4, out, in);
	
	usleep(100000);
	
	// now poll status...
	while(!is_switch_closed(channel))
	{
		printf("hunting...\r\n");
		usleep(1000000);
	};

#if 1
	/// wait for busy to clear...
	do
	{
	
		status = reg_read(channel, REG_STATUS, 2);
		printf("hunting2...(0x%x)\r\n", status);
		usleep(10000);
	}while(!(status & STATUS_BUSY_FLAG)); // BZY is active low
#endif
	printf("Found home!  Position 0x%x, ", get_pos(channel));
}

void dspin_driver::release_switch(uint32_t channel)
{
	uint8_t out[4], in[4];
	//uint16_t status;
	
	out[0] = CMD_RELEASE_SW | 0x01;
		
	send_cmd_single(channel, 1, out, in);

	// now poll status...
	while(is_switch_closed(channel))
	{
		printf(" release hunting...\r\n");
		usleep(100000);
	};
}

bool dspin_driver::is_switch_closed(uint32_t channel)
{
	uint32_t status;
	
	status = reg_read(channel, REG_STATUS, 2);
	
	printf("is_closed[x%x]: status x%x, bool: x%x\r\n", channel, status, (status & STATUS_SWITCH_CLOSED_FLAG));
	
	return (status & STATUS_SWITCH_CLOSED_FLAG);
}

void dspin_driver::set_step_mode(uint32_t channel, bool full)
{
	if(full)
	{
		reg_write(channel, REG_STEP_MODE, 1, 0);
	}
	else
	{
		reg_write(channel, REG_STEP_MODE, 1, 0x02);
	}
}

uint8_t  dspin_driver::get_adc_val(uint32_t channel)
{
	uint8_t val;

	val = reg_read(channel, REG_ADC_OUT, 1);
	
	return val;
}

void dspin_driver::run(uint32_t channel, bool forward)
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
	out[3] = 41;

	send_cmd_single(channel, 4, out, in);
}

void dspin_driver::stop(uint32_t channel, bool hard)
{
	uint8_t out[4], in[4];

	out[0] = CMD_STOP;
	if(hard)
	{
		out[0] |= 0x08;
	}

	send_cmd_single(channel, 1, out, in);
}

void dspin_driver::move(uint32_t channel, bool forward, uint16_t steps)
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

	send_cmd_single(channel, 4, out, in);
		
	//printf("sent move, x%x, x%x\r\n", forward, steps);	

}

	
void dspin_driver::reset()
{
	uint8_t out[4], in[4];

	// try to reset
	out[0] = 0xc0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(uint32_t i = 0; i < NUM_MOTORS; i++)
	{
		send_cmd_single(i, 1, out, in);
	
		sleep(1);
	}
		
	printf("sent reset\r\n");
}

		
uint32_t dspin_driver::reg_read(uint32_t channel, uint32_t regnum,  uint32_t len)
{
	// len parameter is the size of the register to read.

	uint8_t out[4], in[4];
	uint32_t accum = 0;
	
	if(channel >= NUM_MOTORS)
	{
		return 0;
	}
	if(regnum > REG_STATUS)
	{
		return 0;
	}
	if((len == 0) || (len > 3))
	{
		return 0;
	}
	
	out[0] = CMD_GET_PARAM | regnum;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;
	
	if(send_cmd_single(channel, len+1, out, in))
	{
		for(uint32_t i = 0; i < len; i++)
		{
			accum <<= 8;
			accum |= in[i+1];
		}
	}

	return accum;
}

bool dspin_driver::reg_write(uint32_t channel, uint32_t regnum,  uint32_t len, uint32_t value)
{
	// len parameter is the size of the register to read.

	uint8_t out[4], in[4];
	uint32_t accum = 0;
	
	if(channel >= NUM_MOTORS)
	{
		return 0;
	}
	if(regnum > REG_STATUS)
	{
		return 0;
	}
	if((len == 0) || (len > 3))
	{
		return 0;
	}

	out[0] = CMD_SET_PARAM | regnum;
	for(uint32_t i = 0; i < len; i++)
	{
		out[i+1] = (value >> (8*(len-i-1))) & 0xff;
	}
	
	if(send_cmd_single(channel, len+1, out, in))
	{
		return true;
	}

	return accum;
}
		
bool dspin_driver::send_cmd_single(uint32_t channel, uint32_t len, uint8_t * out_data, uint8_t * in_data)
{
	uint8_t out[NUM_MOTORS], in[NUM_MOTORS];
	
	printf("num x%x, chan x%x, len x%x\r\n", NUM_MOTORS, channel, len);
	
	if(channel >= NUM_MOTORS)
	{
		printf("MOTOR ERROR: bad channel num requested: 0x%x\r\n", channel);
		return(false);
	}

#if 1	
	
	// the commands are formatted funny, due to that chip select funkyness.
	// We'll send all of the command bytes in one chunk, then send all of the data bytes in separate chunks.
	// so we'll do len # of transfers, of channel bytes each.
	// The channels we were addressing should just ignore it
	
	for(uint32_t i = 0; i < len; i++)
	{
		//printf("a\r\n");
		// set everything to NOP cmds...
		memset(out, CMD_NOP, NUM_MOTORS);
	
		//printf("b\r\n");
		// then overwrite the one we want to actually send...
		out[channel] = out_data[i] ;
		
#if 0		
		printf("Sending :");
		for(uint32_t ii = 0; ii < NUM_MOTORS; ii++)
		{
			printf("x%x:x%x ",ii,out[ii]);
		}
		
		printf("\r\n");
#endif
		// And send it
		if(!mover_p->transfer(NUM_MOTORS, out, in))
		{
			printf("ERROR: spi transfer...\r\n");
			return false;
		}

#if 0		
		printf("Received :");
		for(uint32_t ii = 0; ii < NUM_MOTORS; ii++)
		{
			printf("x%x:x%x ",ii,in[ii]);
		}
		printf("\r\n");
#endif
		
		//printf("d\r\n");
		// then copy back the response
		in_data[i] = in[channel];

		//printf("e\r\n");
	}
	
#endif	
	return true;
}
