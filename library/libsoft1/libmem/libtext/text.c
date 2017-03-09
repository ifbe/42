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
static int text_list(u8* arg1)
{
	return 0;
}
static int text_choose(u8* arg)
{
	int j;
	if(arg == 0)return 0;
	if(arg[0] == 0)return 0;
	for(j=0;j<8;j++)
	{
		if(arg[j+1] == 0)break;
	}

	say("0x");
	for(;j>=0;j--)say("%02x",arg[j]);
	say("\n");

	return 0;
}
static int text_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int text_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int text_start(u8* p)
{
	return 0;
}
static int text_stop()
{
	return 0;
}
int text_create(u8* softaddr,u64* p)
{
	//
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x74786574;
	p[2]=(u64)text_start;
	p[3]=(u64)text_stop;
	p[4]=(u64)text_list;
	p[5]=(u64)text_choose;
	p[6]=(u64)text_read;
	p[7]=(u64)text_write;

	return 0x100;
}
int text_delete()
{
	return 0;
}
