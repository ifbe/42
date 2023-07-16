#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(void*, ...);




int check_stl(u8* buf, int len)
{
	return 0;
}
int parse_stl(u8* buf)
{
	u32* count = (void*)(buf + 80);
	say("count=%x\n", count[0]);
	return 0;
}