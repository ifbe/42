#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
#define LAPIC_BASE 0xfee00000
#define LAPIC_ID      0x0020	//Local APIC ID
#define LAPIC_VER     0x0030	//Local APIC Version
#define LAPIC_TPR     0x0080	//Task Priority
#define LAPIC_APR     0x0090	//Arbitration Priority
#define LAPIC_PPR     0x00a0	//Processor Priority
#define LAPIC_EOI     0x00b0	//EOI
#define LAPIC_RRD     0x00c0	//Remote Read
#define LAPIC_LDR     0x00d0	//Logical Destination
#define LAPIC_DFR     0x00e0	//Destination Format
#define LAPIC_SVR     0x00f0	//Spurious Interrupt Vector
#define LAPIC_ISR     0x0100	//In-Service (8 registers)
#define LAPIC_TMR     0x0180	//Trigger Mode (8 registers)
#define LAPIC_IRR     0x0200	//Interrupt Request (8 registers)
#define LAPIC_ESR     0x0280	//Error Status
#define LAPIC_ICRLO   0x0300	//Interrupt Command
#define LAPIC_ICRHI   0x0310	//Interrupt Command [63:32]
#define LAPIC_TIMER   0x0320	//LVT Timer
#define LAPIC_THERMAL 0x0330	//LVT Thermal Sensor
#define LAPIC_PERF    0x0340	//LVT Performance Counter
#define LAPIC_LINT0   0x0350	//LVT LINT0
#define LAPIC_LINT1   0x0360	//LVT LINT1
#define LAPIC_ERROR   0x0370	//LVT Error
#define LAPIC_TICR    0x0380	//Initial Count (for Timer)
#define LAPIC_TCCR    0x0390	//Current Count (for Timer)
#define LAPIC_TDCR    0x03e0	//Divide Configuration (for Timer)
//
#define IOAPIC_BASE 0xfec00000
u8 in8(u16 port);
void out8(u16 port, u8 data);
void printmemory(void*, int);
void say(void*, ...);




void endofirq(u32 num)
{
	u32* addr = (u32*)(LAPIC_BASE + LAPIC_EOI);
	*addr = 0;
}
void initapic()
{
	u32* addr;
	say("@initapic\n");

	asm("cli");

	//task priority = 0;
	addr = (u32*)(LAPIC_BASE + LAPIC_TPR);
	*addr = 0;

	//flat mode
	addr = (u32*)(LAPIC_BASE + LAPIC_DFR);
	*addr = 0xffffffff;

	//all cpu use logical id 1
	addr = (u32*)(LAPIC_BASE + LAPIC_LDR);
	*addr = 0x01000000;

	//timer interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_TIMER);
	*addr = 0x10000;

	//performance counter interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_PERF);
	*addr = 0x10000;

	//local interrupt0(normal external interrupt) = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LINT0);
	*addr = 0x8700;

	//local interrupt1(normal nmi processing) = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LINT1);
	*addr = 0x400;

	//error interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_ERROR);
	*addr = 0x10000;

	//disable superious vector + enable apic
	addr = (u32*)(LAPIC_BASE + LAPIC_SVR);
	*addr = 0x1ff;
}




void enableirq(u32 irq)
{
}
void disableirq(u32 irq)
{
}
void initioapic()
{
	int j;
	u32* addr;
	say("@initioapic\n");

	asm("cli");
	out8(0x21, 0xfd);
	out8(0xa1, 0xff);

	addr = (u32*)IOAPIC_BASE;
	for(j=0;j<24;j++)
	{
		addr[0] = 0x10 + (j*2);
		addr[4] = 0x20 + j;
		addr[0] = 0x11 + (j*2);
		addr[4] = 0x10000;
	}
}
