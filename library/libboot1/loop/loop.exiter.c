#include "libboot.h"




void exiter(void* exiter)
{
	say("@exiter, byebye!\n");
}




void exiter_delete(struct worker* wrk, u8* arg)
{
}
void exiter_create(struct worker* wrk, u8* arg, int argc, u8** argv)
{
}
