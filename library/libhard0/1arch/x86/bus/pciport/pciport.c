#include "libhard.h"
//
void ehci_portinit(struct item* dev, u32 addr);
void xhci_portinit(struct item* dev, u32 addr);
void usb4hia_portinit(struct item* dev, u32 addr);
//
void ide_portinit(struct item* dev, u32 addr);
void ahci_portinit(struct item* dev, u32 addr);
void nvme_portinit(struct item* dev, u32 addr);
//
void e1000_portinit(struct item* dev, u32 addr);
//
u32 in32(u16 port);
void out32(u16 port, u32 data);




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




void initpci_port()
{
	struct item* xx;
	u32 idid,type;
	u32 bus,dev,fun,addr;
	logtoall("@initpci_port\n");

	for(bus=0;bus<256;bus++){
	for(dev=0;dev<32;dev++){
	for(fun=0;fun<8;fun++){
		addr = 0x80000000 | (bus<<16) | (dev<<11) | (fun<<8);
		out32(0xcf8, addr);
		idid = in32(0xcfc);
		if(idid == 0xffffffff)continue;

		out32(0xcf8, addr+8);
		type = in32(0xcfc);
		logtoall("(%x,%x,%x)@%08x: idid=%08x, type=%08x\n",bus,dev,fun, addr, idid,type);

		switch(type >> 16){
		case 0x0101:
			xx = device_create(_ide_, 0, 0, 0);
			ide_portinit(xx, addr);
			break;

		case 0x0106:
			if(0x01 == ((type>>8)&0xff)){
				xx = device_create(_ahci_, 0, 0, 0);
				ahci_portinit(xx, addr);
			}
			break;

		case 0x0108:
			switch((type>>8)&0xff){
			case 0x02:
			case 0x03:
				xx = device_create(_nvme_, 0, 0, 0);
				nvme_portinit(xx, addr);
				break;
			}
			break;

		case 0x0200:
			if(0x100e8086 == idid){
				xx = device_create(_eth_, 0, 0, 0);
				e1000_portinit(xx, addr);
			}
			break;

		case 0x0c03:
			switch((type>>8)&0xff){
			case 0x00:
				logtoall("uhci\n");
				break;
			case 0x10:
				logtoall("ohci\n");
				break;
			case 0x20:
				logtoall("ehci\n");
				break;
			case 0x30:
				xx = device_create(_xhci_, 0, 0, 0);
				xhci_portinit(xx, addr);
				break;
			case 0x40:
				xx = device_create(_usb4_, 0, 0, 0);
				usb4hia_portinit(xx, addr);
				break;
			}//usbver
			break;

		}//class,subclass
	}//fun
	}//dev
	}//bus
}
