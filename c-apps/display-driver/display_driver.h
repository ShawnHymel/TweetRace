#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_

#include <stdint.h>
#include <pthread.h>

#include "spi_mover.h"
#include "string_cb.h"

class display_driver
{
	public:
		static const int NUM_STRINGS = 5;//5;
		static const int CHARS_PER_STRING = 8;//8;
	
		display_driver();
		~display_driver();
		
		bool update_string(int index, char * new_string);
		
	private:
		static display_driver * instance_p;
		
		spi_mover * mover_p;
		
		string_cb * strings[NUM_STRINGS];

		// the update thread
		pthread_t       worker_thread;
		pthread_mutex_t data_mutex;
		static bool      terminate;  // should be a mutex?
		static void* thread_func(void * X);
		
		void thread_member_func(int i);
		
		// font data...
		static const uint16_t font[128];
};

#endif // keepout