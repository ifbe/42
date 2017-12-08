#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//random
int createrandom(void*);
int deleterandom();
//signal
int createsignal(void*);
int deletesignal();
//epoll,iocp,kqueue
int createwatcher(void*);
int deletewatcher();
//socket
int createsocket(void*);
int deletesocket();
//shell
int createshell(void*);
int deleteshell();
//uart
int createuart(void*);
int deleteuart();
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* mega0;
static u8* mega1;
static u8* mega2;
static u8* mega3;




void systemread()
{
}
void systemwrite()
{
}
void systemlist()
{
}
void systemchoose()
{
}
void systemstart()
{
}
void systemstop()
{
}
void systemcreate(u8* type, u8* addr)
{
	int j;
	if(type != 0)return;
	for(j=0;j<0x400000;j++)addr[j]=0;

	//where
	mega0=addr;
	mega1=addr+0x100000;
	mega2=addr+0x200000;
	mega3=addr+0x300000;

	//
	createrandom(mega0);
	createsignal(mega0);
	createwatcher(mega0);
	createsocket(mega0);
	createshell(mega0);
	createuart(mega0);

	//
	//say("[8,c):createed system\n");
}
void systemdelete()
{
	//
	//say("[8,c):deleteing system\n");

	//
	deleteuart();
	deleteshell();
	deletesocket();
	deletewatcher();
	deletesignal();
	deleterandom();
}
