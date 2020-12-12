#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void timewrite(u64);
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
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;

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

	if(0x1f == (dt&0x3f)){		//freq=1000/64
		say("%p: ip=%llx,cs=%llx,rf=%llx,sp=%llx,ss=%llx\n",p, p->ip, p->cs, p->flag, p->sp, p->ss);
	}
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
