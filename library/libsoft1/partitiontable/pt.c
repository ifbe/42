#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//挂载
QWORD prelibation(char*);
int explaingpt(char* src,char* dest);	//分区表
int explainmbr(char* src,char* dest);
//
int systemread(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
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




void pt_init(char* addr)
{
	diskhome=addr;
	fshome=addr+0x100000;
	dirhome=addr+0x200000;
	datahome=addr+0x300000;
}
void pt_kill()
{
}
