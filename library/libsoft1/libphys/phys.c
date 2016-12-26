#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int cmp(void*,void*);
int hexstr2data(u8*,u64*);
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




//
static int phys_list(u8* arg1)
{
	return 0;
}
static int phys_choose(u8* arg)
{
	return 0;
}
static int phys_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int phys_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int phys_start(u8* p)
{
	return 0;
}
static int phys_stop()
{
	return 0;
}
int phys_create(u8* softaddr,u64* p)
{
	u8* q;

	//
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;

	//
	p[0]=0;
	p[1]=0x73796870;

	p[10]=(u64)phys_start;
	p[11]=(u64)phys_stop;
	p[12]=(u64)phys_list;
	p[13]=(u64)phys_choose;
	p[14]=(u64)phys_read;
	p[15]=(u64)phys_write;

	return 0x80;
}
int phys_delete()
{
	return 0;
}
