#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int startfile();
int stopfile();
int sectorread(char* rdi,u64 rsi,u64 rcx);
int sectorwrite(char* rdi,u64 rsi,u64 rcx);
//
u64 prelibation(char*);
int explaingpt(char* src,char* dest);
int explainmbr(char* src,char* dest);
//
int hexstring2data(char* src,u64* dest);
int compare(char*,char*);
//基本函数
void printmemory(char*,int);
void say(char*,...);




//what
static char* diskhome;
static char* fshome;
static char* dirhome;
static char* datahome;




static int pt_list(char* p)
{
}
static int pt_choose(char* p)
{
}
static int pt_read(char* p)
{
}
static int pt_write(char* p)
{
}




static int pt_start(u64 type,char* p)
{
	int ret;

	ret=startfile(p);
        if(ret<=0)return -1;

	sectorread(datahome , 0 , 64);
	type=prelibation(datahome);

	if(type==0x747067)              //'gpt'
	{
		explaingpt(datahome,diskhome);
	}
	if(type==0x72626d)              //'mbr'
	{
		explainmbr(datahome,diskhome);
	}

	return 1;
}
static int pt_stop(char* p)
{
	stopfile();
}
void pt_create(char* world,u64* p)
{
	//
	diskhome=world;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	p[0]=0x6d656d;
	p[1]=0x7470;
	p[2]=(u64)pt_start;
	p[3]=(u64)pt_stop;
	p[4]=(u64)pt_list;
	p[5]=(u64)pt_choose;
	p[6]=(u64)pt_read;
	p[7]=(u64)pt_write;
}
void pt_delete()
{
}
