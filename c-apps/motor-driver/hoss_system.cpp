#include <stdio.h>
#include <unistd.h>

#include "hoss_system.h"



hoss_system::hoss_system(bool verbose)
{
	m_verbose = verbose;

	if(m_verbose)
	{
		printf("HS: constructor\r\n");
	}

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_status[i]     = eUNKNOWN;	
		m_increments[i] = 0;
		m_positions[i]  = 0;
	}
	
#ifdef DUMMY

	
#else
	
	m_motor_p = new dspin_driver(verbose);

	m_motor_p->reset();

	for(uint32_t track = 0; track < NUM_HOSSES; track++)
	{	
		m_motor_p->set_config(track, 
								0xff, // kval
								true);
								//...more param to come?
								
	}

	
#endif
}

hoss_system::~hoss_system()
{
	if(m_verbose)
	{
		printf("HS: destructor\r\n");
	}

#ifdef DUMMY
#else

	m_motor_p->reset();

	delete m_motor_p;
	
#endif
}
		
void hoss_system::find_home()
{
	if(m_verbose)
	{
		printf("HS: find_home\r\n");
	}

#ifdef DUMMY

	sleep(10);
	
#else

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_motor_p->find_home(i, false);
		// false means nonblocking - start resetting, then return
	}
	
	// now poll status...
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		while(!m_motor_p->is_switch_closed(i))
		{
			if(m_verbose)
			{
				printf("hunting...[%d]...\r\n", i);
			}
			usleep(1000000);
		};
	}

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_motor_p->release_switch(i);
		m_status[i] = eAT_HOME;
	}

	
#endif
}


bool hoss_system::is_any_at_far_end()
{
	if(m_verbose)
	{
		printf("HS: is_any_at_far_end\r\n");
	}

#ifdef DUMMY
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if( m_status[i] == eAT_FAR_END)	
		{
			return true;
		}
	}
	
	return false;
#else

	uint8_t val;

	// TBD - again, large parallel operation?
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		val = m_motor_p->get_adc_val(i);

		if(m_verbose)
		{
			printf("ADC[%d]: 0x%x\r\n", i, val);
		}
		
		if(val & 0x10)
		{
			m_status[i] = eAT_FAR_END;
			return true;
		}

	}

	return false;
#endif	
	
}

uint32_t hoss_system::get_winner()
{
	if(m_verbose)
	{
		printf("HS: get_winner\r\n");
	}
	
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if(m_status[i] == eAT_FAR_END)
		{
			return i;
		}
	}
	
	return 0xffffffff;
}

hoss_status hoss_system::get_status(uint32_t track_num)
{
	if(m_verbose)
	{
		printf("HS: get_status 0x%x\r\n", track_num);
	}

	return m_status[track_num];
}
		
bool hoss_system::set_race_value(uint32_t track_num, uint32_t increment)
{
	if(m_verbose)
	{
		printf("HS: set_race_value 0x%x, 0x%x\r\n", track_num, increment);
	}

	if(track_num >= NUM_HOSSES)
	{
		return false;
	}

	m_increments[track_num] = increment;
	
	return true;

}

bool hoss_system::race()
{
	if(m_verbose)
	{
		printf("Racing! 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n", 
				m_increments[0],
				m_increments[1],
				m_increments[2],
				m_increments[3],
				m_increments[4]);
	}

#ifdef DUMMY


	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_positions[i] += m_increments[i];
		m_increments[i] = 0;
		
		if(m_status[i] == eAT_HOME)
		{
			m_status[i] = eRUNNING;
		}
		
		if(m_positions[i] >= 30)
		{
			m_status[i] = eAT_FAR_END;
		}
	}

	return true;
	
#else

	// TBD- this will really be a single command, issued to all in parallel?

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_motor_p->move(i, true, m_increments[i]);
		m_increments[i] = 0;

		if(m_status[i] == eAT_HOME)
		{
			m_status[i] = eRUNNING;
		}
	}
	return true;
#endif
}
