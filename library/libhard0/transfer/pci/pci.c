#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u8 in8(u16 port);
u16 in16(u16 port);
u32 in32(u16 port);
void out8(u16 port, u8 data);
void out16(u16 port, u16 data);
void out32(u16 port, u32 data);
void say(void*, ...);




struct pci
{
	//[0,f]
	u32 id;
	u32 stscmd;
	u32 class;
	u32 what;

	//[10,27]
	u32 bar0;
	u32 bar1;
	u32 bar2;
	u32 bar3;
	u32 bar4;
	u32 bar5;

	//[28,3f]
	u32 cardbus;
	u32 subsys;
	u32 expansion;
	u32 capability;
	u32 rsvd;
	u32 interrupt;
};




void initpci()
{
	u32 addr,d1,d2;
	u32 bus,dev,fun;
	for(bus=0;bus<256;bus++)
	{
	for(dev=0;dev<32;dev++)
	{
	for(fun=0;fun<8;fun++)
	{
		addr = (bus<<16) | (dev<<11) | (fun<<8);
		out32(0xcf8, 0x80000000 | addr);
		d1 = in32(0xcfc);
		if(d1 == 0xffffffff)continue;

		out32(0xcf8, 0x80000000 | (addr+0x8));
		d2 = in32(0xcfc);
		if(d2 == 0xffffffff)continue;

		say("bus:%d, dev:%d, fun:%d, vendor:%08x, class:%08x\n",
			bus, dev, fun, d1, d2
		);
	}
	}
	}
}
