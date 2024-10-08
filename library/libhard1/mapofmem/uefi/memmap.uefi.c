#include "libhard.h"
struct memdesc{
    u32 Type;
    u32 Pad;
    u64 PhysicalStart;
    u64 VirtualStart;
    u64 NumberOfPages;
    u64 Attribute;
}__attribute((packed));
struct meminfo{
	u64 onebyte;
	u64 allbyte;
	u8 addr[];
}__attribute((packed));
void parsememmap_uefi(struct meminfo* info)
{
	if(0 == info)return;
	printmemory(info, 0x20);

	int j;
	struct memdesc* desc;
	for(j=0;j<info->allbyte;j+=info->onebyte){
		desc = (struct memdesc*)(info->addr + j);
		logtoall("type=%x,pmem=%p,vmem=%p,numpage=%llx,attr=%llx\n",
			desc->Type, desc->PhysicalStart, desc->VirtualStart, desc->NumberOfPages, desc->Attribute
		);
	}

	logtoall("\n\n");
}