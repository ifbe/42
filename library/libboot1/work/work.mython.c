#include "libboot.h"
//
void* originsearch(void*, int);
void* workersearch(void*, int);
void* devicesearch(void*, int);
void* driversearch(void*, int);
void* systemsearch(void*, int);
void* arterysearch(void*, int);
void* supplysearch(void*, int);
void* entitysearch(void*, int);
void* originmodify(int argc, void* argv);
void* workermodify(int argc, void* argv);
void* devicemodify(int argc, void* argv);
void* drivermodify(int argc, void* argv);
void* systemmodify(int argc, void* argv);
void* arterymodify(int argc, void* argv);
void* supplymodify(int argc, void* argv);
void* entitymodify(int argc, void* argv);
//
void exiter(void*);
void pulser(void*);
void poller(void*);
void realer(void*);
void waiter(void*);
//
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//
u8* getsuffix(u8* p);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int hexstr2u32(void*, u32*);
int hexstr2u64(void*, u64*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);




void term_ls(u8* buf, int len)
{
	if(buf[3] <= 0x20)
	{
		originsearch(0, 0);
		say("----------------\n");
		workersearch(0, 0);
		say("----------------\n");
		devicesearch(0, 0);
		say("----------------\n");
		driversearch(0, 0);
		say("----------------\n");
		systemsearch(0, 0);
		say("----------------\n");
		arterysearch(0, 0);
		say("----------------\n");
		supplysearch(0, 0);
		say("----------------\n");
		entitysearch(0, 0);
		return;
	}

	buf += 3;
	if(0 == ncmp(buf, "origin", 6))originsearch(0, 0);
	else if(0 == ncmp(buf, "worker", 6))workersearch(0, 0);
	else if(0 == ncmp(buf, "device", 6))devicesearch(0, 0);
	else if(0 == ncmp(buf, "driver", 6))driversearch(0, 0);
	else if(0 == ncmp(buf, "system", 6))systemsearch(0, 0);
	else if(0 == ncmp(buf, "artery", 6))arterysearch(0, 0);
	else if(0 == ncmp(buf, "supply", 6))supplysearch(0, 0);
	else if(0 == ncmp(buf, "entity", 6))entitysearch(0, 0);
	else say("ls(%s)\n", buf);
}
void term_mmio(int argc, u8** argv)
{
	if(argc <= 1)return;

	u64 addr;
	hexstr2u64(argv[1], &addr);
	addr &= 0xfffffffffffffffc;
	if(3 == argc){
		u32 data;
		hexstr2u32(argv[2], &data);
		*(u32*)addr = data;
		say("writing: [%llx] = %08x\n", addr, data);
	}
	say("reading: [%llx] = %08x\n", addr, *(u32*)addr);
}
void term_memory(int argc, u8** argv)
{
	if(argc <= 1)return;

	u64 addr;
	hexstr2u64(argv[1], &addr);

	int len = 0x100;
	if(3 == argc)hexstr2u32(argv[2], &len);

	printmemory((void*)addr, len);
}




void term_prompt()
{
	say("[term@void]");
}
int termwrite(u8* buf, int len)
{
	int j;
	u8* argv[8];
	u8 tmp[0x1000];
	if(0 == buf)goto finish;
	if(buf[0] <= 0x20)goto finish;

	if(('q' == buf[0])&&(buf[1] <= 0x20))goto byebye;
	if(0 == ncmp(buf, "exit", 4))goto byebye;

	j = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(0 == ncmp(buf, "ls", 2))term_ls(buf, len);
	else if(0 == ncmp(buf, "mmio", 4))term_mmio(j, argv);
	else if(0 == ncmp(buf, "memory", 5))term_memory(j, argv);
	else if(0 == ncmp(buf, "origin", 6))originmodify(j, argv);
	else if(0 == ncmp(buf, "worker", 6))workermodify(j, argv);
	else if(0 == ncmp(buf, "device", 6))devicemodify(j, argv);
	else if(0 == ncmp(buf, "driver", 6))drivermodify(j, argv);
	else if(0 == ncmp(buf, "system", 6))systemmodify(j, argv);
	else if(0 == ncmp(buf, "artery", 6))arterymodify(j, argv);
	else if(0 == ncmp(buf, "supply", 6))supplymodify(j, argv);
	else if(0 == ncmp(buf, "entity", 6))entitymodify(j, argv);

finish:
	term_prompt();
	return 1;

byebye:
	eventwrite(0,0,0,0);
	return -1;
}
void termread()
{
}




int mython_fromfile(u8* str, int len)
{
	u8 buf[0x2000];
	len = openreadclose(str, 0, buf, 0x2000);
	if(len <= 0)return 0;

	int j,k = 0;
	for(j=0;j<=len;j++){
		if((j==len)|('\n' == buf[j])){
			say("%.*s\n", j-k, buf);
			k = j+1;
		}
	}
	return 1;
}
int mython_create(struct worker* wrk, void* url, int argc, u8** argv)
{
	int j;
	if(0 == argv)return 0;
	else if(argc <= 1)return 0;

	for(j=1;j<argc;j++){
		say("arg[%d]=%s\n", j, argv[j]);
		mython_fromfile(argv[j], 0);
	}

	waiter(wrk);
	return 0;
}
