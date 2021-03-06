#include "libhard.h"
int raspi_version();
//
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
//
void brcmpcie_init();
void brcmxhci_init();




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
	//turnoff peripheral

	//turnoff timer

	//turnoff gic

	//set cpu: no paging, no irq
}
void inithardware()
{
	struct item* p;

	//map
	initmemmap();
	initdevmap();

	//timer
	p = devicecreate(_tmr_, 0, 0, 0);
	initsystmr(p);

	//cpu
	p = devicecreate(_cpu_, 0, 0, 0);
	initcpu_bsp(p);
	initcpu_ap(p);

	//sdhci
	p = devicecreate(_mmc_, 0, 0, 0);
	initsdhci(p);

	//sdhost
	p = devicecreate(_mmc_, 0, 0, 0);
	initsdhost(p);

	if(4 == raspi_version()){
		brcmpcie_init();
		brcmxhci_init();
	}
}
