
#include "libhard.h"
#define xhci_print(fmt, ...) say("<%08lld,xhci>"fmt, timeread_us(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);
int xhci_mmioinit(struct item* dev, u8* xhciaddr);




int xhci_portcaps(struct item* dev, u32 addr)
{
	u32 temp,next;

	out32(0xcf8, addr+0x34);
	next = in32(0xcfc)&0xff;
	xhci_print("pcicap@%x\n", next);

	while(1){
		if(next < 0x40)break;
		if(next > 0xfc)break;

		out32(0xcf8, addr+next);
		temp = in32(0xcfc);
		next = (temp>>8)&0xff;
		xhci_print("cap:type=%x,next=%x\n", temp&0xff, next);
	}
	return 0;
}
int xhci_portinit(struct item* dev, u32 addr)
{
	u64 temp,high;
	xhci_print("port@%x{\n", addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	xhci_print("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp | (1<<10) | (1<<2));	//bus master=1

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc);
	xhci_print("bar0=%x\n", temp);			//lo
	out32(0xcf8, addr+0x14);
	high = in32(0xcfc);
	xhci_print("bar1=%x\n", high);			//hi
	out32(0xcf8, addr+0x18);
	xhci_print("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	xhci_print("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	xhci_print("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	xhci_print("bar5=%x\n", in32(0xcfc));

	xhci_portcaps(dev, addr);

	//if(intel PantherPoint):
	//Write 0xFFFFFFFF (as a dword) to the PCI Config Registers 0xD8 and 0xD0
	xhci_print("}\n");


	//xhci mmio
	u8* mmio = (u8*)((temp&0xfffffff0) | (high<<32));
	return xhci_mmioinit(dev, mmio);
}
