#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
#define LAPIC_BASE 0xfee00000
//
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
//
void* acpi_getlocalapic();
void* acpi_getirqioapic();
void* acpi_getredirtbl();
//
void printmmio(void*, int);
void printmemory(void*, int);
void say(void*, ...);




static volatile u8* addr_localapic = 0;
static volatile u8* addr_irqioapic = 0;
void localapic_check()
{
	say("@apicwhere\n");

	u8* addr = acpi_getlocalapic();
	if(LAPIC_BASE != (u64)addr)return;

	addr_localapic = (void*)addr;
	say("apic@%p\n", addr_localapic);
}




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

	//printmmio((void*)addr_localapic, 0x400);
	say("lapic: id=%x,ver=%x\n", localapic_coreid(), localapic_version());		//wrong

	//Clear task priority to enable all interrupts
	volatile u32* tmp;
	tmp = (volatile u32*)(addr_localapic + LAPIC_TPR);
	*tmp = 0;

	//flat mode
	tmp = (volatile u32*)(addr_localapic + LAPIC_DFR);
	*tmp = 0xffffffff;

	//all cpu use logical id 1
	tmp = (volatile u32*)(addr_localapic + LAPIC_LDR);
	*tmp = 0x01000000;

	//setup spurious vector + enable apic
	tmp = (volatile u32*)(addr_localapic + LAPIC_SVR);
	*tmp = 0x100 | 0xff;

	//done
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




#define TMR_PERIODIC 0x20000
#define TMR_BASEDIV	(1<<20)
void apictimer_init()
{
	volatile u32* LVT = (volatile u32*)(addr_localapic + LAPIC_LVT_TIMER);
	volatile u32* DIV = (volatile u32*)(addr_localapic + LAPIC_TMR_DIVCONFIG);
	volatile u32* CNT = (volatile u32*)(addr_localapic + LAPIC_TMR_INITCOUNT);

	*LVT = 0x40 | TMR_PERIODIC;
	*DIV = 0x3;
	*CNT = 1000*100;
}




//
#define IOAPIC_BASE 0xfec00000
#define IOAPIC_ADDR 0
#define IOAPIC_DATA 0x10
//[0,7]: vector
//[8,10]: delivery mode
#define REDIRTBL_DELIVER_FIXED  (0<<8)
#define REDIRTBL_DELIVER_LOWPRI (1<<8)
#define REDIRTBL_DELIVER_SMI    (2<<8)
#define REDIRTBL_DELIVER_NMI    (4<<8)
#define REDIRTBL_DELIVER_INIT   (5<<8)
#define REDIRTBL_DELIVER_EXTINT (7<<8)
//bit11
#define REDIRTBL_DEST_PHYSICAL (0<<11)
#define REDIRTBL_DEST_LOGICAL  (1<<11)
//bit12: 0=done, 1=still waiting
#define REDIRTBL_STATUS_DONE      (0<<12)
#define REDIRTBL_STATUS_STILLWAIT (1<<12)
//bit13
#define REDIRTBL_POLARITY_ACTIVEHIGH (0<<13)
#define REDIRTBL_POLARITY_ACTIVELOW  (1<<13)
//bit15
#define REDIRTBL_TRIGGER_EDGE  (0<<15)
#define REDIRTBL_TRIGGER_LEVEL (1<<15)
//bit16
#define REDIRTBL_DISABLE (1<<16)




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
	//redirect table
	u8* redirtbl = acpi_getredirtbl();
	u8 gsi = redirtbl[irq];
	say("redirtbl: isa %d -> gsi %d\n", irq, gsi);

	int off = 0x10 + gsi*2;
	u32 lo32 = ioapic_read(off);
	u32 hi32 = (0<<24);
	lo32 &= ~REDIRTBL_DISABLE;
	lo32 &= 0xffffff00;
	lo32 |= 0x20 + irq;
	ioapic_write(off+0, lo32);
	ioapic_write(off+1, hi32);
}
void ioapic_disableirq(u32 irq)
{
	int off = 0x10 + irq*2;
	u32 lo32 = ioapic_read(off);
	lo32 |= REDIRTBL_DISABLE;
	ioapic_write(off, lo32);
}
void ioapic_init()
{
	int j,off,lo32,hi32;
	u32* addr;
	say("@initioapic\n");

	//register dump
	for(j=0;j<0x10+24*2;j++){
		say(" %08x", ioapic_read(j));
		if(7 == (j%8))say("\n");
	}
	j = ioapic_read(0);
	say("id=%x\n", (j>>24)&0xf);
	j = ioapic_read(1);
	say("ver=%x,cnt=%x\n", j&0xff, (j>>16)&0xff);
	j = ioapic_read(2);
	say("priority=%x\n", (j>>24)&0xf);

	//configure perirq
	addr = (u32*)IOAPIC_BASE;
	for(j=0;j<24;j++)
	{
		off = 0x10 + j*2;
		lo32= (0x20 + j)
			+ REDIRTBL_DELIVER_FIXED
			+ REDIRTBL_DEST_PHYSICAL
			+ REDIRTBL_POLARITY_ACTIVEHIGH
			+ REDIRTBL_TRIGGER_EDGE
			+ REDIRTBL_DISABLE;
		hi32 = (0<<24);
		ioapic_write(off+0, lo32);
		ioapic_write(off+1, hi32);
	}
}
