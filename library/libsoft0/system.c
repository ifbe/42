#define QW unsigned long long
#define DW unsigned int
#define u16 unsigned short
#define u8 unsigned char
//file
int createfile();
int deletefile();
//folder
int createfolder();
int deletefolder();
//process
int createprocess();
int deleteprocess();
//random
int createrandom();
int deleterandom();
//signal
int createsignal();
int deletesignal();
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* mega0;
static u8* mega1;
static u8* mega2;
static u8* mega3;




void systemcreate(u8* type, u8* addr)
{
	int j;
	if(type!=0)return;

	//where
	mega0=addr;
	mega1=addr+0x100000;
	mega2=addr+0x200000;
	mega3=addr+0x300000;

	//clean [0x80000,0xfffff]
	for(j=0;j<0x100000;j++)mega0[j]=0;

	//
	createrandom();
	createsignal();

	//
	say("[8,c):createed system\n");
}
void systemdelete()
{
	//
	say("[8,c):deleteing system\n");

	//
	deletesignal();
	deleterandom();
}
