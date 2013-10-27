#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include "dspin_driver.h"


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

	for(int i = 0; i < 4; i++)
	{
		mover_p->set_mode((i & 0x01), (i & 0x02));
	
		// try to read status
		//out[0] = 0x20 | 0x18;
		out[0] = 0x0d;
		out[1] = 0;
		out[2] = 0;
		out[3] = 0;

		// Funny chip select semantics...each byte needs a discrete ship select strobe, or it just shifts out the other side
		for(int i = 0; i < 4; i++)
		{
			mover_p->transfer(1, &out[i], &in[i]);
			usleep(1000);
		}
		
		//std::cout << "Received:" << std::hex << in[0] << in[1] << in[2] << in[3] << std::dec << std::endl;
		printf("Rcvd: 0x%x 0x%x, 0x%x, 0x%x\r\n", in[0], in[1], in[2], in[3]);
	}	
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