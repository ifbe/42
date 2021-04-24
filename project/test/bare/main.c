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
#define _want_ hex32('w','a','n','t')
#define _done_ hex32('d','o','n','e')
void syscall(u64, u64*);




void main()
{
	u64 arg[8];
	syscall(_ver_,   arg);
	syscall(_yield_, arg);

	arg[0] = (u64)"shit";
	arg[1] = 0;
	syscall(_want_, arg);
	syscall(_done_, arg);

	syscall(_exit_,  arg);
	syscall(_ver_,   arg);	//should not print anything
	for(;;);
}
