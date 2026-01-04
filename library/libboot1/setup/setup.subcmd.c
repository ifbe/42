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
void* style_alloc();
//
void md5sum(u8* dst, u8* src, int len);
void sha1sum(u8* dst, u8* src, int len);
void sha256sum(u8* dst, u8* src, int len);
void sha512sum(u8* dst, u8* src, int len);
u32 crc32(u32 crc, u8* buf, u32 len);
//
int pem2der(u8* src, int len, u8* dst, int* dlen);
int der2key(u8* src, int len);
//
int openreadclose(void* name, int off, void* mem, int len);
int openwriteclose(void* name, int off, void* mem, int len);
//
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
	int filesize = openreadclose(path, 0, der, 0x10000);
	logtoall("%s: filesize=%x\n", __FUNCTION__, filesize);

	der2key(der, filesize);
}
void calc_hash(u8* path)
{
	u8 data[0x10000];
	int size = openreadclose(path, 0, data, 0x10000);
	logtoall("%s: filesize=%x\n", __FUNCTION__, size);

	u8 md5val[16];
	md5sum(md5val, data, size);
	logtoall("md5=\n");
	printmemory(md5val, 16);

	u8 sha1val[20];
	sha1sum(sha1val, data, size);
	logtoall("sha1=\n");
	printmemory(sha1val, 20);

	u8 sha256val[32];
	sha256sum(sha256val, data, size);
	logtoall("sha256=\n");
	printmemory(sha256val, 32);

	u8 sha512val[32];
	sha512sum(sha512val, data, size);
	logtoall("sha512val=\n");
	printmemory(sha512val, 64);

	u32 crc32val = crc32(0, data, size);
	logtoall("crc32=%x\n", crc32val);
}
void show_pic(u8* path)
{
	//1
	_obj* pic = entity_alloc_fromtype(hex64('p', 'i', 'c', 't', 'u', 'r', 'e', 0));
	entity_create(pic, path, 0, 0);
	struct style* mgr_to_wnd = style_alloc();

	//2
	_obj* wnd = supply_alloc_fromtype(hex32('w','n','d',0));
	supply_create(wnd, 0, 0, 0);
	struct style* wnd_to_mgr = style_alloc();
	wnd_to_mgr->fshape.vc[0] = 0.0;
	wnd_to_mgr->fshape.vc[1] = 0.0;
	wnd_to_mgr->fshape.vq[0] = 1.0;
	wnd_to_mgr->fshape.vq[1] = 1.0;

	//3
	struct relation* rel0 = relationcreate(pic,mgr_to_wnd, _ent_,0, wnd,wnd_to_mgr, _sup_,0);
	relationattach((void*)&rel0->srcchip, (void*)&rel0->dstchip);
}
void serve_webapp()
{
	_obj* sys = system_alloc_frompath(_TCP_, (u8*)"0.0.0.0:9999");

	_obj* art = artery_alloc_fromtype(_HTTP_);
	artery_create(art, 0, 0, 0);

	struct relation* rel0 = relationcreate(art,0, _art_, _src_, sys,0, _sys_, _dst_);
	relationattach((void*)&rel0->srcchip, (void*)&rel0->dstchip);

	_obj* thr = bootup_alloc_fromtype(_poller_);
	bootup_create(thr, 0, 0, 0);
	bootup_delete(thr);
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
	else if(0 == ncmp(arg, "hash=", 4)){
		calc_hash(arg+5);
		return 0;
	}
	else if(0 == ncmp(arg, "pic", 3)){
		show_pic(arg+4);
		void* p = poller_alloc();
		if(p)poller(p);
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
	else if(0 == ncmp(arg, "webapp", 6)){
		serve_webapp();
		return 0;
	}
	else if(0 == ncmp(arg, "waiter", 6)){
		void* p = waiter_alloc();
		if(p)waiter(p);
		return 0;
	}
	else if(0 == ncmp(arg, "pulser", 6)){
		void* p = pulser_alloc();
		if(p)pulser(p);
		return 0;
	}
	else if(0 == ncmp(arg, "poller", 6)){
		void* p = poller_alloc();
		if(p)poller(p);
		return 0;
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
