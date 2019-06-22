#include "libsoft.h"
int systemi2c_read(int fd, int addr, u8* buf, u8 len);




int mpu9250_parse(u8* buf, int len)
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
	return 0;
}
int ak8963_parse(u8* buf, int len)
{
	short* t = (short*)buf;
	say("m=(%d,%d,%d)\n", t[0], t[1], t[2]);
	return 0;
}




int mpu9250_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int fd;
	u8 tmp[0x10];
	struct element* ele;
	struct relation* rel;
	struct object* obj;
	say("@mpu9250_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->irel0;
	while(1){
		if(0 == rel)break;

		if(_fd_ == rel->srctype){
			obj = (void*)(rel->srcchip);
			if(0 == obj)return 0;

			fd = obj->selffd;
			if(0 == fd)return 0;

			systemi2c_read(fd, (0x68<<16) | 0x3b, tmp, 14);
			mpu9250_parse(tmp, 14);

			systemi2c_read(fd, (0x0c<<16) | 0x03, tmp, 7);
			ak8963_parse(tmp, 14);
		}

		rel = samedstnextsrc(rel);
	}

	//systemread(&rel->srcchip, &rel->dstchip, tmp, 14);

	return 0;
}
int mpu9250_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	return 0;
}
int mpu9250_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mpu9250_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mpu9250_create(struct element* ele, u8* url)
{
	say("@mpu9250_create\n");
	return 1;
}
