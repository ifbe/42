#include "libhard.h"




// granularity
#define PERTABLE    4096
#define PT_PAGE     0b11        // 4k granule
#define PT_BLOCK    0b01        // 2M granule
// accessibility
#define PT_KERNEL   (0<<6)      // privileged, supervisor EL1 access only
#define PT_USER     (1<<6)      // unprivileged, EL0 access allowed
#define PT_RW       (0<<7)      // read-write
#define PT_RO       (1<<7)      // read-only
#define PT_AF       (1<<10)     // accessed flag
#define PT_NX       (1UL<<54)   // no execute
// shareability
#define PT_OSH      (2<<8)      // outter shareable
#define PT_ISH      (3<<8)      // inner shareable
// defined in MAIR register
#define PT_MEM      (0<<2)      // normal memory
#define PT_DEV      (1<<2)      // device MMIO
#define PT_NC       (2<<2)      // non-cachable
//
#define TTBR_ENABLE 1
//
#define POSITIVE_PML3 0x00000
#define POSITIVE_PML2 0x01000
#define POSITIVE_LAST 0x40000
//
#define NEGATIVE_PML3 0x40000
#define NEGATIVE_PML2 0x41000
#define NEGATIVE_LAST 0x42000
//
#define GIGACOUNT 8		//map 8GB




void pagetable_makekern(u8* buf, int len)
{
	u64 j;
	for(j=0;j<len;j++)buf[j] = 0;

	u64* pml2 = (u64*)(buf+POSITIVE_PML2);
	for(j=0;j<512*GIGACOUNT;j++){
		pml2[j] = ((j<<21) + 0) | PT_BLOCK | PT_AF | PT_KERNEL;
		if(j < 512)pml2[j] |= PT_ISH | PT_MEM;
		else pml2[j] |= PT_OSH | PT_DEV;
	}

	u64* pml3 = (u64*)(buf+POSITIVE_PML3);
	for(j=0;j<GIGACOUNT;j++)pml3[j] = ((j<<12) + (u64)pml2) | PT_PAGE | PT_AF | PT_KERNEL | PT_ISH | PT_MEM;
}
void pagetable_makeuser(u8* buf, int len)
{
	pagetable_makekern(buf, 0x40000);

	u64 j;
	for(j=NEGATIVE_PML3;j<NEGATIVE_LAST;j++)buf[j] = 0;

	u64* pml2 = (u64*)(buf+NEGATIVE_PML2);
	pml2[511] = 0 | PT_BLOCK | PT_AF | PT_KERNEL | PT_ISH | PT_MEM;

	u64* pml3 = (u64*)(buf+NEGATIVE_PML3);
	pml3[511] = (u64)pml2 | PT_PAGE | PT_AF | PT_KERNEL | PT_ISH | PT_MEM;
}




void pagetable_use(u64 p0, u64 p1)
{
}
u64 pagetable_get()
{
	u64 addr;
	asm("mrs %0, ttbr1_el1" : "=r"(addr) );
	return addr;
}
void pagetable_enable(u64 p0, u64 p1)
{
    unsigned long r, b;

    // check for 4k granule and at least 36 bits physical address bus
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28) || b<1) {
        say("ERROR: 4k granule or 36 bit address space not supported\n");
        return;
    }

    // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our example
    r=  (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x44 <<16);     // AttrIdx=2: non cacheable
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    // next, specify mapping characteristics in translate control register
    r=  (0b00LL << 37) | // TBI=0, no tagging
        (b << 32) |      // IPS=autodetected
        (0b10LL << 30) | // TG1=4k
        (0b11LL << 28) | // SH1=3 inner
        (0b01LL << 26) | // ORGN1=1 write back
        (0b01LL << 24) | // IRGN1=1 write back
        (0b0LL  << 23) | // EPD1 enable higher half
        (25LL   << 16) | // T1SZ=25, 3 levels (512G)
        (0b00LL << 14) | // TG0=4k
        (0b11LL << 12) | // SH0=3 inner
        (0b01LL << 10) | // ORGN0=1 write back
        (0b01LL << 8) |  // IRGN0=1 write back
        (0b0LL  << 7) |  // EPD0 enable lower half
        (25LL   << 0);   // T0SZ=25, 3 levels (512G)
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));

    // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // lower half, user space
    asm volatile ("msr ttbr0_el1, %0" : : "r" (p0 | 1));
    // upper half, kernel space
    asm volatile ("msr ttbr1_el1, %0" : : "r" (p1 | 1));

	u64 sctlr;
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (sctlr));
    sctlr |= 0xC00800;     // set mandatory reserved bits
    sctlr &= ~((1<<25) |   // clear EE, little endian translation tables
         (1<<24) |   // clear E0E
         (1<<19) |   // clear WXN
         (1<<12) |   // clear I, no instruction cache
         (1<<4) |    // clear SA0
         (1<<3) |    // clear SA
         (1<<2) |    // clear C, no cache at all
         (1<<1));    // clear A, no aligment check
    sctlr |=  (1<<0);     // set M, enable MMU
    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (sctlr));
}
void pagetable_disable()
{
}
void initpaging(void* addr)
{
	say("@initpaging: new@%p\n", addr);


	//pagetable_makekern(addr, POSITIVE_LAST);
	pagetable_makeuser(addr, NEGATIVE_LAST);


	pagetable_use((u64)addr, (u64)addr+NEGATIVE_PML3);
	pagetable_enable((u64)addr, (u64)addr+NEGATIVE_PML3);
}