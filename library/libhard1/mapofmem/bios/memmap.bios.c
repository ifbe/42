#include "libhard.h"



struct meminfo{
    u64 addr;
    u64 size;
    u64 attr;
}__attribute__((packed));
void parsememmap_bios(void* p)
{
    printmemory(p, 0x40);

    int j;
    struct meminfo* inf;
    for(j=0;j<16;j++){
        inf = (struct meminfo*)(p + 0x20*j);
        logtoall("addr=%p,size=%p,attr=%p\n", inf->addr, inf->size, inf->attr);
    }
}