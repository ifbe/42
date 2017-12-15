#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void out8(u16 port, u8 data);




void isr_825x()
{
}
void init825x()
{
	u16 lo = 1192&0xff;
	u16 hi = 1192/256;

	out8(0x43, 0x34);
	out8(0x40, lo);
	out8(0x40, hi);
}
