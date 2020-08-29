#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define GDTBUF 0x10000
void lgdt(void* buf, int len);
void say(void*, ...);




//0: 00,00,00,00,00,00,00,00
//1: 00,00,00,00,00,9a,2f,00
//2: 00,00,00,00,00,92,2f,00
void initgdt()
{
	int j;
	u8* buf = (u8*)GDTBUF;
	for(j=0;j<0x10000;j++)buf[j] = 0;

	*(u64*)(buf+0x00) = 0;
	*(u64*)(buf+0x08) = 0x002f9a0000000000;
	*(u64*)(buf+0x10) = 0x002f920000000000;

	lgdt(buf, 8*3-1);
}
