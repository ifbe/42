#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u8 in8(u16 port);
void out8(u16 port, u8 data);
void printmemory(void*, int);
void say(void*, ...);




void disableirq(u32 num)
{
}
void enableirq(u32 num)
{
	u8 tmp;
	if((num>=0)&&(num<=7))
	{
		num = ~(1<<num);
		tmp = in8(0x21);
		out8(0x21, tmp & num);
	}
	else if((num>=8)&&(num<=15))
	{
		num = ~(1<<(num-8));
		tmp = in8(0xa1);
		out8(0xa1, tmp & num);

		tmp = in8(0x21);
		out8(0x21, tmp & 0xfb);
	}
}




void endofirq(int num)
{
	if(num >= 8)out8(0xa0, 0x20);
	out8(0x20, 0x20);
}
void init8259()
{
	say("@init8259\n");

	//icw1
	out8(0x20, 0x11);
	out8(0xa0, 0x11);

	//icw2
	out8(0x21, 0x20);
	out8(0xa1, 0x28);

	//icw3
	out8(0x21, 0x4);
	out8(0xa1, 0x2);

	//icw4
	out8(0x21, 0x1);
	out8(0xa1, 0x1);

	//
	out8(0x21, 0xff);
	out8(0xa1, 0xff);

	say("\n\n");
}
