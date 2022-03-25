#include "libhard.h"
#define Present 1
#define Writable 2
#define Allowuser 4
#define Writethrough 8
#define Dontcache 0x10
#define Accessed 0x20
#define Dirty 0x40
#define Isleaf 0x80
#define Globalpage 0x100
#define Noexecute (1<<63)
//
#define PML4OFFS 0x00000
#define PDPTOFFS 0x01000
#define GIGACOUNT 1024			//1 terabyte
//
#define PDPTUSER 0x40000
#define PDIRUSER 0x41000




u64 pagetable_virt2phys(u64 cr3, u64 va)
{
	//bit48to63 must be signed extension of bit47
	u64 bit47_63 = (va>>47)&0x1ffff;
	if((0 != bit47_63)&&(0x1ffff != bit47_63)){
		return 0;
	}

	//search pml4
	u64* pml4 = (u64*)cr3;
	u64 bit39_47 = (va>>39)&0x1ff;
	u64 val4 = pml4[bit39_47];
	if(0 == (val4 & 1)){
		return 0;
	}

	u64* pdp = (u64*)(val4 & 0x7ffffffffffff000);
	u64 bit30_38 = (va>>30)&0x1ff;
	u64 val3 = pdp[bit30_38];
	if(0 == (val3 & 1)){
		return 0;
	}
	if(val3 & Isleaf){
		return (val3&0x7fffffffc0000000) + (va&0x3fffffff);
	}

	u64* pd = (u64*)(val3 & 0x7ffffffffffff000);
	u64 bit21_29 = (va>>21)&0x1ff;
	u64 val2 = pd[bit21_29];
	if(0 == (val2 & 1)){
		return 0;
	}
	if(val2 & Isleaf){
		return (val2&0x7fffffffffe00000) + (va&0x1fffff);
	}

	u64* pt = (u64*)(val2 & 0x7ffffffffffff000);
	u64 bit12_20 = (va>>12)&0x1ff;
	u64 val1 = pt[bit12_20];
	if(0 == (val1 & 1)){
		return 0;
	}
	return (val1&0x7ffffffffffff000) + (va&0xfff);
}




void pagetable_makekern(u8* buf, int len, u8* tmp, int plen)
{
	//clear memory
	u64 j;
	for(j=0;j<len;j++)buf[j] = 0;
	for(j=0;j<len;j++)tmp[j] = 0;


	//page table: unused


	//page directory: waste 0x1000*62 B, means 62 GB
	//realaddr8B_per_item, 512item_per_table, 62table_we_used
	u64* pdir = (u64*)tmp;
	for(j=0;j<512*GIGACOUNT;j++)pdir[j] = ((j<<21) + 0) | Isleaf | Writable | Present;


	//page directory pointer: waste 0x1000 B, actually 62*8 B
	//pdiraddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pdpt0 = (u64*)(buf+PDPTOFFS);
	u64* pdpt1 = (u64*)(buf+PDPTOFFS+0x1000);
	for(j=0;j<GIGACOUNT;j++)pdpt0[j] = ((j<<12) + (u64)pdir) | Writable | Present;


	//page map level 4: waste 0x1000 B, actually 8B
	//pdptaddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pml4 = (u64*)(buf+PML4OFFS);
	pml4[0] = (u64)pdpt0 | Writable | Present;
	pml4[1] = (u64)pdpt1 | Writable | Present;
}
void pagetable_makeuser(u8* buf, int len, u64 pa, int plen, u64 va, int vlen)
{
	//pagetable_makekern(buf, 0x40000);

	//copy kern
	//whereis va, *where = pa | Allowuser | Writable | Present;
	//u64 bit47_63 = (va>>47)&0x1ffff;		//must 0 or 0x1ffff
	u64 bit39_47 = (va>>39)&0x1ff;
	u64 bit30_38 = (va>>30)&0x1ff;
	u64 bit21_29 = (va>>21)&0x1ff;
	u64 bit12_20 = (va>>12)&0x1ff;
	say("pml4:%d, pml3:%d, pml2:%d, pml1:%d\n",bit39_47, bit30_38, bit21_29, bit12_20);

	u64* pdir = (u64*)(buf+PDIRUSER);
	pdir[bit21_29] = pa | Isleaf | Allowuser | Writable | Present;

	//page directory pointer: waste 0x1000 B, actually 62*8 B
	//pdiraddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pdpt = (u64*)(buf+PDPTUSER);
	pdpt[bit30_38] = (u64)pdir | Allowuser | Writable | Present;

	//page map level 4: waste 0x1000 B, actually 8B
	//pdptaddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pml4 = (u64*)(buf+PML4OFFS);
	pml4[bit39_47] = (u64)pdpt | Allowuser | Writable | Present;
}




void pagetable_readcr3()
{
	u8* old;
	asm __volatile__(
		"mov %%cr3, %%rax\n"
		"mov %%rax, %0\n"
		:
		:"m"(old)
	);
	say("old cr3=%p\n", old);
	printmemory(old, 0x20);
}
void pagetable_use(u8* cr3)
{
	say("@pagetable_writecr3+++\n");
	printmemory(cr3, 0x20);

	//write cr3
	asm __volatile__(
		"mov %0, %%rax\n"
		"mov %%rax, %%cr3\n"
		:
		:"m"(cr3)
	);

	say("@pagetable_writecr3---\n");
}
void initpaging(u8* pagehome, int len, void* pdir, int plen)
{
	say("@initpaging\n");

	pagetable_readcr3();

	pagetable_makekern(pagehome, len, pdir, plen);
	//pagetable_makeuser(pagehome, 0x42000, 0,0, 0,0);

	pagetable_use(pagehome);

	//printmemory((u8*)0xffffffffffe00000, 0x100);
	say("\n\n");
}