#include "libhard.h"




#define FDT_BEGIN_NODE 1
#define FDT_END_NODE   2
#define FDT_PROP       3
#define FDT_NOP        4
#define FDT_END        9
struct fdt_header{
	u32            magic;	//[00,03]: d0,0d,fe,ed
	u32        totalsize;	//[04.07]
	u32    off_dt_struct;	//[08,0b]
	u32   off_dt_strings;	//[0c,0f]
	u32   off_mem_rsvmap;	//[10,13]
	u32 version_thisfile;	//[14,17] 
	u32 version_lastcomp;	//[18,1b]
	u32  boot_cpuid_phys;	//[1c,1f]
	u32  size_dt_strings;	//[20,23]
	u32   size_dt_struct;	//[24,27]
}__attribute__((packed));	//big endian
struct fdt_reserve_entry{
	u64 addr;
	u64 size;
}__attribute__((packed));	//big endian, terminate when (0==addr)&&(0==size)




static u32 be2le(u32 x)
{
	u32 _07_00 = (x>>24)&0xff;
	u32 _15_08 = (x>> 8)&0xff00;
	u32 _23_16 = (x<< 8)&0xff0000;
	u32 _31_24 = (x<<24)&0xff000000;
	return _31_24 | _23_16 | _15_08 | _07_00;
}
void parsedevmap_dtb(struct fdt_header* h)
{
	if(0 == h){
		say("[dt]wrong addr\n");
		return;
	}
	if(0xedfe0dd0 != h->magic){
		say("[dt]wrong magic\n");
		return;
	}
	printmemory(h,sizeof(struct fdt_header));
	say("[dt]magic=%x,total=%x\n",   be2le(h->magic),            be2le(h->totalsize));
	say("[dt]ver:this=%x,comp=%x\n", be2le(h->version_thisfile), be2le(h->version_lastcomp));
	say("[dt]dat:off=%x,len=%x\n",   be2le(h->off_dt_struct),    be2le(h->size_dt_struct));
	say("[dt]str:off=%x,len=%x\n",   be2le(h->off_dt_strings),   be2le(h->size_dt_strings));
	say("[dt]mem:off=%x\n",          be2le(h->off_mem_rsvmap));
	say("[dt]cpu:phy=%x\n",          be2le(h->boot_cpuid_phys));
}