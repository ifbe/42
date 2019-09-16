#include "libsoft.h"
#define PI 3.1415926535897932384626433832795




int readlsm9ds1(void* it, float* measure)
{
	short t;
	u8 reg[32];

	t = relationread(it, _src_, 0, 0x6a0018, reg+0, 6);
	t = relationread(it, _src_, 0, 0x6a0019, reg+1, 5);
	t = relationread(it, _src_, 0, 0x6a001a, reg+2, 4);
	t = relationread(it, _src_, 0, 0x6a001b, reg+3, 3);
	t = relationread(it, _src_, 0, 0x6a001c, reg+4, 2);
	t = relationread(it, _src_, 0, 0x6a001d, reg+5, 1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[0] = t * 500.0/32768 * PI/180.0;
	t = *(short*)(reg+2);
	measure[1] =-t * 500.0/32768 * PI/180.0;
	t = *(short*)(reg+4);
	measure[2] = t * 500.0/32768 * PI/180.0;


	relationread(it, _src_, 0, 0x6a0028, reg+0, 6);
	relationread(it, _src_, 0, 0x6a0029, reg+1, 5);
	relationread(it, _src_, 0, 0x6a002a, reg+2, 4);
	relationread(it, _src_, 0, 0x6a002b, reg+3, 3);
	relationread(it, _src_, 0, 0x6a002c, reg+4, 2);
	relationread(it, _src_, 0, 0x6a002d, reg+5, 1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[3] = t * 9.8*8 / 32768.0;
	t = *(short*)(reg+2);
	measure[4] =-t * 9.8*8 / 32768.0;
	t = *(short*)(reg+4);
	measure[5] = t * 9.8*8 / 32768.0;


	relationread(it, _src_, 0, 0x1c0028, reg+0, 6);
	relationread(it, _src_, 0, 0x1c0029, reg+1, 5);
	relationread(it, _src_, 0, 0x1c002a, reg+2, 4);
	relationread(it, _src_, 0, 0x1c002b, reg+3, 3);
	relationread(it, _src_, 0, 0x1c002c, reg+4, 2);
	relationread(it, _src_, 0, 0x1c002d, reg+5, 1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[6] =-t * 400.0 / 32768.0;
	t = *(short*)(reg+2);
	measure[7] =-t * 400.0 / 32768.0;
	t = *(short*)(reg+4);
	measure[8] = t * 400.0 / 32768.0;

	say("%f,%f,%f, %f,%f,%f, %f,%f,%f\n",
		measure[0], measure[1], measure[2],
		measure[3], measure[4], measure[5],
		measure[5], measure[7], measure[8]);
	return 9;
}




int lsm9ds1_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int lsm9ds1_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int ret;
	float tmp[10];
	void* it = (void*)(self->chip);

	if(_clk_ == self->flag){
		ret = readlsm9ds1(it, tmp);
		relationwrite(it, _dst_, 0, 0, tmp, ret);
	}
	return 0;
}
int lsm9ds1_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int lsm9ds1_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int lsm9ds1_create(struct element* ele, u8* url)
{
	say("@lsm9ds1_create\n");
	return 1;
}
int lsm9ds1_delete(struct element* ele)
{
	return 0;
}
