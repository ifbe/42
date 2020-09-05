#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);




void ahci_mmioinit(u8* addr)
{
	say("ahci@mmio:%p{\n", addr);
	printmmio(addr, 0x20);
    say("}\n");
}
void ahci_portinit(u32 addr)
{
	unsigned int temp;
	say("ahci@port:%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc) | (1<<10) | (1<<2);		//bus master=1
	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp);

	out32(0xcf8, addr+0x4);
	say("sts,cmd=%x\n", in32(0xcfc));

	out32(0xcf8, addr+0x10);
	say("bar0=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x14);
	say("bar1=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x18);
	say("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	say("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	say("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	temp = in32(0xcfc);
	say("bar5=%x\n", temp);

	say("}\n");


	//hba addr
	u8* mmio = (u8*)(u64)(temp&0xfffffffe);
	ahci_mmioinit(mmio);
}