
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "display_driver.h"

int main(void)
{
	display_driver * disp_p;

	disp_p = new display_driver();
	
	for(char c = 'A'; c <= 'Z'; c++)
	{
		char tempbuf[10];
	
		std::cout << "sending: " << c << std::endl;
	
		sprintf(tempbuf, "%c", c);
	
		disp_p->update_string(0, tempbuf);
		usleep(500000);
		
	}
	
	delete disp_p;
}