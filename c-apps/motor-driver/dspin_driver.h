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
	private:
		spi_mover * mover_p;
		
};

#endif // keepout