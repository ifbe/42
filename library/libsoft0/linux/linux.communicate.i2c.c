#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
static int fp=-1;
static unsigned char buf[16];




void writei2c()
{
}
void readi2c(unsigned char* dest,unsigned char src)
{
	//write address
	int ret=write(fp,&src,1);
	usleep(100);
	if(ret!=1)
	{
		printf("(error write)%x:%x\n",src,ret);
		return;
	}

	//read data
	ret=read(fp,dest,1);
	usleep(1000);
	if(ret!=1)
	{
		printf("(error read)%x:%x\n",src,ret);
		return;
	}
}
void switchi2c(char* what)	//	i2c://0x1.0x62
{
}
void listi2c(char* towhere)	//	enumerate all i2c host and device
{
}




void starti2c()
{
	//open
	fp = open("/dev/i2c-1",O_RDWR);
	if(fp<0)
	{
		printf("error open\n");
		return;
	}

	//ioctl
	ret=ioctl(fp,I2C_SLAVE,0x62);
	if(ret < 0)
	{
		printf("error ioctl\n");
		return;
	}
}
void stopi2c()
{
	if(fp != -1)close(fp);
}
void initi2c()
{
}
void killi2c()
{
}
