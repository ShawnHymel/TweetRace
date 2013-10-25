
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "spi_mover.h"

int main(void)
{
	uint8_t outbuf[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
	uint8_t inbuf[10];

	spi_mover * mover_p = NULL;

	std::cout << "starting" << std::endl;
	
	mover_p = new spi_mover("/dev/spidev0.0");
	
	if( ! mover_p->transfer(10, outbuf, inbuf))
	{
		std::cout << "Transfer failed!" << std::endl;
	}
	else
	{	
		for(int i = 0; i < 10; i++)
		{
			if( outbuf[i] == inbuf[i])
			{
				std::cout << "Buffer match at idx: " << i << std::endl;
			}
			else
			{
				std::cout << "Buffer miscompare: " << outbuf[i] << " vs. " << inbuf[i] << std::endl;
			}
		}
	}
		
		
	delete mover_p;
	
	return 0;
}