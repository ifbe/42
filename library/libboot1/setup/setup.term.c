#include "libuser.h"
#define _term_ hex32('t','e','r','m')
#define _vt100_ hex64('v','t','1','0','0',0,0,0)
void waiter(void*);




void term_delete(struct item* wrk, u8* arg)
{
}
int term_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	//composer
	void* xxx = arterycreate(_vt100_, 0, 0, 0);

	//client
	void* ccc = supplycreate(_std_, 0, 0, 0);

	//server
	void* sss = systemcreate(0, argv[1], 0, 0);


	struct relation* crel = relationcreate(xxx,0, _art_,_dst_, ccc,0, _sup_,_dst_);
	relationlinkup((void*)&crel->srcchip, (void*)&crel->dstchip);

	struct relation* srel = relationcreate(xxx,0, _art_,_src_, sss,0, _sys_,_dst_);
	relationlinkup((void*)&srel->srcchip, (void*)&srel->dstchip);

	waiter(wrk);
	return 0;
}




void term_init()
{
}
void term_exit(void* addr)
{
}