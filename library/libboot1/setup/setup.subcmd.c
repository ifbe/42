#include "libboot.h"
#define _term_ hex32('t','e','r','m')
//
void* waiter_alloc();
void waiter(void*);
void* pulser_alloc();
void pulser(void*);
void* poller_alloc();
void poller(void*);
//
int pem2der(u8* src, int len, u8* dst, int* dlen);
int der2key(u8* src, int len);
int openreadclose(void* name, int off, void* mem, int len);
int openwriteclose(void* name, int off, void* mem, int len);
int mysnprintf(void*, int, void*, ...);


static _obj* worker = 0;


void parse_pem(u8* path)
{
	u8 pem[0x10000];
	u8 der[0x10000];
	u8 name[128];

	int filesize = openreadclose(path, 0, pem, 0x2000);
	logtoall("filesize=%x\n", filesize);

	int dcnt = 0;
	int dlen = 0;
	int curr = 0;
	int used = 0;
	while(1){
		used = pem2der(pem+curr, filesize-curr, der, &dlen);
		logtoall("[%x,%x)%d\n", curr, curr+used, dcnt);
		if(used <= 0)break;

		//logtoall("dcnt=%d\n", dcnt);
		printmemory(der, dlen);

		mysnprintf(name, 128, "%d.der", dcnt);
		openwriteclose(name, 0, der, dlen);

		if(curr+used+10 >= filesize){
			logtoall("%s: used=%x size=%x\n", __FUNCTION__, curr+used, filesize);
			break;
		}

		curr += used;
		dcnt++;
	}
}
void parse_der(u8* path)
{
	u8 der[0x10000];
	u8 name[128];

	int filesize = openreadclose(path, 0, der, 0x10000);
	logtoall("%s: filesize=%x\n", __FUNCTION__, filesize);

	der2key(der, filesize);
}
int subcmd_eachcmd(struct item* wrk, u8* arg)
{
	void* thr = 0;
	if(0 == ncmp(arg, "help", 4)){
		logtoall("./a.out myml xxxx.myml\n");
		logtoall("./a.out mython xxxx.my\n");
		logtoall("./a.out ...\n");
		return 0;
	}
	else if(0 == ncmp(arg, "pem", 3)){
		parse_pem(arg+4);
		return 0;
	}
	else if(0 == ncmp(arg, "der", 3)){
		parse_der(arg+4);
		return 0;
	}
	else if(0 == ncmp(arg, "myml", 4)){
		logtoall("mode=myml\n");
		thr = bootup_alloc_fromtype(_myml_);
		bootup_create(thr, 0, 1, &arg);
		bootup_delete(thr);
		return 0;
	}
	else if(0 == ncmp(arg, "mython", 6)){
		logtoall("mode=mython\n");
		thr = bootup_alloc_fromtype(_mython_);
		bootup_create(thr, 0, 1, &arg);
		bootup_delete(thr);
		return 0;
	}
	else if(0 == ncmp(arg, "term", 4)){
		logtoall("mode=term\n");
		thr = bootup_alloc_fromtype(_term_);
		bootup_create(thr, 0, 1, &arg);
		bootup_delete(thr);
		return 0;
	}
	else if(0 == ncmp(arg, "guiapp", 6)){
		logtoall("mode=guiapp\n");
		thr = bootup_alloc_fromtype(_guiapp_);
		bootup_create(thr, 0, 1, &arg);
		bootup_delete(thr);
		return 0;
	}
	else if(0 == ncmp(arg, "waiter", 6)){
		void* p = waiter_alloc();
		if(p)waiter(p);
	}
	else if(0 == ncmp(arg, "pulser", 6)){
		void* p = pulser_alloc();
		if(p)pulser(p);
	}
	else if(0 == ncmp(arg, "poller", 6)){
		void* p = poller_alloc();
		if(p)poller(p);
	}

	int j;
	u8* ptr = 0;
	for(j=0;j<256;j++){
		if(arg[j] < 0x20)break;
		if('.' == arg[j])ptr = arg+j;
	}
	if(0 == ptr){
		logtoall("type=unknown\n");
		return 0;
	}

	logtoall("type=%.8s\n", ptr);
	if(0 == ncmp(ptr, ".myml", 5)){
		thr = bootup_alloc_fromtype(_myml_);
		bootup_create(thr, 0, 1, &arg);
		bootup_delete(thr);
	}

	return 0;
}


void decide_loop_or_exit()
{
	int j, cnt=0;
	_obj* chosen = 0;
	for(j=-1;j>-16;j--){
		//logtoall("%d@%p: tier=%.8s kind=%.8s type=%.8s vfmt=%.8s\n", j, &worker[j], &worker[j].tier, &worker[j].kind, &worker[j].type, &worker[j].vfmt);
		if( (0 == chosen) && (_main_ == worker[j].vfmt) )chosen = &worker[j];
		if(0 != worker[j].type)cnt++;
	}

	if(cnt <= 1){
		logtoall("no new node created, exit now\n");
		return;
	}

	if(0 == chosen){
		logtoall("main thread not set, create one\n");
		chosen = bootup_alloc_fromtype(_poller_);
		if(0 == chosen)return;

		bootup_create(chosen, 0, 0, 0);
	}

	logtoall("main thread=%.8s@%p\n", &chosen->type, chosen);
	switch(chosen->type){
	case _poller_:
		poller(chosen);
		break;
	case _pulser_:
		pulser(chosen);
		break;
	}
}




int subcmd_create(struct item* wrk, void* arg, int argc, u8** argv)
{
	if(argc <= 1){
		void* std = supply_alloc_fromtype(_std_);
		supply_create(std, 0, 0, 0);

		void* mpoller = bootup_alloc_fromtype(_poller_);
		if(mpoller){
			bootup_create(mpoller, 0, 0, 0);
			poller(mpoller);
		}
	}
	else{
		int j;
		for(j=1;j<argc;j++){
			subcmd_eachcmd(wrk, argv[j]);
		}

		decide_loop_or_exit();
	}
	return 0;
}
int subcmd_delete(struct item* wrk)
{
	return 0;
}



void subcmd_init(void* addr)
{
	worker = addr+0x100000;
}
void subcmd_exit()
{
}
