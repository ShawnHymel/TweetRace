
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "dspin_driver.h"

int main(void)
{
	printf("testing!!\r\n");


	dspin_driver * motor_p;

	motor_p = new dspin_driver(true);

#if 1
	for(int i = 0 ; i < 100; i++)
	{
		motor_p->test3();
		sleep(10);
	}
#endif	
	
#if 0
	
	printf("testing1!!\r\n");
	motor_p->test2(0);

	sleep(10);

	printf("testing2!!\r\n");
	motor_p->test2(1);

	sleep(10);

	

	printf("testing3!!\r\n");
	//motor_p->test2(2);

	sleep(10);
#endif	
	
	delete motor_p;
	
#if 0
	uint32_t pos;

	dspin_driver * motor_p;

	motor_p = new dspin_driver();

	motor_p->reset();
	
	//motor_p->test();
	//printf("### initial position x%x\r\n", motor_p->get_pos());

	uint16_t cfg;
	cfg = motor_p->get_config();
	printf("Initial config: 0x%x\r\n", cfg);
	
	cfg &= 0x0010;
	
	motor_p->set_config(cfg);
	
	motor_p->set_step_mode(true); // to full steps

	sleep(1);
	
	motor_p->thwack_kvals();

	sleep(1);
	
	motor_p->find_home(); 
	
	motor_p->release_switch();
	
	// For some reason, required to make it work...
	sleep(1);	
	
	motor_p->run(true);

	cfg = motor_p->get_config();
	printf("Initial config: 0x%x\r\n", cfg);
	
	for(int i = 0; i < 5; i++)
	{
		printf("###running position x%x, status x%x\r\n", motor_p->get_pos(), motor_p->get_status());
		sleep(1);
	}
	
	motor_p->stop(false);

	printf("###post run position x%x\r\n", motor_p->get_pos());

	sleep(1);
	
	motor_p->run(false);

	cfg = motor_p->get_config();
	printf("Initial config: 0x%x\r\n", cfg);
	
	for(int i = 0; i < 5; i++)
	{
		printf("###running position x%x, status x%x\r\n", motor_p->get_pos(), motor_p->get_status());
		sleep(1);
	}
	
	motor_p->stop(false);

	printf("###post run position x%x\r\n", motor_p->get_pos());

	cfg = motor_p->get_config();
	printf("Initial config: 0x%x\r\n", cfg);
	
	motor_p->test();
	
	sleep(5);
	
	motor_p->stop(true);
	
	motor_p->test();
	
	delete motor_p;
#endif	
}