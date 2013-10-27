
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "dspin_driver.h"

int main(void)
{
	dspin_driver * motor_p;

	motor_p = new dspin_driver();

	motor_p->test();
	
	delete motor_p;
}