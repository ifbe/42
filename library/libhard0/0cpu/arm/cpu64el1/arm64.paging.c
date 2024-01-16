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
#define GIGACOUNT 32		//map 32GB




u64 pagetable_virt2phys(u64 tb, u64 va)
{
	u64* root;
    if(0 == (va>>63))root = (void*)tb;
    else root = (void*)(tb+NEGATIVE_PML3);

	u64 bit30_38 = (va>>30)&0x1ff;
	u64 val3 = root[bit30_38];
	if(0 == val3){
		return 0;
	}
	if(PT_BLOCK == (val3 & 3)){
		return (val3&0x7fffffffc0000000) + (va&0x3fffffff);
	}

	u64* pd = (u64*)(val3 & 0x7ffffffffffff000);
	u64 bit21_29 = (va>>21)&0x1ff;
	u64 val2 = pd[bit21_29];
	if(0 == val2){
		return 0;
	}
	if(PT_BLOCK == (val2 & 3)){
		return (val2&0x7fffffffffe00000) + (va&0x1fffff);
	}

	u64* pt = (u64*)(val2 & 0x7ffffffffffff000);
	u64 bit12_20 = (va>>12)&0x1ff;
	u64 val1 = pt[bit12_20];
	if(0 == val1){
		return 0;
	}
	return (val1&0x7ffffffffffff000) + (va&0xfff);
}




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
void pagetable_makeuser(u8* buf, int len, u64 pa, int plen, u64 va, int vlen)
{
	pagetable_makekern(buf, 0x40000);

	u64 j;
	for(j=NEGATIVE_PML3;j<NEGATIVE_LAST;j++)buf[j] = 0;

	u64* pml2 = (u64*)(buf+NEGATIVE_PML2);
	pml2[511] = pa | PT_BLOCK | PT_AF | PT_USER | PT_ISH | PT_MEM;

	u64* pml3 = (u64*)(buf+NEGATIVE_PML3);
	pml3[511] = (u64)pml2 | PT_PAGE | PT_AF | PT_USER | PT_ISH | PT_MEM;
}




void pagetable_use(void* addr)
{
    // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    u64 ttbr0 = (u64)addr;
    u64 ttbr1 = (u64)addr+NEGATIVE_PML3;

    // lower half
    asm volatile ("msr ttbr0_el1, %0" : : "r" (ttbr0 | 1));
    // upper half
    asm volatile ("msr ttbr1_el1, %0" : : "r" (ttbr1 | 1));
}
void pagetable_get(u64* ttbr0, u64* ttbr1)
{
	asm volatile("mrs %0, ttbr0_el1" : "=r"(*ttbr0) );
	asm volatile("mrs %0, ttbr1_el1" : "=r"(*ttbr1) );
}




void pagetable_enable()
{
    unsigned long r, b;

    // check for 4k granule and at least 36 bits physical address bus
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28) || b<1) {
        logtoall("ERROR: 4k granule or 36 bit address space not supported\n");
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
	logtoall("@initpaging: table@%p\n", addr);

	pagetable_use(addr);
	pagetable_enable();
}
void exitpaging()
{
}