#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>		//if you have problem
#include<linux/i2c-dev.h>
void printmemory(void*, int);
void say(void*, ...);




int i2c_read(int fd, int addr, u8* buf, u8 len)
{
	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data packets;
	u8 dev = addr>>16;
	u8 reg = addr&0xffff;

	//out
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = 1;
	messages[0].buf   = &reg;

	//in
	messages[1].addr  = dev;
	messages[1].flags = I2C_M_RD;
	messages[1].len   = len;
	messages[1].buf   = buf;

	//send
	packets.msgs      = messages;
	packets.nmsgs     = 2; 
	if(ioctl(fd, I2C_RDWR, &packets) < 0)
	{
		perror("Unable to send data");
		return -1;
	}

	return 1;
}
int i2c_write(int fd, int addr, u8* buf, u8 len)
{
	int j;
	u8 out[512];
	struct i2c_msg messages[1];
	struct i2c_rdwr_ioctl_data packets;
	u8 dev = addr>>16;
	u8 reg = addr&0xffff;

	//which,what0,what1,what2......
	out[0] = reg;
	for(j=0;j<len;j++)out[j+1] = buf[j];

	//message
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = len+1;
	messages[0].buf   = out;

	//transfer
	packets.msgs  = messages;
	packets.nmsgs = 1;
	j = ioctl(fd, I2C_RDWR, &packets);
	if(j < 0)
	{
		//perror("Unable to send data");
		return -1;
	}

	return 1;
}
int i2c_stop(int fd, int dev, int reg, int len)
{
	return 0;
}
int i2c_start(int fd, int dev, int reg, int len)
{
	return 0;
}




int i2c_search()
{
	return 0;
}
int i2c_modity()
{
	return 0;
}
int i2c_delete(int fd)
{
	return close(fd);
}
int i2c_create(u8* name, int flag, int argc, u8** argv)
{
	int j;
	u8 buf[256];

	for(j=0;j<128;j++)
	{
		if(name[j] <= 0x20)
		{
			buf[j] = 0;
			break;
		}
		else
		{
			buf[j] = name[j];
		}
	}

	say("i2c: %s\n", buf);
	return open((void*)buf, O_RDWR);
}
