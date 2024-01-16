#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void datewrite(u64);
//
void irqchip_enableirq(int chip, int pin, int apicid, int intvec);
void irqchip_disableirq(int chip, int pin);
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void printmemory(void*, int);
void logtoall(void*, ...);




void isr_rtc()
{
	int j;
	u64 date;
	u8* p = (u8*)&date;

	//ack
	out8(0x70, 0x8c);
	p[0] = in8(0x71);

	//century
	out8(0x70, 0x32);
	p[6] = in8(0x71);

	//year
	out8(0x70, 0x9);
	p[5] = in8(0x71);

	//month
	out8(0x70, 0x8);
	p[4] = in8(0x71);

	//day
	out8(0x70, 0x7);
	p[3] = in8(0x71);

	//hour
	out8(0x70, 0x4);
	p[2] = in8(0x71);

	//minute
	out8(0x70, 0x2);
	p[1] = in8(0x71);

	//second
	out8(0x70, 0x0);
	p[0] = in8(0x71);

	for(j=0;j<7;j++)p[j] = (p[j]&0xf) + (p[j]>>4)*10;
	//logtoall("date: %d,%d,%d,%d,%d,%d,%d,%d\n",p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
	datewrite(date);
}
void initrtc()
{
	logtoall("@initrtc\n");
	u8 tmp;
	out8(0x70, 0x8a);
	out8(0x71, 0xaf);	//3=8192hz,6=1024hz,a=64hz,f=2hz

	out8(0x70, 0x8b);
	tmp = in8(0x71);
	out8(0x70, 0x8b);
	out8(0x71, tmp|0x40);

	irqchip_enableirq(0,8, 0,0x28);
	logtoall("initrtc end\n\n");
}
