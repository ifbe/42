#include "libsoft.h"
int parse_pe(void* pe, int len);
u64 thread_forthisprocess(void* code, void* arg, int procid);
//
void pagetable_use(void* cr3);
void pagetable_makeuser(void* buf, int len, u64 pa, int plen, u64 va, int vlen);




struct procstate{
	void*     cr3;	//physical address of page table, 1m align
	void*    code;	//physical address of code binary, 2m align
	u8*      path;	//current folder
	u64*  fdtable;	//fd0=stdin, fd1=stdout
	u64* memtable;
	int      stat;
	int      type;
};
//}__attribute__((packed));		//arm64 compiler generates wrong code if packed...
static struct procstate perproc[8];
static int proccount;



/*
#define _ver_ hex32('v','e','r',0)
#define _exit_ hex32('e','x','i','t')
static void proctest_main();
static void proctest_entry()
{
	proctest_main();
}
static void proctest_syscall(u64 req, u64* arg)
{
	//req: rdx
	//arg: rax, rbx, rsi, rdi, r8, r9, r14, r15
	asm(
		"movq %0, %%rax\n"		//arg0
		"movq %1, %%rbx\n"		//arg1
		"movq %2, %%rsi\n"		//arg2
		"movq %3, %%rdi\n"		//arg3
		"movq %4, %%r8\n"		//arg4
		"movq %5, %%r9\n"		//arg5
		"movq %6, %%r14\n"		//arg6
		"movq %7, %%r15\n"		//arg7
		"movq %8, %%rdx\n"		//this is call id
		"int $0x80\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
}
static void proctest_main()
{
	u64 arg[8];
	proctest_syscall(_ver_, arg);
	proctest_syscall(_exit_, arg);

	for(;;);
}
static void proctest_end()
{
}*/




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
		say("%d:cr3=%p,code=%p,path=%p,fdtbl=%p\n", j,
			perproc[j].cr3, perproc[j].code, perproc[j].path, perproc[j].fdtable
		);
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
	if(0 == perproc[now].cr3){
		process_malloc(&pa, &cr);
		perproc[now].cr3 = (void*)cr;
		perproc[now].code = (void*)pa;
	}
	else{
		cr = (u64)perproc[now].cr3;
		pa = (u64)perproc[now].code;
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
	perproc[now].path = 0;
	perproc[now].stat = 1;
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
	pagetable_use(perproc[next].cr3);
}
void process_registersupplier(int coreid, void* cr3)
{
	say("incoming process: from coreid=%d\n", coreid);

	perproc[0].cr3 = cr3;
	perproc[0].path = 0;
	perproc[0].code = 0;
	perproc[0].stat = 1;
	proccount = 1;
}




void initprocess()
{
	int j;
	for(j=0;j<8;j++){
		perproc[j].cr3 = 0;
		perproc[j].code = 0;
		perproc[j].path = 0;
		perproc[j].stat = 0;
	}
	proccount = 0;
}
void freeprocess()
{
}