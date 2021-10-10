#include "libhard.h"
#define DWC2BASE 0x980000
void* mmiobase();




//must add: dtoverlay=dwc2
//must del: otg_mode=1
void brcmdwc2_init()
{
    say("@brcmdwc2_init\n");
    printmmio(mmiobase()+DWC2BASE, 0x100);
}
void brcmdwc2_exit()
{
}