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
void inittimer();
//
void init8042();
//
void initvmtool();




void initdbg()
{
	//debug serial

	//debug framebuffer
	_obj* wnd = supply_prep(0, _wnd_, 0, 0);
	supply_create(wnd, 0, 0, 0);
}




void exitmap()
{
}
void initmap()
{
	//mem
	parsememmap_uefi(getmemmap());

	//acpi
	parsedevmap_acpi(getdevmap());
}




void initcpu0()
{
	struct item* p;

	//localapic check
	localapic_check();

	//cpu_bsp: gdt,paging,idt,apic...
	p = device_create(_cpu_, 0, 0, 0);
	initcpu_bsp(p);
}




void exitarch()
{
}
void initarch()
{
	struct item* p;

	//ioapic or dual8259
	p = device_create(_irq_, 0, 0, 0);
	initirq(p);

	//timer
	p = device_create(_tmr_, 0, 0, 0);
	inittimer();

	//cpu_app: after cpu_bsp and pic and timer
	initcpu_ap();
}




void exitsoc()
{
}
void initsoc()
{
	struct item* p;

	//input
	init8042();

	//pci
	p = device_create(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();
}




void exitboard()
{
}
void initboard()
{
	//vmware
	initvmtool();
}




void freehardware()
{
}
void inithardware()
{
	initdbg();
	initmap();


	initcpu0();
	initarch();
	initsoc();
	initboard();
}
