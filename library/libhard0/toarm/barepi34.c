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
//
void initpinmgr();
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
	p = device_create(_tmr_, 0, 0, 0);
	initsystmr(p);

	//cpu
	p = device_create(_cpu_, 0, 0, 0);
	initcpu_bsp(p);
	initcpu_ap(p);

	//pinctrl
	initpinmgr();

	//soc, board
	switch(raspi_version() ){
/*
bcm2711(4b, 4cm)
sd/mmc  sdhci2@ sdhost
wifi    sdhci@
bt      uart0   uart1
typec   xhci    dwc2
usbA    vl805

(dwc2)=
-otg_mode=1
+dtoverlay=dwc2
(xhci)=
+otg_mode=1
-dtoverlay=dwc2
*/
	case 4:
		//sdhost: can be confiured to use on gpio 22-26(sd0)
		p = device_create(_mmc_, 0, 0, 0);
		initsdhost(p);

		//sdhci_new: pi4.sdcard or cm4.emmc
		p = device_create(_mmc_, 0, 0, 0);
		initsdhci(p, SDHCI_OFFS_NEW);

		//sdhci_old: wifi		//todo
		//p = device_create(_mmc_, 0, 0, 0);
		//initsdhci(p, SDHCI_OFFS_OLD);

		//typec: dwc2 or internal_xhci
		brcmxhci_init();
		brcmdwc2_init();

		//pcie: a lot of things
		brcmpcie_init();
		//rpiextxhci_init();
		break;
/*
bcm2837(3b, 3cm, zero2w)
sd/mmc  sdhost
wifi    sdhci
bt      uart0   uart1
usbA    xhci
*/
	case 3:
		//sdhost: it's sdcard on pi3, it's none on qemu
		p = device_create(_mmc_, 0, 0, 0);
		initsdhost(p);

		//sdhci_old: it's wifi on pi3, it's sdcard on qemu
		p = device_create(_mmc_, 0, 0, 0);
		initsdhci(p, SDHCI_OFFS_OLD);

		break;
/*
bcm2835(1b, 2b, zerow)
sd/mmc  sdhci
wifi    -
bt      -
*/
	default:
		break;
	}
}
