#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void chemequa_create(void*, void*);
void chemequa_delete();
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




//
static int chem_list(u8* arg1)
{
	return 0;
}
static int chem_choose(u8* arg)
{
	return 0;
}
static int chem_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int chem_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int chem_start(u8* p)
{
	return 0;
}
static int chem_stop()
{
	return 0;
}
int chem_create(u8* base, u64* p)
{
	u8* q;

	//
	guys = base;
	fshome = base+0x100000;
	dirhome = base+0x200000;
	datahome = base+0x300000;

	//
	p[0]=0;
	p[1]=0x6d656863;
	p[2]=(u64)chem_start;
	p[3]=(u64)chem_stop;
	p[4]=(u64)chem_list;
	p[5]=(u64)chem_choose;
	p[6]=(u64)chem_read;
	p[7]=(u64)chem_write;

	q = (u8*)p;
	q += 0x100;

	chemequa_create(base, q);
	q += 0x100;

	return q-(u8*)p;
}
int chem_delete()
{
	return 0;
}
