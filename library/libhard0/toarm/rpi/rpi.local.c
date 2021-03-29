

#define ARM_LOCAL 0xff800000
//
#define ARM_CONTROL      0x00	//ARM Timer and AXI Error IRQ control
#define CORE_IRQ_CONTROL 0x0c	//VideoCore Interrupt Control
#define PMU_CONTROL_SET  0x10	//PMU Bit Set
#define PMU_CONTROL_CLR  0x14	//PMU Bit Clear
#define PERI_IRQ_ROUTE0  0x24	//Peripheral Interrupt Routing (Bank 0)
#define AXI_QUIET_TIME   0x30	//AXI Outstanding Transaction Time and IRQ Control
#define LOCAL_TIMER_CTL  0x34	//Local Timer Control
#define LOCAL_TIMER_IRQ  0x38	//Local Timer Reload and Interrupt
//
#define TIMER_CNTRL0 0x40	//Timer Interrupt Control for ARM Core 0
#define TIMER_CNTRL1 0x44	//Timer Interrupt Control for ARM Core 1
#define TIMER_CNTRL2 0x48	//Timer Interrupt Control for ARM Core 2
#define TIMER_CNTRL3 0x4c	//Timer Interrupt Control for ARM Core 3
//
#define MAILBOX_CNTRL0 0x50	//Mailbox Interrupt Control for ARM Core 0
#define MAILBOX_CNTRL1 0x54	//Mailbox Interrupt Control for ARM Core 1
#define MAILBOX_CNTRL2 0x58	//Mailbox Interrupt Control for ARM Core 2
#define MAILBOX_CNTRL3 0x5c	//Mailbox Interrupt Control for ARM Core 3
//
#define IRQ_SOURCE0 0x60	//IRQ Source flags for ARM Core 0
#define IRQ_SOURCE1 0x64	//IRQ Source flags for ARM Core 1
#define IRQ_SOURCE2 0x68	//IRQ Source flags for ARM Core 2
#define IRQ_SOURCE3 0x6c	//IRQ Source flags for ARM Core 3
//
#define FIQ_SOURCE0 0x70	//FIQ Source flags for ARM Core 0
#define FIQ_SOURCE1 0x74	//FIQ Source flags for ARM Core 1
#define FIQ_SOURCE2 0x78	//FIQ Source flags for ARM Core 2
#define FIQ_SOURCE3 0x7c	//FIQ Source flags for ARM Core 3