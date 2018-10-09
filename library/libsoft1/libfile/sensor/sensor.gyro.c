#include "libsoft.h"
int starti2c(int fd, int dev, int reg, int len);
int systemi2c_read(int fd, int addr, u8* buf, u8 len);




int mpuclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	//printmemory(buf, len);

	short acc[3];
	short temp;
	short gyro[3];

	int j;
	u8 tmp[14];
	for(j=0;j<14;j+=2)
	{
		tmp[j] = buf[j+1];
		tmp[j+1] = buf[j];
	}

	acc[0] = *(short*)(tmp+0);
	acc[1] = *(short*)(tmp+2);
	acc[2] = *(short*)(tmp+4);
	temp = *(short*)(tmp+6);
	gyro[0] = *(short*)(tmp+8);
	gyro[1] = *(short*)(tmp+10);
	gyro[2] = *(short*)(tmp+12);
	say("a=(%d,%d,%d), g=(%d,%d,%d), t=%d\n",
		acc[0],acc[1],acc[2],
		gyro[0],gyro[1],gyro[2],
		temp
	);
}




static void mpuclient_thread(struct element* ele)
{
	int fd,addr;
	u8 buf[256];
	struct relation* rel;
	struct object* obj;

	rel = ele->irel0;
	if(0 == rel)return;

	obj = (void*)(rel->srcchip);
	if(0 == obj)return;

	fd = obj->selffd;
	addr = 0x0068003b;
	while(1)
	{
		systemi2c_read(fd, addr, buf, 14);
		mpuclient_write(ele, 0, obj, 0, buf, 14);
	}
}




int mpuclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	struct object* obj = systemcreate(_i2c_, url);
	if(0 == obj)return 0;

	//setup mpu9250

	//forever read
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	threadcreate(mpuclient_thread, ele);
	return 1;
}
