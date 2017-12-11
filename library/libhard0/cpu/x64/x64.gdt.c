#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void lgdt(void* buf, int len);
void say(void*, ...);




void initgdt()
{
	say("@initgdt\n");
}
