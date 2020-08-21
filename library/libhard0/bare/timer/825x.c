#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void enableirq(int);
void disableirq(int);
//
void printmemory(void*, int);
void say(void*, ...);




static int dt = 0;
void isr_825x()
{
	if(0==(dt%100))say("dt=%d\n",dt);
	dt ++;
}
void init825x()
{
	u16 lo = 11931&0xff;
	u16 hi = 11931/256;

	out8(0x43, 0x34);
	out8(0x40, lo);
	out8(0x40, hi);

	enableirq(0);
}
