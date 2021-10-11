#include "libhard.h"
#define SDHCI_OFFS_OLD 0x00300000
#define SDHCI_OFFS_NEW 0x00340000	//when piver>4
int raspi_version();
//
void* getdtb();
void parsedevmap_dtb();
//
void initcpu_bsp();
void initcpu_ap();
//
void initsystmr(void*);
//sd
void initsdhci(void*, int offs);
void initsdhost(void*);
//typec
void brcmxhci_init();
void brcmdwc2_init();
//pcie
void brcmpcie_init();
void rpiextxhci_init();




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
/*
        sdhci   sdhost  sdhci2 dwc2    xhci    vl805
old     sdcard  -       -      -       -       -
pi3     wifi    sdcard  -      -       -       -
pi4     wifi    unused  sdcard typec-1 typec-2 4*usbA
cm4     wifi    unused  emmc   typec-1 typec-2 -

typec-1=
-otg_mode=1
+dtoverlay=dwc2

typec-2=
+otg_mode=1
-dtoverlay=dwc2
*/
	switch(raspi_version() ){
	case 4:
		//sdhost: can be confiured to use on gpio 22-26(sd0)
		p = devicecreate(_mmc_, 0, 0, 0);
		initsdhost(p);

		//sdhci_old: wifi		//todo
		//p = devicecreate(_mmc_, 0, 0, 0);
		//initsdhci(p, SDHCI_OFFS_OLD);

		//sdhci_new: pi4.sdcard or cm4.emmc
		p = devicecreate(_mmc_, 0, 0, 0);
		initsdhci(p, SDHCI_OFFS_NEW);

		//typec: dwc2 or internal_xhci
		brcmxhci_init();
		brcmdwc2_init();

		//pcie: a lot of things
		brcmpcie_init();
		//rpiextxhci_init();
		break;

	case 3:
		//sdhost: it's sdcard on pi3, it's none on qemu
		p = devicecreate(_mmc_, 0, 0, 0);
		initsdhost(p);

		//sdhci_old: it's wifi on pi3, it's sdcard on qemu
		p = devicecreate(_mmc_, 0, 0, 0);
		initsdhci(p, SDHCI_OFFS_OLD);

		break;

	default:
		break;
	}
}
