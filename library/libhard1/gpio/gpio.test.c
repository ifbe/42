#include "libsoft.h"
#define _con_ hex32('c','o','n', 0)
#define _chip_ hex32('c','h','i','p')
#define _pin_value_ hex32('p','v', 0, 0)
#define _pin_mode_value_ hex32('p','m','v', 0)


static u8 pin_mode_value[2][3] = {
{23, 'o', 0},
{24, 'i', 1}
};


static void onrecvkey(_obj* obj, u8* buf, int len)
{
	logtoall("buf[0]=%x\n", buf[0]);
	u8 pin_value[1][2];
	switch(buf[0]){
	case '0':
		pin_value[0][0] = 24;
		pin_value[0][1] = 0;
		writing_data_into_peer(obj, _chip_, 0,_pin_value_, pin_value, 1);
		break;
	case '1':
		pin_value[0][0] = 24;
		pin_value[0][1] = 1;
		writing_data_into_peer(obj, _chip_, 0,_pin_value_, pin_value, 1);
		break;
	}

	pin_value[0][0] = 23;
	reading_data_from_peer(obj, _chip_, 0,_pin_value_, pin_value, 1);
	logtoall("volt(%d)=%d\n", pin_value[0][0], pin_value[0][1]);
}


int gpiotest_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	logtoall("@gpiotest_take\n");

	return 0;
}
int gpiotest_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	u32 foottype = stack[sp-1].foottype;
	//logtoall("@gpiotest_give:%.4s\n", foottype);

	switch(foottype){
	case _chip_:
		break;
	case _con_:
		onrecvkey(obj, buf, len);
		break;
	}

	return 0;
}
int gpiotest_attach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		pin_mode_value[0][1] = 'i';
		pin_mode_value[1][1] = 'o';
		writing_data_into_peer(self->pchip, _chip_, 0,_pin_mode_value_, pin_mode_value, 2);
		break;
	}
	return 0;
}
int gpiotest_detach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		pin_mode_value[0][1] = 'i';
		pin_mode_value[1][1] = 'i';
		writing_data_into_peer(self->pchip, _chip_, 0,_pin_mode_value_, pin_mode_value, 2);
		break;
	}
	return 0;
}




int gpiotest_read(_obj* obj,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int gpiotest_write(_obj* obj,void* foot,p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int gpiotest_create(_obj* obj, void* arg, int argc, u8** argv)
{
	logtoall("@gpiotest_create\n");
	return 1;
}
int gpiotest_delete(_obj* ele)
{
	return 0;
}
