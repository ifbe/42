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
void initirq(void*);
//
void initcpu_bsp(struct item* p);
void initcpu_ap();
//
void initpci_port(void*);
void initpci_mmio();
//
void inittimer();
//
void init8042();
//
void initboarddebug(void*);
void initvmtool();




//must run before every thing else, (this cant print to serial or screen)
void initmap()
{
	//mem
	parsememmap_uefi(getmemmap());

	//acpi
	parsedevmap_acpi(getdevmap());
}




//we need to debug, init these as soon as possiable
void initdbg()
{
	//debug serial
	struct item* uart = device_alloc_prepobj(_dev_, _uart_, 0, 0);
	initboarddebug(uart);

	//debug framebuffer
	_obj* wnd = supply_alloc_prepobj(0, _wnd_, 0, 0);
	supply_create(wnd, 0, 0, 0);

	//print all pending log
}




//init boot cpu into 64bit mode
void initcpu0()
{
	struct item* p;

	//localapic check
	localapic_check();

	//cpu_bsp: gdt,paging,idt,apic...
	p = device_alloc_prepobj(_dev_, _cpu_, 0, 0);
	initcpu_bsp(p);
}




//init apic and non-boot cpu
void exitarch()
{
}
void initarch()
{
	struct item* p;

	//ioapic or dual8259
	p = device_alloc_prepobj(_dev_, _irq_, 0, 0);
	initirq(p);

	//timer
	p = device_alloc_prepobj(_dev_, _tmr_, 0, 0);
	inittimer();

	//cpu_app: after cpu_bsp and pic and timer
	initcpu_ap();
}




//init soc device: 8042, pci
void exitsoc()
{
}
void initsoc()
{
	struct item* p;

	//input
	init8042();

	//pci
	p = device_alloc_prepobj(_dev_, _pci_, 0, 0);
	initpci_port(p);
	initpci_mmio();
}




//init board device: vm special device, i2c, spi, ...
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
	initmap();
	initdbg();


	initcpu0();
	initarch();
	initsoc();
	initboard();
}
