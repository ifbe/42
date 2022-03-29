#include "libhard.h"
void* acpi_getpcieaddr();




void initpci_mmio()
{
	say("@initpci_mmio\n");

	void* base = acpi_getpcieaddr();
	if(0 == base)return;
	say("pciebase=%p\n", base);

	u32 bus,dev,fun;
	u32* addr;
	for(bus=0;bus<2;bus++){		//max 256
	for(dev=0;dev<32;dev++){
	for(fun=0;fun<8;fun++){
		addr = base + (bus<<20) + (dev<<15) + (fun<<12);
		if(0xffffffff == addr[0])continue;

		say("@%p(%x,%x,%x): idid=%08x, type=%08x\n", addr,bus,dev,fun, addr[0],addr[2]);
	}
	}
	}
}
