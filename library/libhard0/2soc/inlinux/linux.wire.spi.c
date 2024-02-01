#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>		//Needed for SPI port
#include <unistd.h>		//Needed for SPI port
#include <sys/ioctl.h>		//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include "libhard.h"




//SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, idle low, data on rising edge, output on falling edge
//SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, idle low, data on falling edge, output on rising edge
//SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, idle high, data on falling edge, output on rising edge
//SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, idle high, data on rising, edge output on falling edge
struct privdata{
	int fd;
	unsigned int spi_speed;
	u8 spi_mode;
	u8 spi_bitsPerWord;
};




int spi_read_byte(struct privdata* priv, u8 reg)
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
		.speed_hz = priv->spi_speed,
		.bits_per_word = priv->spi_bitsPerWord,
	};

	ret = ioctl(priv->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("can't send spi message");
		return -1;
	}

	return rx_buffer[1];
}
int spi_write_byte(struct privdata* priv, u8 reg, u8 data)
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
		.speed_hz = priv->spi_speed,
		.bits_per_word = priv->spi_bitsPerWord,
	};

	ret = ioctl(priv->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("can't send spi message");
		return -1;
	}

	return rx_buffer[1];
}




int spi_read(_obj* obj,void* foot, u8* arg,int reg, u8* buf,int len)
{
	logtoall("@%s:%p,%x,%p,%x\n", __func__, arg, reg, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;
	int fd = priv->fd;

	int j,ret;
	for(j=0;j<len;j++){
		ret = spi_read_byte(priv, reg+j);
		if(ret < 0)break;

		buf[j] = ret;
	}
	return j;
}
int spi_write(_obj* obj,void* foot, u8* arg,int reg, u8* buf,int len)
{
	logtoall("@%s:%p,%x,%p,%x\n", __func__, arg, reg, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;
	int fd = priv->fd;

	int j,ret;
	for(j=0;j<len;j++){
		ret = spi_write_byte(priv, reg+j, buf[0]);
		if(ret < 0)break;

		buf[j] = ret;
	}
	return j;
}
int spi_delete(_obj* obj)
{
	if(0 == obj)return 0;

	struct privdata* priv = (void*)obj->priv_256b;
	if(0 == priv)return 0;

	int fd = priv->fd;
	if(fd<=0)return 0;

	int ret = close(fd);
	if(ret < 0)perror("Error - Could not close SPI device");
	return ret;
}
int spi_create(_obj* obj,void* foot, int argc, char** argv)
{
	return 0;
}

int spi_free(_obj* obj)
{
	return 0;
}
_obj* spi_alloc(u64 type, u8* buf)
{
	unsigned int spi_speed = 1000000;
	u8 spi_mode = SPI_MODE_3;
	u8 spi_bitsPerWord = 8;

	int j;
	char str[128];
	for(j=0;j<128;j++){
		if(buf[j] > 0x20)str[j] = buf[j];
		else{str[j] = 0;break;}
	}

	int fd = open(str, O_RDWR);
	if (fd < 0){
		perror("Error - Could not open SPI device");
		return 0;
	}

	int ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
	if(ret < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		return 0;
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
	if(ret < 0)
	{
		perror("Could not set SPIMode (RD)...ioctl fail");
		return 0;
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
	if(ret < 0)
	{
		perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
		return 0;
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
	if(ret < 0)
	{
		perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
		return 0;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if(ret < 0)
	{
		perror("Could not set SPI speed (WR)...ioctl fail");
		return 0;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if(ret < 0)
	{
		perror("Could not set SPI speed (RD)...ioctl fail");
		return 0;
	}

	_obj* obj = device_alloc_fromfd(_spi_, fd);
	struct privdata* priv = (void*)obj->priv_256b;
	priv->fd = fd;
	priv->spi_speed = spi_speed;
	priv->spi_mode = spi_mode;
	priv->spi_bitsPerWord = spi_bitsPerWord;
	return obj;
}