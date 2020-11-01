#include "libhard.h"
//0=250dps, 1=500dps, 2=1000dps, 3=2000dps
#define gyr_cfg 2
#define gyr_max 1000
//0=2g, 1=4g, 2=8g, 3=16g
#define acc_cfg 0
#define acc_max 2*9.8
//
int sleep_us();




enum mpu9250{
	CONFIG	= 0x1A,
	GYRO_CONFIG   = 0x1B,
	ACCEL_CONFIG  = 0x1C,
	ACCEL_CONFIG2 = 0x1D,

	FIFO_EN       = 0x23,

	INT_STATUS    = 0x3A,

	INT_ENABLE    = 0x38,

	TEMP_OUT_H    = 0x41,
	TEMP_OUT_L    = 0x42,

	USER_CTRL     = 0x6A,
	PWR_MGMT_1    = 0x6B,

	FIFO_COUNTH   = 0x72,
	FIFO_COUNTL   = 0x73,
	FIFO_R_W      = 0x74,
	WHO_AM_I      = 0x75,
};




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




int spi9250_slv8963_read(struct item* dri, u8 reg, u8* buf, int len)
{
	u8 tmp[16];

	//MPUREG_I2C_SLV0_ADDR
	tmp[0] = 0xc|0x80;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x25, tmp,1);

	//MPUREG_I2C_SLV0_REG
	tmp[0] = reg;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x26, tmp,1);

	//MPUREG_I2C_SLV0_CTRL
	tmp[0] = len|0x80;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x27, tmp,1);

	//10ms
	sleep_us(1000*10);

	//read out
	take_data_from_peer(dri,_spi_, 0,0, 0,0x49|0x80, buf,len);
	return 0;
}
int spi9250_slv8963_write(struct item* dri, u8 reg, u8* buf, int len)
{
	u8 tmp[16];

	//MPUREG_I2C_SLV0_ADDR
	tmp[0] = 0xc;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x25, tmp,1);

	//MPUREG_I2C_SLV0_REG
	tmp[0] = reg;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x26, tmp,1);

	//MPUREG_I2C_SLV0_D0
	tmp[0] = buf[0];
	give_data_into_peer(dri,_spi_, 0,0, 0,0x63, tmp,1);

	//MPUREG_I2C_SLV0_CTRL
	tmp[0] = 0x81;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x27, tmp,1);

	return 0;
}
int spi9250_slv8963_stop(struct item* dri)
{
	return 0;
}
int spi9250_slv8963_start(struct item* dri)
{
	u8 tmp[0x20];

//check 8963.whoami
	spi9250_slv8963_read(dri, 0, tmp, 1);
	if(0x48 != tmp[0]){
		say("ak8963.whoami != 0x48\n");
		return 0;
	}

//init ak8963
	tmp[0] = 1;
	spi9250_slv8963_write(dri, 0xb, tmp, 1);
	sleep_us(1000);

	tmp[0] = 0x16;
	spi9250_slv8963_write(dri, 0xa, tmp, 1);
	sleep_us(1000);

	return 0;
}




int mpu9250_spiread(struct item* dri, float* vec)
{
	u8 tmp[0x20];

	//read 8963
	spi9250_slv8963_read(dri, 3, tmp, 7);

	//read 9250
	take_data_from_peer(dri,_spi_, 0,0, 0,0x3b|0x80, tmp,20);

	//parse
	mpu9250_parse(tmp, vec);
	ak8963_parse(tmp+14, &vec[6]);
	return 9;
}
int mpu9250_spiinit(struct item* dri)
{
	u8 tmp[0x80];

//check 9250.whoami
	take_data_from_peer(dri,_spi_, 0,0, 0,0x75|0x80, tmp,1);
	if(0x71 != tmp[0]){
		say("mpu9250.whoami != 0x71\n");
		return 0;
	}

//init mpu9250
	//reset
	tmp[0] = 0x80;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x6b, tmp,1);
	sleep_us(1000);

	//clock
	tmp[0] = 0x01;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x6b, tmp,1);
	sleep_us(1000);

	//enable
	tmp[0] = 0x00;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x6c, tmp,1);
	sleep_us(1000);

	//CONFIG
	tmp[0] = 0x03;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x1a, tmp,1);
	sleep_us(1000);

	//SMPLRT_DIV
	tmp[0] = 0x04;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x19, tmp,1);
	sleep_us(1000);

	//GYRO_CONFIG
	take_data_from_peer(dri,_spi_, 0,0, 0,0x1b|0x80, tmp,1);
	tmp[0] = (tmp[0] & 0xe7) | (gyr_cfg << 3);
	give_data_into_peer(dri,_spi_, 0,0, 0,0x1b, tmp,1);
	sleep_us(1000);

	//ACCEL_CONFIG
	take_data_from_peer(dri,_spi_, 0,0, 0,0x1c|0x80, tmp,1);
	tmp[0] = (tmp[0] & 0xe7) | (acc_cfg << 3);
	give_data_into_peer(dri,_spi_, 0,0, 0,0x1c, tmp,1);
	sleep_us(1000);

	//ACCEL_CONFIG2
	take_data_from_peer(dri,_spi_, 0,0, 0,0x1d|0x80, tmp,1);
	tmp[0] = (tmp[0] & 0xf0) | 0x3;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x1d, tmp,1);
	sleep_us(1000);

	//INT_PIN_CFG
	tmp[0] = 0x22;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x37, tmp,1);
	sleep_us(1000);

	//INT_ENABLE
	tmp[0] = 0x01;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x38, tmp,1);
	sleep_us(1000);

	//MPUREG_USER_CTRL
	tmp[0] = 0x20;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x6a, tmp,1);
	sleep_us(1000);

	//MPUREG_I2C_MST_CTRL
	tmp[0] = 0xd;
	give_data_into_peer(dri,_spi_, 0,0, 0,0x24, tmp,1);
	sleep_us(1000*10);

	return 0;
}




