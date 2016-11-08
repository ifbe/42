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
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;
//
static u64 haha = 0;




//real worker
int parttable_explain(u8* p)
{
	//explain
	if(isgpt(p) > 0)
	{
		say("gpt\n");
		gpt_explain(p, fshome);
	}
	else if(ismbr(p) > 0)
	{
		say("mbr\n");
		mbr_explain(p, fshome);
	}
	else
	{
		say("parttable notfound\n");
		return -1;
	}
	return 0;
}
int parttable_focus(u64 where)
{
	haha = where;
}
int parttable_read(u8* mem, u8* file, u64 offset, u64 count)
{
	return readfile(mem, file, offset + haha, count);
}
int parttable_write(u8* mem, u8* file, u64 offset, u64 count)
{
	return readfile(mem, file, offset + haha, count);
}




//just command
int parttable_ls()
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
	return j / 0x80;
}
static int parttable_cd(u8* p)
{
	int ret;

	//exit?
	stopfile(p);
	if(p == 0)return -3;

	//open
	ret = startfile(p);
	if(ret <= 0)return -2;

	//read
	ret = readfile(datahome, 0, 0, 0x8000);

	//
	ret = parttable_explain(datahome);
	if(ret < 0)return ret;

	//list
	parttable_ls();
	return 0;
}
static int parttable_show()
{
	return 0;
}
static int parttable_edit()
{
	return 0;
}
static int parttable_start()
{
	return 0;
}
static int parttable_stop()
{
	return 0;
}
void parttable_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x7470;

	p[10]=(u64)parttable_start;
	p[11]=(u64)parttable_stop;
	p[12]=(u64)parttable_ls;
	p[13]=(u64)parttable_cd;
	p[14]=(u64)parttable_show;
	p[15]=(u64)parttable_edit;
}
void parttable_delete()
{
}

