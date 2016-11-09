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




void parttable_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;
}
void parttable_delete()
{
}

