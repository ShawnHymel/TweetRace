#ifndef _SPI_MOVER_H__
#define _SPI_MOVER_H__

//#include <unistd.h>
#include <stdint.h>

class spi_mover
{
	public:
		spi_mover(const char* path);
		~spi_mover();
		
		bool set_mode(bool idles_hi,
					bool sample_on_rising);
		
		
		bool transfer(uint32_t num,
					uint8_t * out_buf,
					uint8_t * in_buf);
				 
	private:
	
		uint32_t m_fd;
		
		uint8_t m_mode;
		uint8_t m_bits;
		uint32_t m_speed;
		uint16_t m_delay;
};

#endif // outer keepout
