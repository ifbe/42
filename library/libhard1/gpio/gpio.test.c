#include "libsoft.h"
#define _pin_mode_value_ hex32('p','m','v', 0)


static u8 pin_mode_value[2][3] = {
{23, 'i', 0},
{24, 'o', 1}
};


int gpiotest_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	say("@gpiotest_take\n");

	return 0;
}
int gpiotest_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int gpiotest_attach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _gpio_:
		writing_data_into_peer(self->pchip, _gpio_, (p64)pin_mode_value,_pin_mode_value_, 0, 0);
		break;
	}
	return 0;
}
int gpiotest_detach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _gpio_:
		writing_data_into_peer(self->pchip, _gpio_, (p64)pin_mode_value,_pin_mode_value_, 0, 0);
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
	say("@gpiotest_create\n");
	return 1;
}
int gpiotest_delete(_obj* ele)
{
	return 0;
}
