#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//logical
void initlogical(char*);
int mount(char*);
int logic_ls(char*);
int logic_cd(char*);
int logic_load(char*);
int logic_store(char*);
//physical
void initreal(char*);
void real_list(char*);
void real_into(char*);
void real_read(char*);
void real_write(char*);
//libsoft
int compare(char*,char*);
int hexstring2data(char*,QWORD*);
int buf2arg(char*,char**,char**);
//libboot
int printmemory(char* addr,int count);
int say(char* str,...); 




//
static char* diskhome=0;
static char* fshome=0;
static char* dirhome=0;
static char* datahome=0;




void initsoftware(char* world)
{
	//每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//两个小弟
	initreal(world);
	initlogical(world);

	//扫描一遍所有认识的东西，选中找到的第一个
	real_choose(0);
}
void killsoftware()
{
}
