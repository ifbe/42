#include "libhard.h"
void* getdtb();
void parsedtb();
//
void initsystmr();
void initsdcard();




void initmemmap()
{
}
void initdevmap()
{
	void* dtb = getdtb();
	if(0 == dtb)return;

	parsedtb(dtb);
}




void freehardware()
{
}
void inithardware()
{
	struct item* p;

	//map
	initmemmap();
	initdevmap();

	//cpu
	p = devicecreate(_cpu_, 0, 0, 0);

	//timer
	p = devicecreate(_tmr_, 0, 0, 0);
	initsystmr();

	//sdcard
	p = devicecreate(_mmc_, 0, 0, 0);
	initsdcard();
}
