#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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
void say(char*,...);




void driverread()
{
}
void driverwrite()
{
}
void driverlist()
{
	say("empty driver\n");
}
void driverchoose()
{
}
void driverstart()
{
}
void driverstop()
{
}
void drivercreate(void* addr)
{
	//say("[4,8):createed driver\n");
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
void driverdelete()
{
	//say("[4,8):deleteing driver\n");
}
