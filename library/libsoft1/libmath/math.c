#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static void math_list(u8* arg1)
{
}
static void math_choose(u8* arg)
{
}
static void math_read()
{
}
static void math_write()
{
}
static void math_start(u8* p)
{
}
static void math_stop()
{
}
int math_create(u8* softaddr,u64* p)
{
	p[0]=0;
	p[1]=0x6874616d;
	p[2]=(u64)math_start;
	p[3]=(u64)math_stop;
	p[4]=(u64)math_list;
	p[5]=(u64)math_choose;
	p[6]=(u64)math_read;
	p[7]=(u64)math_write;

	//
	return 0x100;
}
int math_delete()
{
	return 0;
}
