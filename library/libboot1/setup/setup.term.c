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
	//systemcreate();


	struct relation* rel = relationcreate(xxx,0, _art_,_dst_, ccc,0, _sup_,0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	waiter(wrk);
	return 0;
}




void term_init()
{
}
void term_exit(void* addr)
{
}