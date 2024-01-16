
#include "libhard.h"
#define usb4_print(fmt, ...) logtoall("<%08lld,usb4hia>"fmt, timeread_us(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);




int usb4hia_portcaps(struct item* dev, u32 addr)
{
	u32 temp,next;

	out32(0xcf8, addr+0x34);
	next = in32(0xcfc)&0xff;
	usb4_print("pcicap@%x\n", next);

	while(1){
		if(next < 0x40)break;
		if(next > 0xfc)break;

		out32(0xcf8, addr+next);
		temp = in32(0xcfc);
		next = (temp>>8)&0xff;
		usb4_print("cap:type=%x,next=%x\n", temp&0xff, next);
	}
	return 0;
}
int usb4hia_portinit(struct item* dev, u32 addr)
{
	u64 temp,high;
	usb4_print("port@%x{\n", addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	usb4_print("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp | (1<<10) | (1<<2));	//bus master=1

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc);
	usb4_print("bar0=%x\n", temp);			//lo
	out32(0xcf8, addr+0x14);
	high = in32(0xcfc);
	usb4_print("bar1=%x\n", high);			//hi
	out32(0xcf8, addr+0x18);
	usb4_print("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	usb4_print("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	usb4_print("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	usb4_print("bar5=%x\n", in32(0xcfc));

	usb4hia_portcaps(dev, addr);

	//if(intel PantherPoint):
	//Write 0xFFFFFFFF (as a dword) to the PCI Config Registers 0xD8 and 0xD0
	usb4_print("}\n");


	//usb4hia mmio
	u8* mmio = (u8*)((temp&0xfffffff0) | (high<<32));
    printmmio(mmio, 0x40);
	return 0;
}
