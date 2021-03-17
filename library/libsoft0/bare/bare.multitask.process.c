#include "libsoft.h"
int parse_pe(void* pe, int len);
void pagetable_use(void* cr3);
void pagetable_makeuser(void* buf, int len, u64 pa, int plen, u64 va, int vlen);




struct procstate{
	volatile void* cr3;		//physical address, page table
	volatile void* code;	//physical address, 2m align
	volatile void* path;
}__attribute__((packed));
static struct procstate table[8];
static int proccount;




void process_malloc(u64* twomega, u64* onemega)
{
	u8* mem = memorycreate(0x300000, 0);
	u64 tmp = (u64)mem;
	if(tmp&0x1fffff){
		*twomega = tmp+0x100000;
		*onemega = tmp;
	}
	else{
		*twomega = tmp;
		*onemega = tmp+0x200000;
	}
}




int processsearch(void* buf, int len)
{
	int j;
	for(j=0;j<8;j++){
		say("%d:cr3=%p,code=%p,path=%p\n", j, table[j].cr3, table[j].code, table[j].path);
	}
	return 0;
}
int processmodify()
{
	return 0;
}
u64 processcreate(void* file, void* args)
{
	say("file=%s\n",file);
	u64 cr,pa,va;
	int now = 1;
	if(0 == table[now].code){
		process_malloc(&pa, &cr);
		table[now].cr3 = (void*)cr;
		table[now].code = (void*)pa;

		va = 0xffffffffffe00000;
		pagetable_makeuser((void*)cr, 0x100000, pa, 0x200000, va, 0x200000);
		pagetable_use((void*)cr);
		printmemory((void*)va, 0x200);
	}

	void* p = (void*)table[now].code;
	int ret = readfile(file,0, 0,0, p,0x10000);

	printmemory(p, 0x100);
	parse_pe(p, 0x10000);

	//pagetable_makeuser(cr3, 0x100000, 0xffffffff00000000, len, p, len);
	proccount = now;

	return 0;
}
void processdelete(u64 h)
{
}




void process_schedule()
{
}
void process_registersupplier(int coreid)
{
	say("incoming process: from coreid=%d\n", coreid);

	table[0].cr3 = (void*)0x40000;
	table[0].path = 0;
	table[0].code = 0;
	proccount = 1;
}




void initprocess()
{
	int j;
	for(j=0;j<8;j++){
		table[j].cr3 = 0;
		table[j].code = 0;
		table[j].path = 0;
	}
	proccount = 0;
}
void freeprocess()
{
}