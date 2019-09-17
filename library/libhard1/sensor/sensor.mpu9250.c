#include "libsoft.h"
int sleep_us();




int ak8963_parse(u8* buf, int len)
{
	short* t = (short*)buf;
	say("m=(%d,%d,%d)\n", t[0], t[1], t[2]);
	return 0;
}
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



/*
int mpu9250_start_i2c(int fd)
{
	say("@mpu9250_start_i2c\n");
	return 0;
}




int spi8963_read(int fd, u8 reg, u8* buf, int len)
{
	u8 tmp[16];

	//MPUREG_I2C_SLV0_ADDR
	tmp[0] = 0xc|0x80;
	systemspi_write(fd, 0x25, tmp, 1);

	//MPUREG_I2C_SLV0_REG
	tmp[0] = reg;
	systemspi_write(fd, 0x26, tmp, 1);

	//MPUREG_I2C_SLV0_CTRL
	tmp[0] = len|0x80;
	systemspi_write(fd, 0x27, tmp, 1);

	//10ms
	sleep_us(1000*10);

	//read out
	systemspi_read(fd, 0x49 | 0x80, buf, len);
	return 0;
}
int spi8963_write(int fd, u8 reg, u8* buf, int len)
{
	u8 tmp[16];

	//MPUREG_I2C_SLV0_ADDR
	tmp[0] = 0xc;
	systemspi_write(fd, 0x25, tmp, 1);

	//MPUREG_I2C_SLV0_REG
	tmp[0] = reg;
	systemspi_write(fd, 0x26, tmp, 1);

	//MPUREG_I2C_SLV0_D0
	tmp[0] = buf[0];
	systemspi_write(fd, 0x63, tmp, 1);

	//MPUREG_I2C_SLV0_CTRL
	tmp[0] = 0x81;
	systemspi_write(fd, 0x27, tmp, 1);

	return 0;
}
int mpu9250_start_spi(int fd)
{
	u8 tmp[0x80];
	say("@mpu9250_start_spi\n");

//check stage
	//check 9250.whoami
	systemspi_read(fd, 0x75|0x80, tmp, 1);
	if(0x71 != tmp[0]){
		say("mpu9250.whoami != 0x71\n");
		return 0;
	}

	//MPUREG_USER_CTRL, MPUREG_I2C_MST_CTRL
	tmp[0] = 0x20;
	systemspi_write(fd, 0x6a, tmp, 1);
	tmp[0] = 0xd;
	systemspi_write(fd, 0x24, tmp, 1);

	//check 8963.whoami
	spi8963_read(fd, 0, tmp, 1);
	if(0x48 != tmp[0]){
		say("ak8963.whoami != 0x48\n");
		return 0;
	}


//init mpu9250


//init ak8963
	tmp[0] = 1;
	spi8963_write(fd, 0xb, tmp, 1);

	tmp[0] = 0x16;
	spi8963_write(fd, 0xa, tmp, 1);

	return 0;
}
int mpu9250_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int fd;
	u8 tmp[0x80];
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

			switch(obj->type){
				case _i2c_:{
					systemi2c_read(fd, (0x68<<16) | 0x3b, tmp, 14);
					mpu9250_parse(tmp, 14);

					systemi2c_read(fd, (0x0c<<16) | 0x03, tmp, 7);
					ak8963_parse(tmp, 6);
					break;
				}
				case _spi_:{
					tmp[0] = 0xc | 0x80;
					systemspi_write(fd, 0x25, tmp, 1);
					tmp[0] = 0x3;
					systemspi_write(fd, 0x26, tmp, 1);
					tmp[0] = 0x87;
					systemspi_write(fd, 0x27, tmp, 1);

					systemspi_read(fd, 0x3b|0x80, tmp, 20);
					mpu9250_parse(tmp, 14);
					ak8963_parse(tmp+14, 6);
					break;
				}
			}//switch
		}//if(fd)

		rel = samedstnextsrc(rel);
	}//while

	//systemread(&rel->srcchip, &rel->dstchip, tmp, 14);

	return 0;
}
*/
int mpu9250_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int mpu9250_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@mpu9250_write: %.4s\n", &self->flag);
	return 0;
}
int mpu9250_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mpu9250_start(struct halfrel* self, struct halfrel* peer)
{
	u64 foot = self->flag;
	say("@mpu9250_start: %.8s\n", &foot);
	return 0;
}




int mpu9250_create(struct element* ele, u8* url)
{
	say("@mpu9250_create\n");
	return 1;
}
int mpu9250_delete(struct element* ele)
{
	return 0;
}
