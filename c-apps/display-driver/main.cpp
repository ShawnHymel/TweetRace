
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
		uint32_t templen;
	
		std::cout << "sending: " << c << std::endl;
	
		sprintf(tempbuf, "test%c", c);
		templen = strlen(tempbuf);
		disp_p->update_string(0, tempbuf, templen);

		sprintf(tempbuf, "%c %c %c", c, c, c);
		templen = strlen(tempbuf);
		disp_p->update_string(1, tempbuf, templen);
		
		sprintf(tempbuf, "3%c3%c3", c, c);
		templen = strlen(tempbuf);
		disp_p->update_string(2, tempbuf, templen);

		sprintf(tempbuf, "4%c4%c4", c, c);
		templen = strlen(tempbuf);
		disp_p->update_string(3, tempbuf, templen);

		sprintf(tempbuf, "5Really long %c", c);
		templen = strlen(tempbuf);
		disp_p->update_string(4, tempbuf, templen);
	
		
		sleep(20);
		
	}
	
	delete disp_p;
}