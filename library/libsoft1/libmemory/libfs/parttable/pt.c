#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int ismbr(u8*);
void mbr_explain(u8*, u8*);
int isgpt(u8*);
void gpt_explain(u8*, u8*);
int isapple(u8*);
void apple_explain(u8*, u8*);
//
int startfile(u8*);
int stopfile(u8*);
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(u64 start,u64 count);
void say(void*, ...);




//
static u8* fshome;
static u8* dirhome;
static u8* datahome;




static void parttable_list()
{
	int j;
	for(j=0;j<0x80*0x80;j+=0x80)
	{
		if(*(u64*)(fshome+j) == 0)break;

		say("(%8s,%8s)	[%08llx,%08llx] %s\n",
			fshome+j, fshome+j+8,
			*(u64*)(fshome+j+0x10), *(u64*)(fshome+j+0x18),
			fshome+j+0x40
		);
	}
}
static int parttable_choose(u8* p)
{
	int ret;

	//exit?
	stopfile(p);
	if(p == 0)return 0;

	//open
	ret = startfile(p);
	if(ret <= 0)return -1;

	//read
	readfile(datahome, 0, 0, 0x8000);

	//explain
	if(isgpt(datahome) > 0)
	{
		say("gpt\n");
		gpt_explain(datahome, fshome);
	}
	else if(ismbr(datahome) > 0)
	{
		say("mbr\n");
		mbr_explain(datahome, fshome);
	}
	else
	{
		say("parttable notfound\n");
		return 0;
	}

	//list
	parttable_list();
	return 0;
}
static void parttable_read()
{
}
static void parttable_write()
{
}
static void parttable_start()
{
}
static void parttable_stop()
{
}
void parttable_create(void* world, u64* p)
{
	fshome = world+0x100000;
	dirhome = world+0x200000;
	datahome = world+0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x7470;

	p[10]=(u64)parttable_start;
	p[11]=(u64)parttable_stop;
	p[12]=(u64)parttable_list;
	p[13]=(u64)parttable_choose;
	p[14]=(u64)parttable_read;
	p[15]=(u64)parttable_write;
}
void parttable_delete()
{
}

