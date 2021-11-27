#include "libhard.h"




void syscall_caller(u64 req, u64* arg)
{
	//req: x8
	//arg: x0, x1, x2, x3, x4, x5, x6, x7
	asm(
		"mov x0, %0\n"		//arg0
		"mov x1, %1\n"		//arg1
		"mov x2, %2\n"		//arg2
		"mov x3, %3\n"		//arg3
		"mov x4, %4\n"		//arg4
		"mov x5, %5\n"		//arg5
		"mov x6, %6\n"		//arg6
		"mov x7, %7\n"		//arg7
		"mov x8, %8\n"		//this is call id
		"svc #0\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
		: "x0","x1","x2","x3","x4","x5","x6","x7","x8"
	);
}
