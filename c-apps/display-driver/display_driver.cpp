#include <unistd.h>
#include <iostream>

#include "display_driver.h"

bool display_driver::terminate = false;;

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
	while(1)
	{
		if(display_driver::terminate)
		{
			std::cout << "thread exiting" << std::endl;
			pthread_exit(0);
		}
	
		usleep(1000000);
		std::cout << "." << std::endl;
	}
}