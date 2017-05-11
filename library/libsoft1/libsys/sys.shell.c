#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(void*, ...);




static int shell_list(u8* p)
{
	return 0;
}
static int shell_choose(u8* p)
{
	return 0;
}
static int shell_read(u8* p)
{
	return 0;
}
static int shell_write(u8* p)
{
	return 0;
}




static int shell_start(u64 type, u8* p)
{
	return 0;
}
static int shell_stop()
{
	return 0;
}
void shell_delete()
{
}
void shell_create(u8* world, u64* p)
{
	//
	p[0]=0x6c616963657073;
	p[1]=0x6c6c656873;
	p[2]=(u64)shell_start;
	p[3]=(u64)shell_stop;
	p[4]=(u64)shell_list;
	p[5]=(u64)shell_choose;
	p[6]=(u64)shell_read;
	p[7]=(u64)shell_write;
}
