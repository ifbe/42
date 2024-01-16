#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);




void ide_portinit(struct item* dev, u32 addr)
{
	u32 temp;
	logtoall("ide@port:%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp=in32(0xcfc) | (1<<10) | (1<<2);	//bus master=1
	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	logtoall("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x10);
	logtoall("bar0=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x14);
	logtoall("bar1=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x18);
	logtoall("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	logtoall("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	logtoall("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	logtoall("bar5=%x\n", in32(0xcfc));

    logtoall("}\n");
}
