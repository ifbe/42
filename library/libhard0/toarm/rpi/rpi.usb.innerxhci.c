#include "libhard.h"
#define BRCM_XHCI 0x9C0000
void* mmiobase();
int xhci_mmioinit(struct item* dev, u8* xhciaddr);




//must add: otg_mode=1
//must del: dtoverlay=dwc2
void brcmxhci_init()
{
	say("@brcmxhci_init\n");
return;
	void* addr = mmiobase()+BRCM_XHCI;
	printmmio(addr, 0x100);

	struct item* per = device_create(_xhci_, 0, 0, 0);
	xhci_mmioinit(per, addr);
}
void brcmxhci_exit()
{
}
