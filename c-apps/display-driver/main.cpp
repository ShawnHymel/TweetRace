
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
		char tempbuf[80];
	
		std::cout << "sending: " << c << std::endl;
	
		sprintf(tempbuf, "test%c", c);
		disp_p->update_string(0, tempbuf);

		sprintf(tempbuf, "%c %c %c", c, c, c);
		disp_p->update_string(1, tempbuf);
		
		sprintf(tempbuf, "3%c3%c3", c, c);
		disp_p->update_string(2, tempbuf);

		sprintf(tempbuf, "4%c4%c4", c, c);
		disp_p->update_string(3, tempbuf);

		sprintf(tempbuf, "5Really long %c", c);
		disp_p->update_string(4, tempbuf);
		
		
		sleep(20);
		
	}
	
	delete disp_p;
}