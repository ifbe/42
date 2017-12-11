#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define apicbase 0xfee00000
#define taskpriority 0x80
#define destination 0xe0
#define spurious 0xf0
#define timerintr 0x320
#define perfintr 0x340
#define localintr0 0x350
#define localintr1 0x360
#define errorintr 0x370
void say(void*, ...);
void printmemory(void*, int);




void initapic()
{
	u32* addr;
	say("@initapic\n");

	asm("cli");

	//task priority = 0;
	addr = (u32*)(apicbase + taskpriority);
	*addr = 0;

	//timer interrupt = disable
	addr = (u32*)(apicbase + timerintr);
	*addr = 0x10000;

	//performance counter interrupt = disable
	addr = (u32*)(apicbase + perfintr);
	*addr = 0x10000;

	//local interrupt0(normal external interrupt) = disable
	addr = (u32*)(apicbase + localintr0);
	*addr = 0x8700;

	//local interrupt1(normal nmi processing) = disable
	addr = (u32*)(apicbase + localintr1);
	*addr = 0x400;

	//error interrupt = disable
	addr = (u32*)(apicbase + errorintr);
	*addr = 0x10000;

	//disable superious vector + enable apic
	addr = (u32*)(apicbase + spurious);
	*addr = 0x170;
}
