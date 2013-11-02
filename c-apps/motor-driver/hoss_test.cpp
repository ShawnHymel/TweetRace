#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "hoss_system.h"

int main()
{
	srand(time(NULL));

	hoss_system * hp;
	
	hp = new hoss_system();
	
	// wont return until they're all at the starting line.
	hp->find_home();
	
	
	sleep(1);
	printf("starting race\r\n");
	
	while(!hp->is_any_at_far_end())
	{
		for(uint32_t i = 0; i < NUM_HOSSES; i++)
		{	
			hp->set_race_value(i, (rand()%5)*10);
		}
	
		hp->race();
	
		sleep(1);
	}
	
	// someone reached the end
	uint32_t winner = hp->get_winner();
	printf("Winner is 0x%x\r\n", winner);

	sleep(5);

	delete(hp);

	return 0;
}
