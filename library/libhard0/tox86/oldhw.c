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

	initpci_port();
	initpci_mmio();

	init8259();
	initapic();

	init825x();
	initrtc();

	asm("sti");
	asm("int3");
	asm("int $0x80");
}
