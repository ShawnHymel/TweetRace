#include <stdio.h>
#include <unistd.h>

#include "hoss_system.h"



hoss_system::hoss_system()
{
	printf("HS: constructor\r\n");

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_status[i]     = eUNKNOWN;	
		m_increments[i] = 0;
		m_positions[i]  = 0;
	}
	
#ifdef DUMMY

	
#else
	
	m_motor_p = new dspin_driver();

	m_motor_p->reset();
	
	uint16_t cfg;
	cfg = m_motor_p->get_config();
	printf("Initial config: 0x%x\r\n", cfg);
	cfg &= 0x0010;
	m_motor_p->set_config(cfg);
	
	m_motor_p->set_step_mode(true); // to full steps

	sleep(1);
	
	m_motor_p->thwack_kvals();

	sleep(1);

#endif
}

hoss_system::~hoss_system()
{
	printf("HS: destructor\r\n");

#ifdef DUMMY
#else

	delete m_motor_p;
	
#endif
}
		
void hoss_system::find_home()
{
	printf("HS: find_home\r\n");


#ifdef DUMMY

	sleep(10);
	
#else

	//for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_motor_p->find_home();
		
		m_motor_p->release_switch();
	}

	
#endif
}


bool hoss_system::is_any_at_far_end()
{
	printf("HS: is_any_at_far_end\r\n");

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
		val = m_motor_p->get_adc_val();

		printf("ADC[%d]: 0x%x\r\n", i, val);
		
		if(val & 0x10)
		{
			return true;
		}

	}

	return false;
#endif	
	
}

uint32_t hoss_system::get_winner()
{
	printf("HS: get_winner\r\n");
	
#ifdef DUMMY
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if(m_status[i] == eAT_FAR_END)
		{
			return i;
		}
	}
	
	return 0xffffffff;
#else

	return 0;
#endif

}

hoss_status hoss_system::get_status(uint32_t track_num)
{
	printf("HS: get_status 0x%x\r\n", track_num);

	return m_status[track_num];
}
		
bool hoss_system::set_race_value(uint32_t track_num, uint32_t increment)
{
	printf("HS: set_race_value 0x%x, 0x%x\r\n", track_num, increment);

	if(track_num >= NUM_HOSSES)
	{
		return false;
	}

	m_increments[track_num] = increment;
	
	return true;

}

bool hoss_system::race()
{
	printf("HS race\r\n");

#ifdef DUMMY

	printf("Racing! 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n", 
			m_increments[0],
			m_increments[1],
			m_increments[2],
			m_increments[3],
			m_increments[4]);

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
		m_motor_p->move(false, m_increments[i]);
		m_increments[i] = 0;
	}

	return false;
#endif
}
