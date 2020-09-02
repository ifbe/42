#warning "this havn't been done"
#include "libhard.h"
void* mmiobase();




//
#define GPIO_BASE       (mmio+0x00200000)
#define GPFSEL0         ((volatile unsigned int*)(mmio+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(mmio+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(mmio+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(mmio+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(mmio+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(mmio+0x00200014))
#define GPSET0          ((volatile unsigned int*)(mmio+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(mmio+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(mmio+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(mmio+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(mmio+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(mmio+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(mmio+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(mmio+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(mmio+0x00200068))
#define GPPUD           ((volatile unsigned int*)(mmio+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(mmio+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(mmio+0x0020009C))
//
#define EMMC_BASE           (mmio+0x00300000)
#define EMMC_ARG2           ((volatile unsigned int*)(mmio+0x00300000))
#define EMMC_BLKSIZECNT     ((volatile unsigned int*)(mmio+0x00300004))
#define EMMC_ARG1           ((volatile unsigned int*)(mmio+0x00300008))
#define EMMC_CMDTM          ((volatile unsigned int*)(mmio+0x0030000C))
#define EMMC_RESP0          ((volatile unsigned int*)(mmio+0x00300010))
#define EMMC_RESP1          ((volatile unsigned int*)(mmio+0x00300014))
#define EMMC_RESP2          ((volatile unsigned int*)(mmio+0x00300018))
#define EMMC_RESP3          ((volatile unsigned int*)(mmio+0x0030001C))
#define EMMC_DATA           ((volatile unsigned int*)(mmio+0x00300020))
#define EMMC_STATUS         ((volatile unsigned int*)(mmio+0x00300024))
#define EMMC_CONTROL0       ((volatile unsigned int*)(mmio+0x00300028))
#define EMMC_CONTROL1       ((volatile unsigned int*)(mmio+0x0030002C))
#define EMMC_INTERRUPT      ((volatile unsigned int*)(mmio+0x00300030))
#define EMMC_INT_MASK       ((volatile unsigned int*)(mmio+0x00300034))
#define EMMC_INT_EN         ((volatile unsigned int*)(mmio+0x00300038))
#define EMMC_CONTROL2       ((volatile unsigned int*)(mmio+0x0030003C))
#define EMMC_SLOTISR_VER    ((volatile unsigned int*)(mmio+0x003000FC))




void freesd()
{
}
void initsd()
{
	u8* mmio = mmiobase();
	printmmio(EMMC_BASE, 0x100);
}
