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
#define LAPIC_LVT_TIMER   0x0320	//LVT Timer
#define LAPIC_LVT_THERMAL 0x0330	//LVT Thermal Sensor
#define LAPIC_LVT_PERF    0x0340	//LVT Performance Counter
#define LAPIC_LVT_LINT0   0x0350	//LVT LINT0
#define LAPIC_LVT_LINT1   0x0360	//LVT LINT1
#define LAPIC_LVT_ERROR   0x0370	//LVT Error
#define LAPIC_TMR_INITCOUNT    0x0380	//Initial Count (for Timer)
#define LAPIC_TMR_CURRCOUNT    0x0390	//Current Count (for Timer)
#define LAPIC_TMR_DIVCONFIG    0x03e0	//Divide Configuration (for Timer)
// Delivery Mode
#define ICR_FIXED                       0x00000000
#define ICR_LOWEST                      0x00000100
#define ICR_SMI                         0x00000200
#define ICR_NMI                         0x00000400
#define ICR_INIT                        0x00000500
#define ICR_STARTUP                     0x00000600
// Destination Mode
#define ICR_PHYSICAL                    0x00000000
#define ICR_LOGICAL                     0x00000800
// Delivery Status
#define ICR_IDLE                        0x00000000
#define ICR_SEND_PENDING                0x00001000
// Level
#define ICR_DEASSERT                    0x00000000
#define ICR_ASSERT                      0x00004000
// Trigger Mode
#define ICR_EDGE                        0x00000000
#define ICR_LEVEL                       0x00008000
// Destination Shorthand
#define ICR_NO_SHORTHAND                0x00000000
#define ICR_SELF                        0x00040000
#define ICR_ALL_INCLUDING_SELF          0x00080000
#define ICR_ALL_EXCLUDING_SELF          0x000c0000
// Destination Field
#define ICR_DESTINATION_SHIFT           24
// Timer
#define TMR_PERIODIC 0x20000
#define TMR_BASEDIV	(1<<20)
//
#define IOAPIC_BASE 0xfec00000
#define IOAPIC_ADDR 0
#define IOAPIC_DATA 0x10
//
void* getlocalapic();
void* gettheioapic();
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void printmmio(void*, int);
void printmemory(void*, int);
void say(void*, ...);




static volatile u8* addr_localapic = 0;
static volatile u8* addr_theioapic = 0;
int localapic_isenabled()
{
	if(0 == addr_localapic)return 0;
	return 1;
}
int localapic_coreid()
{
	volatile u32* apicid = (volatile u32*)(addr_localapic + LAPIC_ID);
	return (*apicid)>>24;
}
int localapic_version()
{
	volatile u32* apicid = (volatile u32*)(addr_localapic + LAPIC_VER);
	return *apicid;
}
void localapic_endofirq(u32 num)
{
	volatile u32* addr = (volatile u32*)(addr_localapic + LAPIC_EOI);
	*addr = 0;
}




void apictimer_init()
{
	volatile u32* LVT = (volatile u32*)(addr_localapic + LAPIC_LVT_TIMER);
	volatile u32* DIV = (volatile u32*)(addr_localapic + LAPIC_TMR_DIVCONFIG);
	volatile u32* CNT = (volatile u32*)(addr_localapic + LAPIC_TMR_INITCOUNT);

	*LVT = 0x40 | TMR_PERIODIC;
	*DIV = 0x3;
	*CNT = 0x1000000;
}




u32 localapic_read(u32 reg)
{
	volatile u32* addr = (volatile u32*)(addr_localapic + reg);
    return *addr;
}
void localapic_write(u32 reg, u32 data)
{
	volatile u32* addr = (volatile u32*)(addr_localapic + reg);
    *addr = data;
	say("write %x to %p\n", data, addr);
}
void localapic_sendinit(u32 apic_id)
{
	say("apic@%p\n",addr_localapic);
    localapic_write(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
    localapic_write(LAPIC_ICRLO, ICR_INIT | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);
    while(localapic_read(LAPIC_ICRLO) & ICR_SEND_PENDING);
}
void localapic_sendstart(u32 apic_id, u32 vector)
{
    localapic_write(LAPIC_ICRHI, apic_id << ICR_DESTINATION_SHIFT);
    localapic_write(LAPIC_ICRLO, vector | ICR_STARTUP | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);
    while(localapic_read(LAPIC_ICRLO) & ICR_SEND_PENDING);
}
void localapic_init()
{
	say("@initapic\n");

	u8* addr = getlocalapic();
	if(LAPIC_BASE != (u64)addr)return;

	printmmio(addr, 0x400);
	say("lapic: id=%x,ver=%x\n", localapic_coreid(), localapic_version());

	//Clear task priority to enable all interrupts
	volatile u32* tmp;
	tmp = (volatile u32*)(addr + LAPIC_TPR);
	*tmp = 0;

	//flat mode
	tmp = (volatile u32*)(addr + LAPIC_DFR);
	*tmp = 0xffffffff;

	//all cpu use logical id 1
	tmp = (volatile u32*)(addr + LAPIC_LDR);
	*tmp = 0x01000000;

	//setup spurious vector + enable apic
	tmp = (volatile u32*)(addr + LAPIC_SVR);
	*tmp = 0x100 | 0xff;

	//done
	addr_localapic = (void*)addr;
	say("\n\n");
}
/*
	//timer interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LVT_TIMER);
	*addr = 0x10000;

	//performance counter interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LVT_PERF);
	*addr = 0x10000;

	//local interrupt0(normal external interrupt) = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LVT_LINT0);
	*addr = 0x8700;

	//local interrupt1(normal nmi processing) = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LVT_LINT1);
	*addr = 0x400;

	//error interrupt = disable
	addr = (u32*)(LAPIC_BASE + LAPIC_LVT_ERROR);
	*addr = 0x10000;
*/




u32 ioapic_read(u8 reg)
{
	volatile u32* addr = (volatile u32*)(IOAPIC_BASE + IOAPIC_ADDR);
	volatile u32* data = (volatile u32*)(IOAPIC_BASE + IOAPIC_DATA);
	*addr = reg;
	return *data;
}
void ioapic_write(u8 reg, u32 val)
{
	volatile u32* addr = (volatile u32*)(IOAPIC_BASE + IOAPIC_ADDR);
	volatile u32* data = (volatile u32*)(IOAPIC_BASE + IOAPIC_DATA);
	*addr = reg;
	*data = val;
}
void ioapic_enableirq(u32 irq)
{
}
void ioapic_disableirq(u32 irq)
{
}
void ioapic_init()
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
