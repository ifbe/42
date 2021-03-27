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


void proctest_syscall(u64 a, u64 b, u64 c, u64 d)
{
	asm("movq %0, %%rax\n"
		"movq %1, %%rbx\n"
		"movq %2, %%rcx\n"
		"movq %3, %%rdx\n"
		"int $0x80\n"
		:
		:"r"(a),"r"(b),"r"(c),"r"(d)
	);
}
void main()
{
	proctest_syscall(_ver_, 0, 0, 0);
	proctest_syscall(_yield_, 0, 0, 0);
	proctest_syscall(_exit_, 0, 0, 0);
	for(;;);
}
