#include "libhard.h"
void initsystmr();
void initsd();




void freehardware()
{
}
void inithardware()
{
	struct item* p;

	//cpu
	p = devicecreate(_cpu_, 0, 0, 0);

	//tmr
	initsystmr();

	//sdcard
	p = devicecreate(_mmc_, 0, 0, 0);
	initsd();
}
