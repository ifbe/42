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
	case 0xd0b:return 5;
	}

	return 0;
}
void* mmiobase()
{
	int ver = raspi_version();
	switch(ver) {
	case 1:return (void*)0x20000000;
	case 2:return (void*)0x3F000000;
	case 3:return (void*)0x3F000000;
	case 4:return (void*)0xFE000000;
	case 5:return (void*)0x1c00000000;
	}

	return 0;
}

#define RPI3_UART0 0x201000
#define RPI3_UART1 0x215000		//miniuart
//
#define RPI4_UART0 0x201000
#define RPI4_UART1 0x215000		//miniuart
#define RPI4_UART2 0x201400
#define RPI4_UART3 0x201600
#define RPI4_UART4 0x201800
#define RPI4_UART5 0x201a00
//
#define RPI5_UART0 0x30000
void* uartaddr()
{
	int ver = raspi_version();
	switch(ver) {
	case 1:return (void*)0x20000000;
	case 2:return (void*)0x3F000000;
	case 3:return (void*)0x3F000000 + RPI3_UART0;
	case 4:return (void*)0xFE000000 + RPI4_UART0;
	case 5:return (void*)((u64)0x1c00000000 + RPI5_UART0);
	}

	return 0;
}




void freerunenv()
{
}
void initrunenv()
{
}
