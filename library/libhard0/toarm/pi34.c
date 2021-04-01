#include "libhard.h"
void* getdtb();
void parsedevmap_dtb();
//
void initcpu_bsp();
void initcpu_ap();
//
void initsystmr(void*);
//
void initsdhci(void*);
void initsdhost(void*);




void initmemmap()
{
}
void initdevmap()
{
	void* dtb = getdtb();
	if(0 == dtb)return;

	parsedevmap_dtb(dtb);
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
	initcpu_bsp(p);
	initcpu_ap(p);

	//timer
	p = devicecreate(_tmr_, 0, 0, 0);
	initsystmr(p);

	//sdhci
	p = devicecreate(_mmc_, 0, 0, 0);
	initsdhci(p);

	//sdhost
	p = devicecreate(_mmc_, 0, 0, 0);
	initsdhost(p);
}
