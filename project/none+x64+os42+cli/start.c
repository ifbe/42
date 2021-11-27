typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
void main();




void start()
{
	main();
}




void syscall(u64 req, u64* arg)
{
	//req: rdx
	//arg: rax, rbx, rsi, rdi, r8, r9, r14, r15
	asm(
		"movq %0, %%rax\n"		//arg0
		"movq %1, %%rbx\n"		//arg1
		"movq %2, %%rsi\n"		//arg2
		"movq %3, %%rdi\n"		//arg3
		"movq %4, %%r8\n"		//arg4
		"movq %5, %%r9\n"		//arg5
		"movq %6, %%r14\n"		//arg6
		"movq %7, %%r15\n"		//arg7
		"movq %8, %%rdx\n"		//this is call id
		"int $0x80\n"
		:
		: "r"(arg[0]),"r"(arg[1]),"r"(arg[2]),"r"(arg[3]),
		  "r"(arg[4]),"r"(arg[5]),"r"(arg[6]),"r"(arg[7]),"r"(req)
	);

	//return arg[0];
}
