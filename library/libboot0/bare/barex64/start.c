#include "libboot.h"
/*
void initpage();
void initgdt();
void initidt();
void initacpi();
//
void init8259();
void initapic();
void initioapic();
//
void init8042();
void initrtc();
void initpci();
//
void createserial();
void main(int, char**);




void start()
{
	main(0, 0);
}*/




void start()
{
	pwrclkcreate(_start_, 0, 0, 0);
	workercreate(_loop_, 0, 0, 0);
}
void* pollenv()
{
	return 0;
}




/*
void baseenvcreate(void* addr)
{
	initpage();
	initgdt();
	initidt();
	initacpi();

	init8259();
	//initapic();
	//initioapic();

	init8042();
	initrtc();
	initpci();

	asm("sti");
}
*/
