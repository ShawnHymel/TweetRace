
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#include "spi_mover.h"

spi_mover::spi_mover(const char* path)
{
	uint8_t mode = SPI_MODE_0; // encodes phase&polarity
	uint8_t bits = 8;
	uint32_t speed = 500000;
	m_delay = 0;

	int status = -1;
	
	m_fd = open(path, O_RDWR);
	printf("got fd: 0x%x\r\n", m_fd);
	if (m_fd < 0)
		perror("can't open SPI device");

	/*
	 * spi mode
	 */
	status = ioctl(m_fd, SPI_IOC_WR_MODE, &mode);
	if (status == -1)
		perror("can't set spi mode");

	status = ioctl(m_fd, SPI_IOC_RD_MODE, &m_mode);
	if (status == -1)
		perror("can't get spi mode");

	/*
	 * bits per word
	 */
	status = ioctl(m_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (status == -1)
		perror("can't set bits per word");

	status = ioctl(m_fd, SPI_IOC_RD_BITS_PER_WORD, &m_bits);
	if (status == -1)
		perror("can't get bits per word");

	/*
	 * max speed hz
	 */
	status = ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (status == -1)
		perror("can't set max speed hz");

	status = ioctl(m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &m_speed);
	if (status == -1)
		perror("can't get max speed hz");

	printf("spi mode: %d\n", m_mode);
	printf("bits per word: %d\n", m_bits);
	printf("max speed: %d Hz (%d KHz)\n", m_speed, m_speed/1000);

	
}


spi_mover::~spi_mover()
{
	close(m_fd);
}

bool spi_mover::set_mode(bool idles_hi,
						bool sample_on_rising)
{
	uint8_t mode;
	uint8_t mode_check;
	int status = -1;
 
	if(idles_hi)
	{
		if(sample_on_rising)
		{
			mode = SPI_MODE_3;
		}
		else
		{
			mode = SPI_MODE_2; 
		}
	}
	else // idles lo
	{
		if(sample_on_rising)
		{
			mode = SPI_MODE_0;
		}
		else
		{
			mode = SPI_MODE_1; 
		}
	}
	
	status = ioctl(m_fd, SPI_IOC_WR_MODE, &mode);
	if (status == -1)
	{
		perror("can't set spi mode");
		return false;
 	}
	
	status = ioctl(m_fd, SPI_IOC_RD_MODE, &mode_check);
	if (status == -1)
 	{
		perror("can't get spi mode");
		return false;
 	}
 	
	if(mode != 	mode_check)
 	{
		perror("new spi mode doesn't match requested...");
		return false;
	}
 	
	printf("new mode 0x%x \r\n", mode_check);
	return true;
}

		
bool spi_mover::transfer(uint32_t num,
						uint8_t * out_buf,
						uint8_t * in_buf)
{
	int ret;

#if 0	
	// some argument instrumentation....
	printf("Transfer len: x%x/r/n", num);
	for(int i = 0; i < num; i++)
	{
		printf("out_buf[%d] = 0x%x\r\n", i, out_buf[i]);
	}
	for(int i = 0; i < num; i++)
	{
		printf("in_buf[%d] = 0x%x\r\n", i, in_buf[i]);
	}
#endif	
	
	struct spi_ioc_transfer tr;

	tr.tx_buf = (unsigned long)out_buf;
	tr.rx_buf = (unsigned long)in_buf;
	tr.len = num;
	tr.delay_usecs = m_delay;
	tr.speed_hz = m_speed;
	tr.bits_per_word = m_bits;
	tr.cs_change = 0;
	tr.pad = 0;

#if 0
	printf("tr.tx_buf = 0x%x\r\n", tr.tx_buf);
	printf("tr.rx_buf = 0x%x\r\n", tr.rx_buf);
	printf("tr.len = 0x%x\r\n", tr.len);
	printf("tr.delay_usecs = 0x%x\r\n", tr.delay_usecs);
	printf("tr.speed_hz = 0x%x\r\n", tr.speed_hz);
	printf("tr.bits_per_word = 0x%x\r\n", tr.bits_per_word);
	printf("tr.cs_change = 0x%x\r\n", tr.cs_change);
	printf("tr.pad = 0x%x\r\n", tr.pad);

	uint8_t * ptr = (uint8_t*)&tr;
	for(int i = 0; i < sizeof(spi_ioc_transfer); i++)
	{
		printf("0x%02x \r\n", ptr[i]);
	}
#endif
	
	ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0)
	{
		perror("can't send spi message");
		return false;
	}
	
	return true;

}
