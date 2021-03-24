#include "libsoft.h"
int parse_pe(void* pe, int len);
u64 thread_forthisprocess(void* code, void* arg, int procid);
//
void pagetable_use(void* cr3);
void pagetable_makeuser(void* buf, int len, u64 pa, int plen, u64 va, int vlen);




struct procstate{
	void* cr3;		//physical address, page table
	void* code;	//physical address, 2m align
	void* path;	//current folder
	int stat;
}__attribute__((packed));
static struct procstate table[8];
static int proccount;




#define _ver_ hex32('v','e','r',0)
#define _exit_ hex32('e','x','i','t')
static void proctest_main();
static void proctest_entry()
{
	proctest_main();
}
static void proctest_syscall(u64 a, u64 b, u64 c, u64 d)
{
	asm("movq %0, %%rax\n"
		"movq %1, %%rbx\n"
		"movq %2, %%rcx\n"
		"movq %3, %%rdx\n"
		"int $0x80\n"
		:
		:"r"(a),"r"(b),"r"(c),"r"(d)
	);
}
static void proctest_main()
{
	proctest_syscall(_ver_, 0, 0, 0);
	proctest_syscall(_exit_, 0, 0, 0);

	for(;;);
}
static void proctest_end()
{
}




static void process_malloc(u64* twomega, u64* onemega)
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


	//prepare memory for reading
	u64 cr,pa;
	int now = proccount;
	if(0 == table[now].cr3){
		process_malloc(&pa, &cr);
		table[now].cr3 = (void*)cr;
		table[now].code = (void*)pa;
	}
	else{
		cr = (u64)table[now].cr3;
		pa = (u64)table[now].code;
	}


/*	//test0: copy code to pa
	int j;
	u8* tmp = (void*)pa;
	u8* p = (void*)proctest_entry;
	u8* q = (void*)proctest_end;
	for(j=0;j<q-p;j++)tmp[j] = p[j];
	printmemory(tmp, 0x200);
*/

	//test1: read file to pa
	int ret = readfile(file,0, 0,0, (void*)pa,0x200000);
	if(ret <= 0){
		say("@processcreate: readfile notfound\n");
		return 0;
	}

/*
	//final: read file to tmp, parse it
	switch(type){
	case _pe_:
		parse_pe(p, 0x10000);
		break;
	case _elf_:
		parse_elf(p, 0x10000);
		break;
	case _mach_:
		parse_macho(p, 0x10000);
		break;
	default:
		//it is raw code
	}
*/

	//map va to pa
	u64 va = 0xffffffffffe00000;
	pagetable_makeuser((void*)cr, 0x100000, pa, 0x200000, va, 0x200000);
	//printmemory((void*)va, 0x200);


	//here it almost done
	table[now].path = 0;
	table[now].stat = 1;
	thread_forthisprocess((void*)va, 0, now);

	proccount = now+1;
	return 0;
}
void processdelete(u64 h)
{
}




void process_switchto(int curr, int next)
{
	//save curr

	//load next
	pagetable_use(table[next].cr3);
}
void process_registersupplier(int coreid)
{
	say("incoming process: from coreid=%d\n", coreid);

	table[0].cr3 = (void*)0x40000;
	table[0].path = 0;
	table[0].code = 0;
	table[0].stat = 1;
	proccount = 1;
}




void initprocess()
{
	int j;
	for(j=0;j<8;j++){
		table[j].cr3 = 0;
		table[j].code = 0;
		table[j].path = 0;
		table[j].stat = 0;
	}
	proccount = 0;
}
void freeprocess()
{
}