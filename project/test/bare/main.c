typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _ver_ hex32('v','e','r',0)
#define _yield_ hex32('y','i','e','l')
#define _exit_ hex32('e','x','i','t')


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
}
void main()
{
	u64 arg[8];
	syscall(_ver_,   arg);
	syscall(_yield_, arg);
	syscall(_exit_,  arg);
	syscall(_ver_,   arg);	//should not print anything
	for(;;);
}
