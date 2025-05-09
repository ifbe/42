#include "libhard.h"
//
u32 memmap_type();
void* memmap_addr();
void parsememmap_bios(void*);
void parsememmap_uefi(void*);
//
u32 devmap_type();
void* devmap_addr();
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
void lowlevelserial_disable();
//
void initvmtool();




//must run before every thing else, (this cant print to serial or screen)
void initmap()
{
	//memmap
	u32 mt = memmap_type();
	if(_bios_ == mt){
		struct item* memmap = driver_alloc_fromtype(_memmap_);
		parsememmap_bios(memmap_addr());
	}
	else if(_efi_ == mt){
		struct item* memmap = driver_alloc_fromtype(_memmap_);
		parsememmap_uefi(memmap_addr());
	}

	//devmap
	u32 dt = devmap_type();
	if(_acpi_ == dt){
		struct item* devmap = driver_alloc_fromtype(_devmap_);
		parsedevmap_acpi(devmap_addr());
	}
	else if(_dtb_ == dt){
		struct item* devmap = driver_alloc_fromtype(_devmap_);
		parsedevmap_acpi(devmap_addr());
	}
}




//we need to debug, init these as soon as possiable
void initdbg()
{
	//debug serial
	struct item* uart = device_alloc_fromtype(_uart_);
	initboarddebug(uart);
	lowlevelserial_disable();

	//debug framebuffer
	_obj* wnd = supply_alloc_fromtype(_wnd_);
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
	p = device_alloc_fromtype(_cpu_);
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
	p = device_alloc_fromtype(_irq_);
	initirq(p);

	//timer
	p = device_alloc_fromtype(_tmr_);
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
	p = device_alloc_fromtype(_pci_);
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
