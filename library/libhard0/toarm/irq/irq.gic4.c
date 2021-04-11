#include "libhard.h"
void percputimer_isr(void* regs);


#define GIC4ADDR  0xff840000
#define GICD_BASE 0xFF841000
#define GICC_BASE 0xFF842000
#define GIC_END   0xFF847FFF

// GIC distributor registers
#define GICD_CTLR		(GICD_BASE + 0x000)
	#define GICD_CTLR_DISABLE	(0 << 0)
	#define GICD_CTLR_ENABLE	(1 << 0)
	// secure access
	#define GICD_CTLR_ENABLE_GROUP0	(1 << 0)
	#define GICD_CTLR_ENABLE_GROUP1	(1 << 1)
#define GICD_IGROUPR0		(GICD_BASE + 0x080)		// secure access for group 0
#define GICD_ISENABLER0		(GICD_BASE + 0x100)
#define GICD_ICENABLER0		(GICD_BASE + 0x180)
#define GICD_ISPENDR0		(GICD_BASE + 0x200)
#define GICD_ICPENDR0		(GICD_BASE + 0x280)
#define GICD_ISACTIVER0		(GICD_BASE + 0x300)
#define GICD_ICACTIVER0		(GICD_BASE + 0x380)
#define GICD_IPRIORITYR0	(GICD_BASE + 0x400)
	#define GICD_IPRIORITYR_DEFAULT	0xA0
	#define GICD_IPRIORITYR_FIQ	0x40
#define GICD_ITARGETSR0		(GICD_BASE + 0x800)
	#define GICD_ITARGETSR_CORE0	(1 << 0)
#define GICD_ICFGR0		(GICD_BASE + 0xC00)
	#define GICD_ICFGR_LEVEL_SENSITIVE	(0 << 1)
	#define GICD_ICFGR_EDGE_TRIGGERED	(1 << 1)
#define GICD_SGIR		(GICD_BASE + 0xF00)
	#define GICD_SGIR_SGIINTID__MASK		0x0F
	#define GICD_SGIR_CPU_TARGET_LIST__SHIFT	16
	#define GICD_SGIR_TARGET_LIST_FILTER__SHIFT	24

// GIC CPU interface registers
#define GICC_CTLR		(GICC_BASE + 0x000)
	#define GICC_CTLR_DISABLE	(0 << 0)
	#define GICC_CTLR_ENABLE	(1 << 0)
	// secure access
	#define GICC_CTLR_ENABLE_GROUP0	(1 << 0)
	#define GICC_CTLR_ENABLE_GROUP1	(1 << 1)
	#define GICC_CTLR_FIQ_ENABLE	(1 << 3)
#define GICC_PMR		(GICC_BASE + 0x004)
	#define GICC_PMR_PRIORITY	(0xF0 << 0)
#define GICC_IAR		(GICC_BASE + 0x00C)
	#define GICC_IAR_INTERRUPT_ID__MASK	0x3FF
	#define GICC_IAR_CPUID__SHIFT		10
	#define GICC_IAR_CPUID__MASK		(3 << 10)
#define GICC_EOIR		(GICC_BASE + 0x010)
	#define GICC_EOIR_EOIINTID__MASK	0x3FF
	#define GICC_EOIR_CPUID__SHIFT		10
	#define GICC_EOIR_CPUID__MASK		(3 << 10)




u32 readl(u64 addr)
{
	return *(volatile u32*)addr;
}
void writel(u64 addr, u32 val)
{
	*(volatile u32*)addr = val;
}




void gic4_isr(void* regs)
{
	u32 irq = readl(GICC_IAR);

	if((irq & 0x3ff) == 30) {
		percputimer_isr(regs);
	}

	writel(GICC_EOIR, irq);
}
void gic4_inittimer()
{
	u32 value;

	/*打开Non-secure physical timer中断，具体可以看GIC-400手册*/
	value = readl(GICD_ISENABLER0);
	value |= (1<<30);
	writel(GICD_ISENABLER0, value);
}




void gic4_init()
{
	u32 value;

	/*打开GIC Distributor总中断*/
	value = readl(GICD_CTLR);
	value |= 1;
	writel(GICD_CTLR, value);

	/*打开GIC CPU interface总中断*/
	value = readl(GICC_CTLR);
	value |= 1;
	writel(GICC_CTLR, value);
}
void gic4_exit()
{
}