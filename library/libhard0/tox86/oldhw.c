#include "libhard.h"
//
void* getmemmap();
void initmemmap(void*);
//
void* getdevmap();
void initacpi(void*);
//
void initcpu_bsp(struct item* p);
void initcpu_ap();
//
void initpci_port();
void initpci_mmio();
//
void init8259();        //interrupt controller
void initapic();
//
void init825x();        //timer.pit
void initrtc();         //timer.rtc




void freehardware()
{
}
void inithardware()
{
	struct item* p;

	//mem
	initmemmap(getmemmap());

	//acpi
	initacpi(getdevmap());

	//cpu0: gdt,idt,paging...
	p = devicecreate(_cpu_, 0, 0, 0);
	initcpu_bsp(p);

	//interrupter
	p = devicecreate(_irq_, 0, 0, 0);
	init8259();
	initapic();

	//timer
	p = devicecreate(_tmr_, 0, 0, 0);
	initrtc();
	init825x();

	//cpu ap: after bsp and apic and timer
	initcpu_ap();

	//pci
	p = devicecreate(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();
}
