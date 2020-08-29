#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define GDTBUF 0x10000
void lgdt(void* buf, int len);
void say(void*, ...);




struct gdt{
	u16 limit01;
	u16 base01;
	u8 base2;
	u8 type;
	u8 limit2;
	u8 base3;
};
struct tss{
	u32 rsvd;	//[ 0, 3]
	u64 rsp0;	//[ 4, b]
	u64 rsp1;	//[0c,13]
	u64 rsp2;	//[14,1b]
	u64 rsp3;	//[1c,23]
	u64 ist1;	//[24,2b]
	u64 ist2;	//[2c,33]
	u64 ist3;	//[34,3b]
	u64 ist4;	//[3c,43]
	u64 ist5;	//[44,4b]
	u64 ist6;	//[4c,53]
	u64 ist7;	//[54,5b]
	u32 haha;	//[5c,5f]
	u32 hehe;	//[60,63]
	u16 zero;	//[64,65]
	u16 iopb;	//[66,67]
};




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
