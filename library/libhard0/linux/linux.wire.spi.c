#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>		//Needed for SPI port
#include <unistd.h>		//Needed for SPI port
#include <sys/ioctl.h>		//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#define u8 unsigned char
#define u64 unsigned long long
void say(char*,...);




//SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, idle low, data on rising edge, output on falling edge
//SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, idle low, data on falling edge, output on rising edge
//SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, idle high, data on falling edge, output on rising edge
//SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, idle high, data on rising, edge output on falling edge
u8 spi_mode = SPI_MODE_3;
u8 spi_bitsPerWord = 8;
unsigned int spi_speed = 1000000;




int spi_read_byte(int fd, u8 reg)
{
        int ret;
        u8 rx_buffer[2];
        u8 tx_buffer[2];
	tx_buffer[0] = reg;

        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx_buffer,
                .rx_buf = (unsigned long)rx_buffer,
                .len = 2,
                //.delay_usecs = delay,
                .speed_hz = spi_speed,
                .bits_per_word = spi_bitsPerWord,
        };

        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1) {
                perror("can't send spi message");
		return -1;
        }

        return rx_buffer[1];
}
int spi_write_byte(int fd, u8 reg, u8 data)
{
        int ret;
        u8 rx_buffer[2];
        u8 tx_buffer[2];
	tx_buffer[0] = reg;
	tx_buffer[1] = data;

        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx_buffer,
                .rx_buf = (unsigned long)rx_buffer,
                .len = 2,
                //.delay_usecs = delay,
                .speed_hz = spi_speed,
                .bits_per_word = spi_bitsPerWord,
        };

        ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1) {
                perror("can't send spi message");
		return -1;
        }

        return rx_buffer[1];
}
int spi_read(int fd, int reg, u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++){
		ret = spi_read_byte(fd, reg+j);
		if(ret < 0)break;

		buf[j] = ret;
	}
	return j;
}
int spi_write(int fd, int reg, u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++){
		ret = spi_write_byte(fd, reg+j, buf[0]);
		if(ret < 0)break;

		buf[j] = ret;
	}
	return j;
}
int spi_stop(int fd, int dev, int reg, int len)
{
	return 0;
}
int spi_start(int fd, int dev, int reg, int len)
{
	return 0;
}
int spi_delete(int fd)
{
	int ret = close(fd);
	if(ret < 0)perror("Error - Could not close SPI device");
	return ret;
}
int spi_create(char* buf, int len)
{
	int j,ret;
	u8 str[128];
	for(j=0;j<128;j++){
		if(buf[j] > 0x20)str[j] = buf[j];
		else{str[j] = 0;break;}
	}

	int fd = open(str, O_RDWR);
	if (fd < 0){
		perror("Error - Could not open SPI device");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
	if(ret < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		return -2;
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
	if(ret < 0)
	{
		perror("Could not set SPIMode (RD)...ioctl fail");
		return -3;
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
	if(ret < 0)
	{
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		return -4;
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
	if(ret < 0)
	{
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		return -5;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(ret < 0)
	{
		perror("Could not set SPI speed (WR)...ioctl fail");
		return -6;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(ret < 0)
	{
		perror("Could not set SPI speed (RD)...ioctl fail");
		return -7;
	}
	return fd;
}
