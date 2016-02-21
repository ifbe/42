#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//用了别人的
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int cleverread(QWORD,QWORD,QWORD,char*,QWORD,QWORD);
void printmemory(char* addr,QWORD size);
void say(char* fmt,...);




//memory
static char* fshome;
	static char* first64k;		//+0x10000
static char* dirhome;
static char* datahome;








static int pe_ls()
{
	return 0;
}
static int pe_cd()
{
	return 0;
}
static int pe_load()
{
	return 0;
}
static int pe_store()
{
	return 0;
}
int explainpehead()
{
	QWORD* dstqword=(QWORD*)fshome;

	//func ls,cd,load,store
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(QWORD)pe_ls;
	dstqword[5]=(QWORD)pe_cd;
	dstqword[6]=(QWORD)pe_load;
	dstqword[7]=(QWORD)pe_store;
	dstqword += 8;

	return 1;
}








int ispe(char* addr)
{
	unsigned long long temp;

	//[0,1]:'M','Z'
	temp=*(unsigned short*)addr;
	if(temp!=0x5a4d)return 0;

	//[0x3c,0x3f]:	=0x40,=0xf0,=0x?
	temp=*(unsigned int*)(addr+0x3c);
	if(temp>=0xffc)return 0;

	temp=*(unsigned int*)(addr+temp);
	if(temp!=0x4550)return 0;

	return 64;
}
int explainpe(QWORD sector,char* addr)
{
	int ret=0;

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	fshome=addr+0;
		first64k=fshome+0x10000;
	dirhome=addr+0x100000;
	datahome=addr+0x200000;

	//读分区前8扇区，检查magic值
	ret=readmemory(first64k,sector,0,0x8);	//0x1000
	ret=ispe(first64k);
	if( ret == 0 ) return -1;

	//读出关键数据
	ret=explainpehead();
	if(ret<0)return ret;

	return 0;
}
