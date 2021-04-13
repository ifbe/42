#include "libsoft.h"
#define _want_ hex32('w','a','n','t')
#define _done_ hex32('d','o','n','e')
#define _take_ hex32('t','a','k','e')
#define _give_ hex32('g','i','v','e')






//open, close, read, write, ioctl, seek
void syscall_want()
{
}
void syscall_done()
{
}
void syscall_take()
{
}
void syscall_give()
{
}
void syscall_ioctl()
{
}
void syscall_seek()
{
}




void system_syscall(u64 req, u64* arg)
{
    switch(req){
	case _want_:syscall_want();return;
	case _done_:syscall_done();return;
	case _take_:syscall_take();return;
	case _give_:syscall_give();return;
	default:say("unknown@syscall: %llx\n", req);
    }
}