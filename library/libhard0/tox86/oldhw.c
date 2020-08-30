void initgdt();
void initidt();
//
void initpci();
//
void init8259();        //interrupt controller
//
void init825x();        //timer.pit
void initrtc();         //timer.rtc
//
void say(void*, ...);




void freehardware()
{
}
void inithardware()
{
	asm("cli");

	initgdt();
	initidt();

	initpci();

	init8259();

	init825x();
	initrtc();

	asm("sti");
	asm("int3");
	asm("int $0x80");
}
