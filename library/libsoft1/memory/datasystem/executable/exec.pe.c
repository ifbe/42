#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int systemread( char* rdi,u64 rsi,u64 rcx);
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
int explainpe(char* addr)
{
	int ret=0;

	//
	ret=ispe(first64k);
	if( ret == 0 ) return -1;

	return 0;
}
