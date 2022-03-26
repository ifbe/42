
#include "libhard.h"
#define nvme_print(fmt, ...) say("<%08lld,nvme>"fmt, timeread_us(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);
int nvme_mmioinit(struct item* dev, u8* nvmeaddr);




int nvme_portcaps(struct item* dev, u32 addr)
{
	u32 temp,next;

	out32(0xcf8, addr+0x34);
	next = in32(0xcfc)&0xff;
	nvme_print("pcicap@%x\n", next);

	while(1){
		if(next < 0x40)break;
		if(next > 0xfc)break;

		out32(0xcf8, addr+next);
		temp = in32(0xcfc);
		next = (temp>>8)&0xff;
		nvme_print("cap:type=%x,next=%x\n", temp&0xff, next);
	}
	return 0;
}
int nvme_portinit(struct item* dev, u32 addr)
{
	u32 tmp;
	u64 lo,hi;
	nvme_print("port@%x{\n",addr);

	out32(0xcf8, addr+8);
	tmp = in32(0xcfc);
	switch(tmp>>8){
	case 0x010802:nvme_print("nvme ioctl\n");break;
	case 0x010803:nvme_print("nvme admin\n");break;
	}

	out32(0xcf8, addr+0x10);
	lo = in32(0xcfc);
	nvme_print("bar0=%x\n", lo);
	out32(0xcf8, addr+0x14);
	hi = in32(0xcfc);
	nvme_print("bar1=%x\n", hi);
	out32(0xcf8, addr+0x18);
	nvme_print("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	nvme_print("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	nvme_print("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	nvme_print("bar5=%x\n", in32(0xcfc));

	nvme_portcaps(dev, addr);
	nvme_print("}\n");

	//nvme mmio
	u8* mmio = (u8*)((lo&0xffffc000) | (hi<<32));
	return nvme_mmioinit(dev, mmio);
}