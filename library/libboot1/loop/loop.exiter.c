#include "libboot.h"




void exiter(void* exiter)
{
	logtoall("@exiter, byebye!\n");
}




void exiter_delete(struct item* wrk, u8* arg)
{
}
void exiter_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	wrk->kind = _loop_;
	wrk->type = _exiter_;
	wrk->vfmt = _main_;
}
