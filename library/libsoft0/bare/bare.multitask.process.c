#include "libsoft.h"




struct procstate{
	volatile u64* cr3;
	volatile void* code;
	volatile void* path;
}__attribute__((packed));
static struct procstate table[8];




int processsearch(void* buf, int len)
{
	int j;
	for(j=0;j<8;j++){
		say("%d:cr3=%p,code=%p,path=%p\n", table[j].cr3, table[j].code, table[j].path);
	}
	return 0;
}
u64 processcreate(void* file, void* args)
{
	say("file=%s\n",file);
	if(0 == table[0].code)table[0].code = memorycreate(0x100000, 0);

	void* p = (void*)table[0].code;
	int ret = readfile(file,0, 0,0, p,0x10000);
	printmemory(p, 0x100);
	return 0;
}
void processdelete(u64 h)
{
}



void initprocess()
{
	int j;
	for(j=0;j<8;j++){
		table[j].cr3 = 0;
		table[j].code = 0;
		table[j].path = 0;
	}
}
void freeprocess()
{
}