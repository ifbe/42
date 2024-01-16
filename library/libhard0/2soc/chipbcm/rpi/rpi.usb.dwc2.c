#include "libhard.h"
void* mmiobase();
#define DWC2BASE 0x980000

//common
#define GOTGCTL   0x000
#define GOTGINT   0x004
#define GAHBCFG   0x008
#define GUSBCFG   0x00C
#define GRSTCTL   0x010
#define GINTSTS   0x014
#define GINTMSK   0x018
#define GRXSTSR   0x01C
#define GRXSTSP   0x020
#define GRXFSIZ   0x024
#define GNPTXFSIZ 0x028
#define GNPTXSTS  0x02C
#define GI2CCTL   0x0030
#define GPVNDCTL  0x0034
#define GGPIO     0x0038
#define GUID      0x003c
#define GSNPSID   0x0040
#define GHWCFG1   0x0044
#define GHWCFG2   0x0048
#define GHWCFG3   0x004c
#define GHWCFG4   0x0050
#define GLPMCFG   0x0054
#define GPWRDN    0x0058
#define GDFIFOCFG 0x005c
#define ADPCTL    0x0060
#define GREFCLK   0x0064
#define GINTMSK2  0x0068
#define GINTSTS2  0x006c

//host reg
#define HCFG     0x0400
#define HFIR     0x0404
#define HFNUM    0x0408
#define HPTXSTS  0x0410
#define HAINT    0x0414
#define HAINTMSK 0x0418
#define HFLBADDR 0x041c
#define HPRT0    0x0440

//peri reg
#define DCFG       0x800
#define DCTL       0x804
#define DSTS       0x808
#define DIEPMSK    0x810
#define DOEPMSK    0x814
#define DAINT      0x818
#define DAINTMSK   0x81C
#define DTKNQR1    0x820
#define DTKNQR2    0x824
#define DVBUSDIS   0x828
#define DVBUSPULSE 0x82C
#define DTKNQR3    0x830
#define DIEPEMPMSK 0x834
#define DIEPCTL0   0x900
#define DIEPTSIZ0  0x910
#define DOEPCTL0   0xB00
#define DOEPTSIZ0  0xB10
#define PCGCTL     0xe00
#define PCGCCTL1   0xe04




//must add: dtoverlay=dwc2
//must del: otg_mode=1
void brcmdwc2_init()
{
    logtoall("@brcmdwc2_init\n");

    printmmio(mmiobase()+DWC2BASE+GOTGCTL, 0x80);

    printmmio(mmiobase()+DWC2BASE+HCFG, 0x20);

    printmmio(mmiobase()+DWC2BASE+DCFG, 0x20);
}
void brcmdwc2_exit()
{
}