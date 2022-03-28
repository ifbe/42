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
//
#define _want_ hex32('w','a','n','t')
#define _done_ hex32('d','o','n','e')
#define _take_ hex32('t','a','k','e')
#define _give_ hex32('g','i','v','e')
void syscall_caller(u64, u64*);




void main()
{
	u64 arg[8];
	u8 buf[0x1000];
	syscall_caller(_ver_,   arg);
	syscall_caller(_yield_, arg);

	//open
	arg[0] = (u64)"shit";
	arg[1] = 0;
	syscall_caller(_want_, arg);

	//read
/*	arg[0] = ;
	arg[1] = 0;
	arg[2] = buf;
	arg[3] = 0x1000;
	syscall_caller(_give_, arg);
*/
	//write
	arg[0] = 1;
	arg[1] = 0;
	arg[2] = (u64)"hello world\n";
	arg[3] = 12;
	syscall_caller(_give_, arg);

	//close
	arg[0] = 0;
	syscall_caller(_done_, arg);

	syscall_caller(_exit_,  arg);
	syscall_caller(_ver_,   arg);	//should not print anything
	for(;;);
}
