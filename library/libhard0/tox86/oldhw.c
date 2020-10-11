#include "libhard.h"
void initpaging();
void initgdt();
void initidt();
//
void* getmemmap();
void* getdevmap();
void initacpi(void*);
//
void initpci_port();
void initpci_mmio();
//
void init8259();        //interrupt controller
void initapic();
//
void init825x();        //timer.pit
void initrtc();         //timer.rtc
//for fun
void enterring3();




void initcpu0(struct item* p)
{
	//
	asm("cli");
	initpaging();
	initgdt();
	initidt();


	//--------jump to ring3---------
	say("ring3 try...\n");
	enterring3();
	dbg("ring3 god!!!\n");


	//--------back to ring0---------
	dbg("ring0 try...\n");
	asm("int $0x80");
	say("ring0 god!!!\n");


	//ok
	asm("sti");
}




void initmemmap()
{
	void* p = getmemmap();
}




void freehardware()
{
}
void inithardware()
{
	struct item* p;

	p = devicecreate(_cpu_, 0, 0, 0);
	initcpu0(p);

	init8259();
	initapic();

	initrtc();
	init825x();

	initmemmap();
	initacpi(getdevmap());

	p = devicecreate(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();
}
