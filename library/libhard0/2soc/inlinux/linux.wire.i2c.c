#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>		//if you have problem
#include<linux/i2c-dev.h>
#include "libhard.h"




struct privdata{
	int fd;
};




int i2c_free(_obj* obj)
{
	return 0;
}
_obj* i2c_alloc(u64 type, u8* name)
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
	logtoall("i2c: %s\n", buf);

	int fd = open((void*)buf, O_RDWR);

	_obj* obj = device_alloc_fromfd(_i2c_, fd);
	struct privdata* priv = (void*)obj->priv_256b;
	priv->fd = fd;

	return obj;
}




int i2c_create(_obj* obj,void* foot, int argc, char** argv)
{
	return 0;
}
int i2c_delete(_obj* obj)
{
	if(0 == obj)return 0;

	struct privdata* priv = (void*)obj->priv_256b;
	if(0 == priv)return 0;

	int fd = priv->fd;
	if(fd<=0)return 0;

	return close(fd);
}
int i2c_read(_obj* obj,void* foot, u8* arg,int addr, u8* buf,int len)
{
	//logtoall("@%s:%p,%x,%p,%x\n", __func__, arg, addr, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;
	int fd = priv->fd;

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
int i2c_write(_obj* obj,void* foot, u8* arg,int addr, u8* buf,int len)
{
	//logtoall("@%s:%p,%x,%p,%x\n", __func__, arg, addr, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;
	int fd = priv->fd;

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