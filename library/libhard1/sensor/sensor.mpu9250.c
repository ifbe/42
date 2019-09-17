#include "libhard.h"
//
#define PI 3.1415926535897932384626433832795
//0=250dps, 1=500dps, 2=1000dps, 3=2000dps
#define gyr_cfg 2
#define gyr_max 1000
//0=2g, 1=4g, 2=8g, 3=16g
#define acc_cfg 0
#define acc_max 2*9.8
//
int sleep_us();




int ak8963_parse(u8* buf, float* vec)
{
	short* t = (short*)buf;
	//say("m=(%d,%d,%d)\n", t[0], t[1], t[2]);
	vec[1] = t[0] * 4192.0 / 32760.0;
	vec[0] = t[1] * 4192.0 / 32760.0;
	vec[2] = -t[2] * 4192.0 / 32760.0;
	return 0;
}
int mpu9250_parse(u8* buf, float* vec)
{
	short gyr[3];
	short acc[3];
	short temp;

	acc[0] = (buf[0]<<8) | buf[1];
	if(acc[0] > 32767)acc[0] -= 0x10000;

	acc[1] = (buf[2]<<8) | buf[3];
	if(acc[1] > 32767)acc[1] -= 0x10000;

	acc[2] = (buf[4]<<8) | buf[5];
	if(acc[2] > 32767)acc[2] -= 0x10000;

	temp = (buf[6]<<8) | buf[7];
	if(temp > 32767)temp -= 0x10000;

	gyr[0] = (buf[8]<<8) | buf[9];
	if(gyr[0] > 32767)gyr[0] -= 0x10000;

	gyr[1] = (buf[10]<<8) | buf[11];
	if(gyr[1] > 32767)gyr[1] -= 0x10000;

	gyr[2] = (buf[12]<<8) | buf[13];
	if(gyr[2] > 32767)gyr[2] -= 0x10000;

	vec[0] = gyr[0] * gyr_max / 32768.0 * PI / 180.0;
	vec[1] = gyr[1] * gyr_max / 32768.0 * PI / 180.0;
	vec[2] = gyr[2] * gyr_max / 32768.0 * PI / 180.0;
	vec[3] = acc[0] * acc_max / 32768.0;
	vec[4] = acc[1] * acc_max / 32768.0;
	vec[5] = acc[2] * acc_max / 32768.0;
/*
	say("a=(%d,%d,%d), g=(%d,%d,%d), t=%d\n",
		acc[0],acc[1],acc[2],
		gyr[0],gyr[1],gyr[2],
		temp
	);
*/
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
int mpu9250_spiread(struct driver* dri, float* vec)
{
	u8 tmp[0x20];

	//write
	tmp[0] = 0xc | 0x80;
	relationwrite(dri, _spi_, 0, 0x25, tmp, 1);
	tmp[0] = 0x3;
	relationwrite(dri, _spi_, 0, 0x26, tmp, 1);
	tmp[0] = 0x87;
	relationwrite(dri, _spi_, 0, 0x27, tmp, 1);

	//read
	relationread(dri, _spi_, 0, 0x3b|0x80, tmp, 20);

	//parse
	mpu9250_parse(tmp, vec);
	ak8963_parse(tmp+14, &vec[6]);
	return 9;
}




int mpu9250_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int mpu9250_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int ret;
	float tmp[10];
	struct driver* it;
	say("@mpu9250_write: %.4s\n", &self->flag);

	it = (void*)(self->chip);
	if(0 == it)return 0;

	if(_clk_ == self->flag){
		ret = mpu9250_spiread(it, tmp);
		relationwrite(it, _dst_, 0, 0, tmp, ret);
	}
	return 0;
}
int mpu9250_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mpu9250_start(struct halfrel* self, struct halfrel* peer)
{
	struct driver* it;
	say("@mpu9250_start: %.4s\n", &self->flag);

	it = (void*)(self->chip);
	if(0 == it)return 0;

	if(_spi_ == self->flag){
		//mpu9250_spistart(it, tmp);
	}
	return 0;
}




int mpu9250_create(struct driver* ele, u8* url)
{
	say("@mpu9250_create\n");
	return 1;
}
int mpu9250_delete(struct driver* ele)
{
	return 0;
}
