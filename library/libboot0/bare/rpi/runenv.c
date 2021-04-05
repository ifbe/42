#include "libboot.h"
int miniuart_getc();




static struct event ev[2];
void* pollenv()
{
	int ret = miniuart_getc();
	if(ret > 0xff)return 0;

	ev->why = ret;
	ev->what = _char_;
	return ev;
}




static void* dtb = 0;
void* getdtb()
{
	return dtb;
}
void setdtb(void* p)
{
	dtb = p;
}




int raspi_version()
{
	u32 reg;
	asm volatile ("mrs %0, midr_el1" : "=r" (reg));
	//asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));

	switch((reg >> 4) & 0xFFF) {
	case 0xB76:return 1;
	case 0xC07:return 2;
	case 0xD03:return 3;
	case 0xD08:return 4;
	}

	return 0;
}
void* mmiobase()
{
	u32 reg;
	asm volatile ("mrs %0, midr_el1" : "=r" (reg));
	//asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));

	switch((reg >> 4) & 0xFFF) {
	case 0xB76:return (void*)0x20000000;	//1
	case 0xC07:return (void*)0x3F000000;	//2
	case 0xD03:return (void*)0x3F000000;	//3
	case 0xD08:return (void*)0xFE000000;	//4
	}

	return 0;
}




void freerunenv()
{
}
void initrunenv()
{
}
