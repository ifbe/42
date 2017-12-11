#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void out8(u16 port, u8 data);
void printmemory(void*, int);
void say(void*, ...);




void init8259()
{
	say("@init8259\n");

	asm("cli");

	//icw1
	out8(0x20, 0x11);
	out8(0xa0, 0x11);

	//icw2
	out8(0x21, 0x20);
	out8(0xa1, 0x28);

	//icw3
	out8(0x21, 0x4);
	out8(0xa1, 0x2);

	//icw4
	out8(0x21, 0x1);
	out8(0xa1, 0x1);

	//
	out8(0x21, 0xfd);
	out8(0xa1, 0xff);
}
