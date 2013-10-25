#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_

#include <stdint.h>
#include <pthread.h>

#include "spi_mover.h"

class display_driver
{
	public:
		static const int NUM_STRINGS = 1;//5;
		static const int CHARS_PER_STRING = 1;//8;
	
		display_driver();
		~display_driver();
		
		void update_string(int index, char * new_string);
		
	private:
		static display_driver * instance_p;
		
		spi_mover * mover_p;
		
		
		char * strings[NUM_STRINGS];
		uint32_t str_lens[NUM_STRINGS];

		// the update thread
		pthread_t worker_thread;
		static bool      terminate;  // should be a mutex?
		static void* thread_func(void * X);
		
		void thread_member_func(int i);
		
		// font data...
		static const uint16_t font[16];
};

#endif // keepout