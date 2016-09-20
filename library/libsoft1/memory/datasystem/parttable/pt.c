#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//挂载
int explaingpt(char* src,char* dest);	//分区表
int explainmbr(char* src,char* dest);
//
int systemstart(u64,char*);
int systemstop();
int systemread(char* rdi,u64 rsi,u64 rcx);
int systemwrite(char* rdi,u64 rsi,u64 rcx);
//基本函数
u64 prelibation(char*);
int hexstring2data(char* src,u64* dest);
int mem2file(char* src,char* dest,u64 ignore,int size);
int compare(char*,char*);
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

	ret=systemstart(1,p);
        if(ret<=0)return -1;

	systemread(datahome , 0 , 64);
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
	systemstop();
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
