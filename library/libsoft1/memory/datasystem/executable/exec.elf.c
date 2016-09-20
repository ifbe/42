#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int systemread(char* rdi,u64 rsi,u64 rcx);
int systemwrite(char* rdi,u64 rsi,u64 rcx);
int cleverread(u64,u64,u64,char*,u64,u64);
//用了别人的
void printmemory(char* addr,u64 size);
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
	u64* dstqword=(u64*)fshome;

	//func ls,cd,load,store
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(u64)elf_ls;
	dstqword[5]=(u64)elf_cd;
	dstqword[6]=(u64)elf_load;
	dstqword[7]=(u64)elf_store;
	dstqword += 8;

	return 1;
}








int iself(char* addr)
{
	unsigned int temp=*(unsigned int*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
int explainelf(u64 sector,char* addr)
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
