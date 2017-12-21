#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void chance_create(void*, void*);
void chance_delete();
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




//
static int bio_list(u8* arg1)
{
	return 0;
}
static int bio_choose(u8* arg)
{
	return 0;
}
static int bio_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int bio_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int bio_start(u8* p)
{
	return 0;
}
static int bio_stop()
{
	return 0;
}
int bio_create(u8* base, u64* p)
{
	u8* q;

	//
	guys = base;
	fshome = base+0x100000;
	dirhome = base+0x200000;
	datahome = base+0x300000;

	//
	p[0]=0;
	p[1]=0x6f6962;
	p[2]=(u64)bio_start;
	p[3]=(u64)bio_stop;
	p[4]=(u64)bio_list;
	p[5]=(u64)bio_choose;
	p[6]=(u64)bio_read;
	p[7]=(u64)bio_write;

	q = (u8*)p;
	q += 0x100;

	chance_create(base, q);
	q += 0x100;

	return q-(u8*)p;
}
int bio_delete()
{
	return 0;
}
