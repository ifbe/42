#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int electrical_create(void*, void*);
int electrical_delete();
int mechanical_create(void*, void*);
int mechanical_delete();
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
	p[2]=(u64)phys_start;
	p[3]=(u64)phys_stop;
	p[4]=(u64)phys_list;
	p[5]=(u64)phys_choose;
	p[6]=(u64)phys_read;
	p[7]=(u64)phys_write;
	q = (u8*)p;
	q += 0x100;

	electrical_create(softaddr, q);
	q += 0x100;

	mechanical_create(softaddr, q);
	q += 0x100;

	return q-(u8*)p;
}
int phys_delete()
{
	mechanical_delete();
	electrical_delete();
	return 0;
}
