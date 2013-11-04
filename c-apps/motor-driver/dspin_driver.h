#ifndef _DSPIN_DRIVER_H_
#define _DSPIN_DRIVER_H_

#include <stdint.h>
//#include <pthread.h>

#include "spi_mover.h"

class dspin_driver
{
	public:
		static const uint32_t NUM_MOTORS = 2;//5;
	
		dspin_driver(bool verbose = false);
		~dspin_driver();
		
		void test();
		void test2(uint32_t channel);
		void test3();
		
		bool reset(uint32_t channel);
		bool set_config(uint32_t channel, uint8_t kval, bool full);
		
		void find_home(uint32_t channel, bool blocking);
		void release_switch(uint32_t channel);
		bool is_switch_closed(uint32_t channel);

		void set_step_mode(uint32_t channel, bool full);
		
		uint8_t  get_adc_val(uint32_t channel);
		
		uint32_t get_pos(uint32_t channel);

		void run(uint32_t channel, bool forward);
		void stop(uint32_t channel, bool hard);
		void move(uint32_t channel, bool forward, uint16_t steps);
		
		void reset();
		
	private:
	
		uint32_t reg_read(uint32_t channel, uint32_t regnum, uint32_t len);
		bool     reg_write(uint32_t channel, uint32_t regnum,  uint32_t len, uint32_t value);
	
		bool send_cmd_single(uint32_t channel, uint32_t len, uint8_t * out_data, uint8_t * in_data);
	
	
		spi_mover * mover_p;
		bool        m_verbose;
};

#endif // keepout