#include "libhard.h"
void initgdt();
void initidt();
//
void initpci_port();
void initpci_mmio();
//
void init8259();        //interrupt controller
void initapic();
//
void init825x();        //timer.pit
void initrtc();         //timer.rtc




void initcpu0(struct device* p)
{
	asm("cli");

	initgdt();
	initidt();

	asm("sti");
	asm("int3");
	asm("int $0x80");
}




void freehardware()
{
}
void inithardware()
{
	struct device* p;

	p = devicecreate(_cpu_, 0, 0, 0);
	initcpu0(p);

	p = devicecreate(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();

	init8259();
	initapic();

	init825x();
	initrtc();
}
