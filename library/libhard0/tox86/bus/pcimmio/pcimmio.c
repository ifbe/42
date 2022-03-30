#include "libhard.h"
int acpi_getpcie(void** addr, int* size);




struct MCFG_CONFSPACE{
	u64    BaseAddr;	//[0.7]
	u16    GroupNum;	//[8,9]
	u8 BusNum_start;	//a
	u8   BusNum_end;	//b
	u32        Rsvd;	//[c,f]
}__attribute__((packed));




void initpci_mmio()
{
	say("@initpci_mmio\n");

	struct MCFG_CONFSPACE* conf = 0;
	int size = 0;
	int ret = acpi_getpcie(&conf, &size);
	if(0 == ret)return;
	if(0 == conf)return;
	if(0 == size)return;
	say("acpi mcfg: conf=%p, size=%x\n", conf, size);

	int j,cnt;
	u32* addr;
	u32 bus,dev,fun;
	u32 idid,type;
	for(j=0;j<size;j++){
		say("#%d: addr=%p, group=%x, busstart=%x, busend=%x\n", j,
			conf[j].BaseAddr, conf[j].GroupNum, conf[j].BusNum_start, conf[j].BusNum_end);

		cnt = conf[j].BusNum_end - conf[j].BusNum_start;
		for(bus=0;bus<cnt;bus++){		//max 256
		for(dev=0;dev<32;dev++){
		for(fun=0;fun<8;fun++){
			addr = (void*)conf[j].BaseAddr + (bus<<20) + (dev<<15) + (fun<<12);
			if(0xffffffff == addr[0])continue;

			idid = addr[0];
			type = addr[2];
			say("(%x,%x,%x,%x)@%p: idid=%08x, type=%08x\n",
				conf[j].GroupNum, conf[j].BusNum_start+bus, dev, fun,
				addr, idid, type);

			switch(type >> 16){
			case 0x0101:
				say("ide\n");
				break;

			case 0x0106:
				if(0x01 == ((type>>8)&0xff)){
					say("ahci\n");
				}
				break;

			case 0x0108:
				switch((type>>8)&0xff){
				case 0x02:
				case 0x03:
					say("nvme\n");
					break;
				}
				break;

			case 0x0200:
				if(0x100e8086 == idid){
					say("eth.e1000\n");
				}
				else{
					say("eth\n");
				}
				break;

			case 0x0c03:
				switch((type>>8)&0xff){
				case 0x00:
					say("uhci\n");
					break;
				case 0x10:
					say("ohci\n");
					break;
				case 0x20:
					say("ehci\n");
					break;
				case 0x30:
					say("xhci\n");
					break;
				case 0x40:
					say("usb4hia\n");
					break;
				}//usbver
				break;

			}//switch
		}//each fun
		}//each dev
		}//each bus
	}//each mcfg
}
