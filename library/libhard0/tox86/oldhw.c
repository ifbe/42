#include "libhard.h"
void initpaging();
void initgdt();
void initidt();
//
void* getmemmap();
void* getdevmap();
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




void initcpu0(struct device* p)
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




void parsetable(void* buf)
{
	u8* addr = (u8*)(u64)(*(u32*)buf);
	say("%.4s@%p\n", addr, addr);
}
void parsexsdt(void* buf)
{
	int j;
	int len = *(u8*)(buf+4);
	say("xsdt,%x@%p\n", len, buf);
	for(j=0x24;j<len;j+=8)parsetable(buf+j);
}
void parsersdt(void* buf)
{
	int j;
	int len = *(u8*)(buf+4);
	say("rsdt,%x@%p\n", len, buf);

	for(j=0x24;j<len;j+=4)parsetable(buf+j);
}
void initdevmap()
{
	void* buf = getdevmap();
	if(0 == buf)return;

	int len = *(u8*)(buf+4);
	say("rsdptr,%x@%p\n", len, buf);

	if(0 == *(u8*)(buf+0xf)){
		void* rsdt = (void*)(u64)(*(u32*)(buf+0x10));
		parsersdt(rsdt);
	}
	else{
		void* xsdt = (void*)(u64)(*(u32*)(buf+0x18));
		parsexsdt(xsdt);
	}
}




void freehardware()
{
}
void inithardware()
{
	struct device* p;

	p = devicecreate(_cpu_, 0, 0, 0);
	initcpu0(p);

	initmemmap();
	initdevmap();

	p = devicecreate(_pci_, 0, 0, 0);
	initpci_port(p);
	initpci_mmio();

	init8259();
	initapic();

	initrtc();
	init825x();
}
