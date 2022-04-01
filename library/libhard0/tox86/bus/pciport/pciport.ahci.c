
#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);
void ahci_mmioinit(struct item* dev, void* abar);
#define ahci_print(fmt, ...) say("<%08lld,ahci>" fmt, timeread_us(), ##__VA_ARGS__)




void ahci_portinit(struct item* dev, u32 addr)
{
	u32 temp;
	ahci_print("port@%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	ahci_print("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp | (1<<10) | (1<<2));		//bus master=1

	out32(0xcf8, addr+0x10);
	ahci_print("bar0=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x14);
	ahci_print("bar1=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x18);
	ahci_print("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	ahci_print("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	ahci_print("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	temp = in32(0xcfc);
	ahci_print("bar5=%x\n", temp);

	ahci_print("}\n");


	//hba addr
	void* mmio = (void*)(u64)(temp&0xfffffff0);
	ahci_mmioinit(dev, mmio);
}
