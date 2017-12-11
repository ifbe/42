#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define ioapicbase 0xfec00000
void out8(u16 port, u8 data);
void printmemory(void*, int);
void say(void*, ...);




void initioapic()
{
	int j;
	u32* addr;
	say("@initioapic\n");

	asm("cli");
	out8(0x21, 0xfd);
	out8(0xa1, 0xff);

	addr = (u32*)ioapicbase;
	for(j=0;j<24;j++)
	{
		addr[0] = 0x10 + (j*2);
		addr[4] = 0x20 + j;
		addr[0] = 0x11 + (j*2);
		addr[4] = 0x10000;
	}
}
