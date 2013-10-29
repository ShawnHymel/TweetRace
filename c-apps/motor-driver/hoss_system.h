#ifndef _HOSS_SYSTEM_H_
#define _HOSS_SYSTEM_H_

#include <stdint.h>

// Class for keepin track of hosses

static const uint32_t NUM_HOSSES = 5;
	
enum hoss_status
{
	eUNKNOWN = 0,
	eSEEKING,
	eAT_HOME,
	eRUNNING,
	eAT_FAR_END,
	eERROR,
	eMAX // for range checking
};


class hoss_system
{
	public:
	
	
		hoss_system();
		~hoss_system();
		
		void find_home();
		bool is_any_seeking();
		bool is_any_at_far_end();
		uint32_t get_winner();

		hoss_status get_status(uint32_t track_num);
		
		bool        set_race_value(uint32_t track_num, uint32_t increment);
		bool        race();
		
		
		
	private:
		hoss_status m_status[NUM_HOSSES];
	
		uint32_t    m_increments[NUM_HOSSES];

		uint32_t    m_positions[NUM_HOSSES];
};


#endif