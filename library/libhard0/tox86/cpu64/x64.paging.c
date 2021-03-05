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
//[0x40000, 0x80000)
#define PAGEHOME 0x40000
#define PDIRADDR PAGEHOME
#define PDPTADDR (PAGEHOME+0x3e000)
#define PML4ADDR (PAGEHOME+0x3f000)
#define COUNT (64-2)



u64 pagetable_virt2phys(u64 va)
{
	//bit48to63 must be signed extension of bit47
	u64 bit47_63 = (va>>47)&0x1ffff;
	if((0 != bit47_63)&&(0x1ffff != bit47_63)){
		return 0;
	}

	//search pml4
	u64* pml4 = (u64*)PML4ADDR;
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




void pagetable_makeuser(u64* buf, int len, u64 va, int vlen, u64 pa, int plen)
{
	//copy kern
	//whereis va, *where = pa | Allowuser | Writable | Present;
}
void pagetable_makekern()
{
	//clear memory
	u64 j;
	u8* buf = (u8*)PAGEHOME;
	for(j=0;j<0x40000;j++)buf[j] = 0;


	//page table: unused


	//page directory: waste 0x1000*62 B, means 62 GB
	//realaddr8B_per_item, 512item_per_table, 62table_we_used
	u64* pdir = (u64*)PDIRADDR;
	for(j=0;j<512*COUNT;j++)pdir[j] = ((j<<21) + 0) | Isleaf | Writable | Present;


	//page directory pointer: waste 0x1000 B, actually 62*8 B
	//pdiraddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pdpt = (u64*)PDPTADDR;
	for(j=0;j<COUNT;j++)pdpt[j] = ((j<<12) + (u64)pdir) | Writable | Present;


	//page map level 4: waste 0x1000 B, actually 8B
	//pdptaddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pml4 = (u64*)PML4ADDR;
	pml4[0] = (u64)pdpt | Writable | Present;
}




void pagetable_use()
{
	//write cr3
	asm __volatile__(
		"mov $0x7f000, %rax\n"
		"mov %rax, %cr3\n"
	);
}
void initpaging()
{
	say("@initpaging\n");

	pagetable_makekern();

	pagetable_use();
}