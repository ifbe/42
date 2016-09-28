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




int iself(char* addr)
{
	unsigned int temp=*(unsigned int*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
int explainelf(char* addr)
{
	int ret=0;

	//
	ret=iself(first64k);
	if( ret == 0 ) return -1;

	return 0;
}
