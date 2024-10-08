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
void brcmxhci_init(void*);
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

	struct item* devmap = driver_alloc_fromtype(_devmap_);
	parsedevmap_dtb(dtb);
}
void initmap()
{
	initmemmap();
	initdevmap();
}




void initdbg()
{
	//debug serial

	//debug framebuffer

	//print all pending log
}




void initcpu0()
{
}




void initarch()
{
	struct item* p;

	//timer
	p = device_alloc_fromtype(_tmr_);
	initsystmr(p);

	//cpu
	p = device_alloc_fromtype(_cpu_);
	initcpu_bsp(p);
	initcpu_ap(p);

	//mbox
	p = device_alloc_fromtype(_mbox_);
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
	p = device_alloc_fromtype(_mmc_);
	initsdhost(p);

	//sdhci_old: it's wifi on pi3, it's sdcard on qemu
	p = device_alloc_fromtype(_mmc_);
	initsdhci_bcm283xsdcard(p, SDHCI_OFFS_OLD);

	p = device_alloc_fromtype(_dwc2_);
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
	p = device_alloc_fromtype(_mmc_);
	initsdhost(p);

	//sdhci_new: pi4.sdcard or cm4.emmc
	p = device_alloc_fromtype(_mmc_);
	initsdhci_bcm2711sdcard(p, SDHCI_OFFS_NEW);

	//sdhci_old: wifi		//todo
	p = device_alloc_fromtype(_wifi_);
	initsdhci_wifi(p, SDHCI_OFFS_OLD);
/*
	//uart0: bt
	p = device_alloc_fromtype(_bt_);
	initrpibt();
*/
	//typec: dwc2 or internal_xhci
	if(0){
		p = device_alloc_fromtype(_xhci_);
		brcmxhci_init(p);
	}
	else{
		p = device_alloc_fromtype(_dwc2_);
		brcmdwc2_init();
	}
/*
	//pcie: a lot of things
	p = device_alloc_fromtype(_pcie_);
	brcmpcie_init();
	//rpiextxhci_init();
*/
}
void initsoc()
{
	//pinctrl
	struct item* p;
	p = device_alloc_fromtype(_pinmgr_);
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




void initboard()
{
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
	initdbg();


	initcpu0();
	initarch();
	initsoc();
	initboard();
}
