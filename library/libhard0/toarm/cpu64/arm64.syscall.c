#include "libhard.h"
#define _ver_ hex32('v','e','r',0)
#define _slp_ hex32('s','l','p',0)
#define _yield_ hex32('y','i','e','l')
#define _exit_ hex32('e','x','i','t')
int system_syscall(u64 req, u64* arg);




struct saved_cpureg{
    u64 x0;
    u64 x1;
    u64 x2;
    u64 x3;
    u64 x4;
    u64 x5;
    u64 x6;
    u64 x7;
    u64 x8;     //call id
    u64 x9;
};




void syscall_handler(struct saved_cpureg* cpureg)
{
	//do what i can
	switch(cpureg->x8){
	case _ver_:syscall_version();return;
	case _slp_:syscall_sleep();return;
	case _yield_:syscall_yield(cpureg);return;
	case _exit_:syscall_exit(cpureg);return;
	//default:say("unknown@syscall: %llx\n", cpureg->x8);
	}

	//let system do rest
	system_syscall(cpureg->x8, 0);
}
void syscall_caller(u64 req, u64* arg)
{
	//req: x8
	//arg: x0, x1, x2, x3, x4, x5, x6, x7
	asm(
		"mov %0, %%x0\n"		//arg0
		"mov %1, %%x1\n"		//arg1
		"mov %2, %%x2\n"		//arg2
		"mov %3, %%x3\n"		//arg3
		"mov %4, %%x4\n"		//arg4
		"mov %5, %%x5\n"		//arg5
		"mov %6, %%x6\n"		//arg6
		"mov %7, %%x7\n"		//arg7
		"mov %8, %%x8\n"		//this is call id
		"int $0x80\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
	);
}