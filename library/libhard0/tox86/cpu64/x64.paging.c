#include "libhard.h"
//[0x40000, 0x80000)
#define PAGEHOME 0x40000
#define PDIRADDR PAGEHOME
#define PDPTADDR (PAGEHOME+0x3e000)
#define PML4ADDR (PAGEHOME+0x3f000)
#define COUNT (64-2)



void pagetable_make()
{
	//clear memory
	u64 j;
	u8* buf = (u8*)PAGEHOME;
	for(j=0;j<0x40000;j++)buf[j] = 0;


	//page table: unused


	//page directory: waste 0x1000*62 B, means 62 GB
	//realaddr8B_per_item, 512item_per_table, 62table_we_used
	u64* pdir = (u64*)PDIRADDR;
	for(j=0;j<512*COUNT;j++)pdir[j] = ((j<<21) + 0) | 0x87;


	//page directory pointer: waste 0x1000 B, actually 62*8 B
	//pdiraddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pdpt = (u64*)PDPTADDR;
	for(j=0;j<COUNT;j++)pdpt[j] = ((j<<12) + (u64)pdir) | 7;


	//page map level 4: waste 0x1000 B, actually 8B
	//pdptaddr8B_per_item, 512item_per_table, 1table_cant_less
	u64* pml4 = (u64*)PML4ADDR;
	pml4[0] = (u64)pdpt | 7;
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

	pagetable_make();

	pagetable_use();
}