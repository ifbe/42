#include "libuser.h"
#define _term_ hex32('t','e','r','m')
#define _vt100_ hex64('v','t','1','0','0',0,0,0)
void waiter(void*);




void term_delete(struct item* wrk, u8* arg)
{
}
int term_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	//server
	void* sss = system_alloc_frompath(0, argv[1]);
	if(0 == sss)return 0;
	system_create(sss, 0, 0, 0);

	//client
	void* ccc = supply_alloc_fromtype(_std_);
	if(0 == ccc)return 0;
	supply_create(ccc, 0, 0, 0);

	//composer
	void* xxx = artery_alloc_fromtype(_vt100_);
	if(0 == xxx)return 0;
	artery_create(xxx, 0, 0, 0);

	//composer and input
	struct relation* crel = relationcreate(xxx,0, _art_,_dst_, ccc,0, _sup_,_dst_);
	relationattach((void*)&crel->srcchip, (void*)&crel->dstchip);

	//composer and target
	struct relation* srel = relationcreate(xxx,0, _art_,_src_, sss,0, _sys_,_dst_);
	relationattach((void*)&srel->srcchip, (void*)&srel->dstchip);

	//wait
	waiter(wrk);
	return 0;
}




void term_init()
{
}
void term_exit(void* addr)
{
}
