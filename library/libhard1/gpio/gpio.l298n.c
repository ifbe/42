#include "libsoft.h"
#define _chip_ hex32('c','h','i','p')
#define _pin_value_ hex32('p','v', 0, 0)
#define _pin_mode_value_ hex32('p','m','v', 0)
int sleep_us(int);




/*
3v3             ----________----5v
g02-sda         ----________----5v
g03-scl         ----________----0v
g04-motor-lf-n  ----________----g14-motor-rf-p
0v              ----________----g15-motor-rf-n
g17-motor-rf-en ----________----g18-i2s-clk
g27-motor-lf-p  ----________----0v
g22-motor-lf-en ----________----g23-sensehat-imu-int1
3v3             ----____r___----g24-sensehat-imu-int2
g10-spi0-mosi   ----____p___----0v
g09-spi0-miso   ----____i___----g25-sensehat-atmel-chip-reset
g11-spi0-sclk   ----____x___--- g08-sensehat-atmel-chip-select(spi0-ce0)
0v              ----________----g07-spi0-ce1
g00-used-3.3v   ----________----g01-motor-rn-n
g05-motor-ln-n  ----________----0v
g06-motor-ln-p  ----________----g12-motor-rn-en
g13-motor-ln-en ----________----0v
g19-i2s-fs      ----________----g16-used-sdmode
g26-motor-rn-p  ----________----g20-i2s-din
0v              ----________----g21-i2s-dout
*/
static char pintable[4][3] = {
	{22, 4,27},	//left,front
	{13, 5, 6},	//left,back
	{17,15,14},	//right,front
	{12, 1,26},	//right,back
};
static char pin_value[12][2] = {
	{22, 0},
	{ 4, 0},
	{27, 0},
	{13, 0},
	{ 5, 0},
	{ 6, 0},
	{17, 0},
	{15, 0},
	{14, 0},
	{12, 0},
	{ 1, 0},
	{26, 0},
};
static char pin_mode_value[12][3] = {
	{22, 'o', 0},	//left, front, en
	{ 4, 'o', 0},
	{27, 'o', 0},
	{13, 'o', 0},	//left, near, en
	{ 5, 'o', 0},
	{ 6, 'o', 0},
	{17, 'o', 0},	//right, front, en
	{15, 'o', 0},
	{14, 'o', 0},
	{12, 'o', 0},	//right, near, en
	{ 1, 'o', 0},
	{26, 'o', 0},
};




int l298n_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	logtoall("@l298n_take\n");

	return 0;
}
int l298n_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	int k;
	u32 in = *(u8*)buf;
	logtoall("vehicleserver_sock:%x\n",in);
	switch(in){
	case 'w':
		for(k=0;k<4;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 0;
			pin_value[k*3+2][1] = 1;
		}
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 's':
		for(k=0;k<4;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 1;
			pin_value[k*3+2][1] = 0;
		}
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'a':
		for(k=0;k<2;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 1;
			pin_value[k*3+2][1] = 0;
		}
		for(k=2;k<4;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 0;
			pin_value[k*3+2][1] = 1;
		}
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'd':
		for(k=0;k<2;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 0;
			pin_value[k*3+2][1] = 1;
		}
		for(k=2;k<4;k++){
			pin_value[k*3+0][1] = 1;
			pin_value[k*3+1][1] = 1;
			pin_value[k*3+2][1] = 0;
		}
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	}

	for(k=0;k<4;k++){
		pin_value[k*3+0][1] = 0;
		pin_value[k*3+1][1] = 0;
		pin_value[k*3+2][1] = 0;
	}
	writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
	return 0;
}
int l298n_attach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		for(j=0;j<12;j++)pin_mode_value[j][1] = 'o';
		writing_data_into_peer(self->pchip, _chip_, 0, _pin_mode_value_, pin_mode_value, 12);
		break;
	}
	return 0;
}
int l298n_detach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		for(j=0;j<12;j++)pin_mode_value[j][1] = 'i';
		writing_data_into_peer(self->pchip, _chip_, 0, _pin_mode_value_, pin_mode_value, 12);
		break;
	}
	return 0;
}




int l298n_read(_obj* obj,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int l298n_write(_obj* obj,void* foot,p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int l298n_create(_obj* obj, void* arg, int argc, u8** argv)
{
	logtoall("@l298n_create\n");
	return 1;
}
int l298n_delete(_obj* ele)
{
	return 0;
}
