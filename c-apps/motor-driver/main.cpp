
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "dspin_driver.h"

int main(void)
{
	uint32_t pos;

	dspin_driver * motor_p;

	motor_p = new dspin_driver();

	motor_p->reset();
	
	motor_p->test();
	
	printf("### initial position x%x\r\n", motor_p->get_pos());

	motor_p->set_step_mode(true); // to full steps
	
	motor_p->run(true);

	for(int i = 0; i < 30; i++)
	{
		printf("###running position x%x, status x%x\r\n", motor_p->get_pos(), motor_p->get_status());
		sleep(1);
	}
	
	motor_p->stop(false);

	printf("###post run position x%x\r\n", motor_p->get_pos());

	motor_p->run(false);

	for(int i = 0; i < 30; i++)
	{
		printf("###running position x%x, status x%x\r\n", motor_p->get_pos(), motor_p->get_status());
		sleep(1);
	}
	
	motor_p->stop(false);

	printf("###post run position x%x\r\n", motor_p->get_pos());

	
	motor_p->test();
	
	sleep(5);
	
	motor_p->stop(true);
	
	motor_p->test();
	
	delete motor_p;
}