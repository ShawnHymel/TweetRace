
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
	uint16_t delay = 0;

	int status = -1;
	
	fd = open(path, O_RDWR);
	printf("got fd: 0x%x\r\n", fd);
	if (fd < 0)
		perror("can't open SPI device");

	/*
	 * spi mode
	 */
	status = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (status == -1)
		perror("can't set spi mode");

	status = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (status == -1)
		perror("can't get spi mode");

	/*
	 * bits per word
	 */
	status = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (status == -1)
		perror("can't set bits per word");

	status = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (status == -1)
		perror("can't get bits per word");

	/*
	 * max speed hz
	 */
	status = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (status == -1)
		perror("can't set max speed hz");

	status = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (status == -1)
		perror("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	
}


spi_mover::~spi_mover()
{
	close(fd);
}
		
bool spi_mover::transfer(uint32_t num,
						uint8_t * out_buf,
						uint8_t * in_buf)
{
	int ret;
#if 0
	uint8_t tx[] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
#endif	

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

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)out_buf;
	tr.rx_buf = (unsigned long)in_buf;
	tr.len = num;
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = bits;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0)
	{
		perror("can't send spi message");
		return false;
	}
	
#if 0	
	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	puts("");
#endif	

	return true;

}
