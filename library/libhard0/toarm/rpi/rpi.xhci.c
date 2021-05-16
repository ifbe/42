#include "libhard.h"
#define BRCM_XHCI 0x9C0000
void* mmiobase();




void brcmxhci_init()
{
    say("@brcmxhci_init\n");
    printmmio(mmiobase()+BRCM_XHCI, 0x40);
}
void brcmxhci_exit()
{
}