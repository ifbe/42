#include "libhard.h"
#define BRCM_XHCI 0x9C0000
void* mmiobase();




//must add: otg_mode=1
//must del: dtoverlay=dwc2
void brcmxhci_init()
{
    say("@brcmxhci_init\n");
    printmmio(mmiobase()+BRCM_XHCI, 0x100);
}
void brcmxhci_exit()
{
}