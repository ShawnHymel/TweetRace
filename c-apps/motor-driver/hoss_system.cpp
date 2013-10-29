#include <stdio.h>

#include "hoss_system.h"

// for interface w/o hardware testing...
#define DUMMY 1

#if DUMMY 
static uint32_t fake;
#endif

hoss_system::hoss_system()
{
	printf("HS: constructor\r\n");
	
#if DUMMY

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_status[i]     = eUNKNOWN;	
		m_increments[i] = 0;
		m_positions[i]  = 0;
	}

	fake = 0;
	
#else
#endif
}

hoss_system::~hoss_system()
{
	printf("HS: destructor\r\n");

#if DUMMY
#else
#endif
}
		
void hoss_system::find_home()
{
	printf("HS: find_home\r\n");


#if DUMMY

	fake = 0;

	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		m_status[i]     = eSEEKING;	
	}
	
#else
#endif
}

bool hoss_system::is_any_seeking()
{
	printf("HS: is_any_seeking\r\n");

#if DUMMY
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if( (i == 0) && ( m_status[i] == eSEEKING)	)
		{
			fake++;
			if(fake == 20)
			{
				for(uint32_t i = 0; i < NUM_HOSSES; i++)
				{
					m_status[i] = eAT_HOME;
				}
			}
		
			return true;
		}
	}
	
	return false;
#else
#endif	
}

bool hoss_system::is_any_at_far_end()
{
	printf("HS: is_any_at_far_end\r\n");

#if DUMMY
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if( m_status[i] == eAT_FAR_END)	
		{
			return true;
		}
	}
	
	return false;
#else
#endif	
	
}

uint32_t hoss_system::get_winner()
{
	printf("HS: get_winner\r\n");
	
#if DUMMY
	for(uint32_t i = 0; i < NUM_HOSSES; i++)
	{
		if(m_status[i] == eAT_FAR_END)
		{
			return i;
		}
	}
	
	return 0xffffffff;
#else
#endif

}

hoss_status hoss_system::get_status(uint32_t track_num)
{
	printf("HS: get_status 0x%x\r\n", track_num);

#if DUMMY

	switch(m_status[track_num])
	{
	case eSEEKING:
	{
		fake++;
		if(fake == 20)
		{
			for(uint32_t i = 0; i < NUM_HOSSES; i++)
			{
				m_status[i] = eAT_HOME;
			}
		}
		
		return m_status[track_num];
	}	
	break;
	
	default:
		return m_status[track_num];
	};

#else
#endif
}
		
bool hoss_system::set_race_value(uint32_t track_num, uint32_t increment)
{
	printf("HS: set_race_value 0x%x, 0x%x\r\n", track_num, increment);

#if DUMMY

	if(track_num >= NUM_HOSSES)
	{
		return false;
	}

	m_increments[track_num] = increment;
	
	return true;

#else
#endif
}

bool hoss_system::race()
{
	printf("HS race\r\n");

#if DUMMY

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
#endif
}
