#include "libhard.h"
void* acpi_getpcieaddr();




void initpci_mmio()
{
	say("@initpci_mmio\n");

	void* addr = acpi_getpcieaddr();
	if(0 == addr)return;

	int j;
	for(j=0;j<2;j++){
		printmmio(addr + (j<<12), 0x40);
	}
}
