#include "libhard.h"
//
void* getmemmap();
void parsememmap_bios(void*);
void parsememmap_uefi(void*);
//
void* getdevmap();
void parsedevmap_acpi(void*);
void parsedevmap_dts(void*);
//
void localapic_check();
void initirq();
//
void initcpu_bsp(struct item* p);
void initcpu_ap();
//
void initpci_port();
void initpci_mmio();
//
void init825x();        //timer.pit
void initrtc();         //timer.rtc
//
void init8042();




void freehardware()
{
}
void inithardware()
{
	struct item* p;

	//mem
	parsememmap_uefi(getmemmap());

	//acpi
	parsedevmap_acpi(getdevmap());

	//localapic check
	localapic_check();

	//cpu_bsp: gdt,paging,idt,apic...
	p = device_create(_cpu_, 0, 0, 0);
	initcpu_bsp(p);

	//ioapic or dual8259
	p = device_create(_irq_, 0, 0, 0);
	initirq(p);

	//timer
	p = device_create(_tmr_, 0, 0, 0);
	initrtc();
	init825x();

	//cpu_app: after cpu_bsp and pic and timer
	initcpu_ap();

	//input
	init8042();

	//pci
	p = device_create(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();
}
