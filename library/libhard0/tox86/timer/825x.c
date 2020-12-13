#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void timewrite(u64);
void scheduleprocess(void*,int,int);
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void enableirq(int);
void disableirq(int);
//
void printmemory(void*, int);
void say(void*, ...);




struct stackframe{
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;

	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;

	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
}__attribute__((packed));




static u64 dt = 0;
void isr_825x(struct stackframe* p)
{
	//if(0==(dt%1000))say("dt=%d\n",dt);
	dt += 1;
	timewrite(dt*1000);

	scheduleprocess(p, 0, dt&0xfffffff);
}
void init825x()
{
	u32 t = 3579545 / 3 / 1000;
	u16 lo = t & 0xff;
	u16 hi = t >> 8;

	out8(0x43, 0x34);
	out8(0x40, lo);
	out8(0x40, hi);

	enableirq(0);
}
