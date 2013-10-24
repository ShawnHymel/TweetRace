#ifndef _SPI_MOVER_H__
#define _SPI_MOVER_H__

//#include <unistd.h>
#include <stdint.h>

class spi_mover
{
	public:
		spi_mover(const char* path);
		~spi_mover();
		
		bool transfer(uint32_t num,
					uint8_t * out_buf,
					uint8_t * in_buf);
				 
	private:
	
		uint32_t fd;
		
		uint8_t mode;
		uint8_t bits;
		uint32_t speed;
		uint16_t delay;
};

#endif // outer keepout
