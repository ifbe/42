#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
int systemread(char* rdi,QWORD rsi,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rcx);
int cleverread(QWORD,QWORD,QWORD,char*,QWORD,QWORD);
//用了别人的
void printmemory(char* addr,QWORD size);
void say(char* fmt,...);




//memory
static char* fshome;
	static char* first64k;		//+0x10000
static char* dirhome;
static char* datahome;








static int elf_ls()
{
	return 0;
}
static int elf_cd()
{
	return 0;
}
static int elf_load()
{
	return 0;
}
static int elf_store()
{
	return 0;
}
int explainelfhead()
{
	QWORD* dstqword=(QWORD*)fshome;

	//func ls,cd,load,store
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(QWORD)elf_ls;
	dstqword[5]=(QWORD)elf_cd;
	dstqword[6]=(QWORD)elf_load;
	dstqword[7]=(QWORD)elf_store;
	dstqword += 8;

	return 1;
}








int iself(char* addr)
{
	unsigned int temp=*(unsigned int*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
int explainelf(QWORD sector,char* addr)
{
	int ret=0;

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	fshome=addr+0;
		first64k=fshome+0x10000;
	dirhome=addr+0x100000;
	datahome=addr+0x200000;

	//读分区前8扇区，检查magic值
	ret=systemread(first64k,sector,0x8);	//0x1000
	ret=iself(first64k);
	if( ret == 0 ) return -1;

	//读出关键数据
	ret=explainelfhead();
	if(ret<0)return ret;

	return 0;
}
