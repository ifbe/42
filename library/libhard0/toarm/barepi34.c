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
void initmbox();
//
void initpinmgr();
//sd
void initsdhost(void*);
void initsdhci_wifi(void*, int offs);
void initsdhci_bcm283xsdcard(void*, int offs);
void initsdhci_bcm2711sdcard(void*, int offs);
//bt
void initrpibt();
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
void initmap()
{
	initmemmap();
	initdevmap();
}



void initarch()
{
	struct item* p;

	//timer
	p = device_create(_tmr_, 0, 0, 0);
	initsystmr(p);

	//cpu
	p = device_create(_cpu_, 0, 0, 0);
	initcpu_bsp(p);
	initcpu_ap(p);

	//mbox
	initmbox();
}




/*
bcm2835(1b, 2b, zerow)
sd/mmc  sdhci
wifi    -
bt      -
*/
void initbcm2835()
{
}



/*
bcm2837(3b, 3cm, zero2w)
sd/mmc  sdhost
wifi    sdhci
bt      uart0   uart1
usbA    xhci
*/
void initbcm2837()
{
	struct item* p;

	//sdhost: it's sdcard on pi3, it's none on qemu
	p = device_create(_mmc_, 0, 0, 0);
	initsdhost(p);

	//sdhci_old: it's wifi on pi3, it's sdcard on qemu
	p = device_create(_mmc_, 0, 0, 0);
	initsdhci_bcm283xsdcard(p, SDHCI_OFFS_OLD);

	brcmdwc2_init();
}



/*
bcm2711(4b, 4cm)
sd/mmc  sdhci2@ sdhost
wifi    sdhci@
bt      uart0   uart1
typec   xhci    dwc2
usbA    vl805

#sd/mmc:
#0x72e000d0.bit1 controls if the uSD slot goes to EMMC2(the ddr capable sdhci) or EMMC (the old sdhci), this bypasses the normal gpio altfunctions, and the uSD isnt on gpio pins

#typec dwc2:
-otg_mode=1
+dtoverlay=dwc2

#typec xhci:		//must set below, otherwise stuck
+otg_mode=1
-dtoverlay=dwc2
*/
void initbcm2711()
{
	struct item* p;

	//sdhost: can be confiured to use on gpio 22-26(sd0)
	p = device_create(_mmc_, 0, 0, 0);
	initsdhost(p);

	//sdhci_new: pi4.sdcard or cm4.emmc
	p = device_create(_mmc_, 0, 0, 0);
	initsdhci_bcm2711sdcard(p, SDHCI_OFFS_NEW);

	//sdhci_old: wifi		//todo
	p = device_create(_wifi_, 0, 0, 0);
	initsdhci_wifi(p, SDHCI_OFFS_OLD);

	//uart0: bt
	p = device_create(_bt_, 0, 0, 0);
	initrpibt();

	//typec: dwc2 or internal_xhci
	brcmxhci_init();
	brcmdwc2_init();

	//pcie: a lot of things
	brcmpcie_init();
	//rpiextxhci_init();
}
void initsoc()
{
	//pinctrl
	initpinmgr();

	//soc, board
	switch(raspi_version() ){
	case 4:
		initbcm2711();
		break;
	case 3:
		initbcm2837();
		break;
	default:
		initbcm2835();
		break;
	}
}




void freehardware()
{
	//turnoff soc

	//turnoff arch

	//set cpu: no paging, no irq
}
void inithardware()
{
	initmap();

	initarch();

	initsoc();
}
