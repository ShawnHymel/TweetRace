#include <unistd.h>
#include <iostream>

#include "display_driver.h"

// static inits
bool display_driver::terminate = false;
display_driver * display_driver::instance_p = NULL;

display_driver::display_driver()
{
	mover_p = new spi_mover("/dev/spidev0.0");

	for(int i = 0; i < NUM_STRINGS; i++)
	{
		strings[0] = NULL;
		str_lens[0] = 0;
	}
	
	// spawn a thread from here?
	display_driver::terminate = false;
	pthread_create(&worker_thread, // thread structure
					NULL, 			// attributes
					&thread_func,	// body
					NULL);			// argv
					
	instance_p = this;
}

display_driver::~display_driver()
{

	display_driver::terminate = true;
	
	pthread_join(worker_thread, NULL);

	delete mover_p;
}

		
void display_driver::update_string(int index, char * new_string)
{

}
		
void* display_driver::thread_func(void * X)
{
	uint32_t i = 0;
	
	while(1)
	{
		if(display_driver::terminate)
		{
			std::cout << "thread exiting" << std::endl;
			pthread_exit(0);
		}
	
		for(i = 0; i < 16; i++)
		{
			if(instance_p != NULL)
			{
				instance_p->thread_member_func(i);
			}
			
			sleep(5);
		}
	
		//usleep(1000000);
		sleep(1);
		std::cout << "." << std::endl;
	}
}

void display_driver::thread_member_func(int i)
{
	uint8_t out[2], in[2];

	out[0] = font[i] & 0xff;
	out[1] = (font[i] & 0xff00) >> 8;

	mover_p->transfer(2, out, in);
	
	std::cout << "sending segment: " << i << std::endl;
}

const uint16_t display_driver::font[16] =
{
	0x0001,
	0x0002,
	0x0004,
	0x0008,
	0x0010,
	0x0020,
	0x0040,
	0x0080,
	0x0100,
	0x0200,
	0x0400,
	0x0800,
	0x1000,
	0x2000,
	0x4000,
	0x8000
};


