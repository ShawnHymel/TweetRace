#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include "dspin_driver.h"


// status masks
static const uint16_t STATUS_HIZ_FLAG          = 0x0001;
static const uint16_t STATUS_BUSY_FLAG         = 0x0002;
static const uint16_t STATUS_SWITCH_CLOSED_FLAG = 0x0004;
static const uint16_t STATUS_SWITCH_EVENT_FLAG = 0x0008;
static const uint16_t STATUS_DIR_FLAG          = 0x0010;
static const uint16_t STATUS_MOTOR_STAT_MASK   = 0x0060;
static const uint16_t STATUS_NOPERF_CMD_FLAG   = 0x0080;

static const uint16_t STATUS_WRONG_CMD_FLAG    = 0x0100;
static const uint16_t STATUS_UNDER_V_FLAG      = 0x0200;// active low
static const uint16_t STATUS_THERM_WARN_FLAG   = 0x0300;// active low
static const uint16_t STATUS_THERM_SDN_FLAG    = 0x0800;
static const uint16_t STATUS_OVR_I_FLAG        = 0x1000;
static const uint16_t STATUS_LOSS_A_FLAG       = 0x2000;
static const uint16_t STATUS_LOSS_B_FLAG       = 0x4000;
static const uint16_t STATUS_SCK_MODE_FLAG     = 0x8000;


dspin_driver::dspin_driver()
{
	mover_p = new spi_mover("/dev/spidev0.1");
	
	mover_p->set_mode(true, true);
}

dspin_driver::~dspin_driver()
{
	delete mover_p;
}

void dspin_driver::test()
{
	uint8_t out[4], in[4];

	// try to read status
	//out[0] = 0x20 | 0x18;
	out[0] = 0xd0;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}
		
	printf("Status: 0x%x 0x%x, 0x%x\r\n", in[0], in[1], in[2]);
	
	// try to read ADC
	out[0] = 0x32;
	out[1] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 2; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}

	printf("ADC: 0x%x, 0x%x\r\n", in[0], in[1]);

	// try to read min speed
	out[0] = 0x28;
	out[1] = 0x00; 
	out[2] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}

	printf("MIN: 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2]);

	// try to read max speed
	out[0] = 0x27;
	out[1] = 0x00; 
	out[2] = 0x00; 

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 3; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}

	printf("MAX: 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2]);
	
}

uint16_t dspin_driver::get_status()
{
	uint8_t out[4], in[4];
	uint16_t retval;

	// read status
	//out[0] = 0x20 | 0x18;
	out[0] = 0xd0;
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
	out[0] = 0x20 | 0x01;
	out[1] = 0;
	out[2] = 0;

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 4; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}
		
	//printf("position: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);

	retval = (in[1] << 16) | (in[2] << 8) | in[3];
	
	return retval;
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
	
	out[0] = 0x82;
	out[1] = 0x00;
	out[2] = 0x14;
	out[3] = 0x14;

	for(int i = 0; i < 4; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}
	
	// now poll status...
	
	do
	{
		status = get_status();
		printf("hunting...(0x%x)\r\n", status);
		usleep(10000);
	}while(!(status & STATUS_SWITCH_CLOSED_FLAG)); // BZY is active low

	do
	{
		status = get_status();
		printf("hunting2...(0x%x)\r\n", status);
		usleep(10000);
	}while(!(status & STATUS_BUSY_FLAG)); // BZY is active low

	
	printf("Found home!  Position 0x%x, ", get_pos());
}

void dspin_driver::set_step_mode(bool full)
{
	uint8_t out[4], in[4];
	uint32_t retval = 0;

	// write step mode register
	out[0] = 0x16;
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
		usleep(1000);
	}

}

		
void dspin_driver::run(bool forward)
{
	uint8_t out[4], in[4];

	// try to read status
	out[0] = 0x50;
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
		usleep(1000);
	}
		
	//printf("run resp: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);
}

void dspin_driver::stop(bool hard)
{
	uint8_t out[4], in[4];

	// try to read status
	out[0] = 0xb0;
	if(hard)
	{
		out[0] |= 0x08;
	}

	// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
	for(int i = 0; i < 1; i++)
	{
		mover_p->transfer(1, &out[i], &in[i]);
		usleep(1000);
	}
		
	printf("sent %s stop\r\n", (hard ? "hard": "soft"));
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
		usleep(1000);
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