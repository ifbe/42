#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//挂载
int explaingpt(char* src,char* dest);	//分区表
int explainmbr(char* src,char* dest);
//
int systemread(char* rdi,QWORD rsi,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rcx);
//基本函数
int hexstring2data(char* src,QWORD* dest);
int mem2file(char* src,char* dest,QWORD ignore,int size);
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
static int pt_switch(char* p)
{
}
static int pt_read(char* p)
{
}
static int pt_write(char* p)
{
}




static int pt_start(QWORD type,char* p)
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
void pt_init(char* world,QWORD* p)
{
	//
	diskhome=world;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	p[0]=0x7470;
	p[1]=0;
	p[2]=(QWORD)pt_start;
	p[3]=(QWORD)pt_stop;
	p[4]=(QWORD)pt_list;
	p[5]=(QWORD)pt_switch;
	p[6]=(QWORD)pt_read;
	p[7]=(QWORD)pt_write;
}
void pt_kill()
{
}
