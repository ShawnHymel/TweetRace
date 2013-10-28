#ifndef _DSPIN_DRIVER_H_
#define _DSPIN_DRIVER_H_

#include <stdint.h>
//#include <pthread.h>

#include "spi_mover.h"

class dspin_driver
{
	public:
		static const int NUM_MOTORS = 1;//5;
	
		dspin_driver();
		~dspin_driver();
		
		void test();
		
		uint16_t get_status();
		uint32_t get_pos();
		
		uint16_t get_config();
		void 	 set_config(uint16_t val);
		
		void thwack_kvals();
		
		void find_home();
		void release_switch();
		bool is_switch_closed();
		
		void set_step_mode(bool full);
		
		void run(bool forward);
		void stop(bool hard);
		void reset();
		
	private:
	
		
	
		spi_mover * mover_p;
		
};

#endif // keepout