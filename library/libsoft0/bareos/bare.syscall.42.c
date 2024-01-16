#include "libsoft.h"
#define _want_ hex32('w','a','n','t')
#define _done_ hex32('d','o','n','e')
#define _take_ hex32('t','a','k','e')
#define _give_ hex32('g','i','v','e')
void syscall_caller(u64 req, u64* arg);
u64 process_virt2phys(u64 va);
int percpu_process();






//open, close, read, write, ioctl, seek
void syscall_want(u64* arg)
{
	u64 va = arg[0];
	int flag = arg[1];

	u64 pa = process_virt2phys(va);
	//logtoall("@want: va=%llx, pa=%llx\n", va, pa);
	if(0 == pa){
		arg[0] = 0;
		return;
	}

	logtoall("@want: %s\n", pa);
}
void syscall_done(u64* arg)
{
	u64 fd = arg[0];
	logtoall("@done: %llx\n", fd);
}
void syscall_take(u64* arg)
{
	u64 fd = arg[0];
	u64 of = arg[1];
	u64 buf = arg[2];
	u64 len = arg[3];
	if(1){
		arg[0] = 0;
		return;
	}

	u64 pa = process_virt2phys(buf);
	//logtoall("@want: va=%llx, pa=%llx\n", va, pa);
	if(0 == pa){
		arg[0] = 0;
		return;
	}

	logtoall("@take: %s\n", pa);
	arg[0] = len;
}
void syscall_give(u64* arg)
{
	u64 fd = arg[0];
	u64 of = arg[1];
	u64 buf = arg[2];
	u64 len = arg[3];
	if(fd != 1){
		arg[0] = 0;
		return;
	}

	u64 pa = process_virt2phys(buf);
	//logtoall("@want: va=%llx, pa=%llx\n", va, pa);
	if(0 == pa){
		arg[0] = 0;
		return;
	}

	logtoall("@give: %s\n", pa);
	arg[0] = len;
}
void syscall_ioctl()
{
}
void syscall_seek()
{
}




//core want a handler
void system_handler(u64 req, u64* arg)
{
    switch(req){
	case _want_:syscall_want(arg);return;
	case _done_:syscall_done(arg);return;
	case _take_:syscall_take(arg);return;
	case _give_:syscall_give(arg);return;
	default:logtoall("unknown@system_handler: %llx{%llx,%llx...}\n", req, arg[0], arg[1]);
    }
}




//user want a syscall
void system_usercall(u64 req, u64* arg)
{
	//same level: do directly

	//no priviledge: call syscall
	syscall_caller(req, arg);
}
