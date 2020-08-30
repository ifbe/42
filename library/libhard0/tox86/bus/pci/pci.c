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
	u32 idid,type;
	u32 bus,dev,fun,addr;
	say("@initpci\n");

	for(bus=0;bus<256;bus++){
	for(dev=0;dev<32;dev++){
	for(fun=0;fun<8;fun++){
		addr = 0x80000000 | (bus<<16) | (dev<<11) | (fun<<8);
		out32(0xcf8, addr);
		idid = in32(0xcfc);
		if(idid == 0xffffffff)continue;

		out32(0xcf8, addr+8);
		type = in32(0xcfc);
		say("@%08x(%d,%d,%d): idid=%08x, type=%08x\n", addr,bus,dev,fun, idid,type);

		switch(type >> 16){
		case 0x0101:
			//ide_portinit(addr);
			break;

		case 0x0106:
			//if(0x01 == ((type>>8)&0xff))ahci_portinit(addr);
			break;

		case 0x0108:
			//if(0x02 == ((type>>8)&0xff))nvme_portinit(addr);
			break;

		case 0x0200:
			//if(0x100e8086 == idid)e1000_portinit(addr);
			break;

/*		case 0x0c03:
			switch((type>>8)&0xff){
			case 0x00:uhci_portinit(addr);break;
			case 0x10:ohci_portinit(addr);break;
			case 0x20:ehci_portinit(addr);break;
			case 0x30:xhci_portinit(addr);break;
			}//usbver
			break;
*/
		}//class,subclass
	}//fun
	}//dev
	}//bus
}