int mpu9250_i2cinit(struct item* dri)
{
	u8 reg[0x20];

	//PWR_MGMT_1	reset
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068006b, 0,0x80);
	sleep_us(1000);

	//PWR_MGMT_1	clock
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068006b, 0,0x01);
	sleep_us(1000);

	//PWR_MGMT_2	enable
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068006c, 0,0x00);
	sleep_us(1000);

	//CONFIG
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068001a, 0,0x03);
	sleep_us(1000);

	//SMPLRT_DIV
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x00680019, 0,0x04);
	sleep_us(1000);

	//GYRO_CONFIG
	take_data_from_peer(dri,_i2c_, 0,0, 0,0x0068001b, reg,1);
	reg[0] &= 0xe7;
	reg[0] |= (gyr_cfg<<3);
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068001b, reg,1);
	sleep_us(1000);

	//ACCEL_CONFIG
	take_data_from_peer(dri,_i2c_, 0,0, 0,0x0068001c, reg,1);
	reg[0] &= 0xe7;
	reg[0] |= (acc_cfg<<3);
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068001c, reg,1);
	sleep_us(1000);

	//ACCEL_CONFIG2
	take_data_from_peer(dri,_i2c_, 0,0, 0,0x0068001d, reg,1);
	reg[0] &= 0xf0;
	reg[0] |= 0x3;
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x0068001d, reg,1);
	sleep_us(1000);

	//INT_PIN_CFG
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x00680037, 0,0x22);
	sleep_us(1000);

	//INT_ENABLE
	give_data_into_peer(dri,_i2c_, 0,0, 0,0x00680038, 0,0x01);
	sleep_us(1000);
	return 0;
}
int mpu9250_i2cread(struct item* dri, float* vec)
{
	u8 tmp[32];

	take_data_from_peer(dri,_i2c_, 0,0, 0,0x68003b, tmp,14);
	mpu9250_parse(tmp, vec);

	take_data_from_peer(dri,_i2c_, 0,0, 0,0x0c0003, tmp,7);
	ak8963_parse(tmp, &vec[6]);

	return 0;
}




int mpu9250_read( struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int mpu9250_write(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	int ret;
	float tmp[10];
	say("@mpu9250_write:%p,%p\n", dri,foot);

	if(_clk_ == stack[sp-1].flag){
		switch(dri->hfmt){
		case _i2c_:ret = mpu9250_i2cread(dri, tmp);break;
		case _spi_:ret = mpu9250_spiread(dri, tmp);break;
		default:return 0;
		}

		give_data_into_peer(dri,_dst_, stack,sp, 0,0, tmp,ret);
	}
	return 0;
}
int mpu9250_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mpu9250_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct item* drv;
	say("@mpu9250_linkup: %.4s\n", &self->flag);

	drv = self->pchip;
	if(0 == drv)return 0;

	switch(self->flag){
	case _i2c_:{
		mpu9250_i2cinit(drv);
		drv->hfmt = _i2c_;
		break;
	}
	case _spi_:{
		mpu9250_spiinit(drv);
		spi9250_slv8963_start(drv);
		drv->hfmt = _spi_;
		break;
	}
	}
	return 0;
}




int mpu9250_create(struct item* ele, u8* url)
{
	say("@mpu9250_create\n");
	return 1;
}
int mpu9250_delete(struct item* ele)
{
	return 0;
}
