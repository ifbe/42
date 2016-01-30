#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//logical
void initlogical(char*);
int mount(char*);
int ls(char*);
int cd(char*);
int load(char*);
int store(char*);
//physical
void initreal(char*);
void softhelp(char*);
void softlist(char*);
void softinto(char*);
void softread(char*);
void softwrite(char*);
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




//
int softcommand(char* buffer)
{
	//---------1.把收到的命令检查并翻译一遍--------
	char* arg0;
	char* arg1;
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return 0;




	//------------2.实际的活都找专人来干------------------
	int ret=compare( arg0 , "help" );
	if(ret==0)
	{
		softhelp(arg1);
		return 1;
	}
	//physical 1
	ret=compare( arg0 , "list" );
	if(ret==0)
	{
		softlist(arg1);
		return 1;
	}
	//physical 2
	ret=compare( arg0 , "into" );
	if(ret==0)
	{
		softinto(arg1);
		return 1;
	}
	//physical 3
	ret=compare( arg0 , "read" );
	if(ret==0)
	{
		softread(arg1);
		return 1;
	}
	//physical 4
	ret=compare( arg0 , "write" );	//dangerous
	if(ret==0)
	{
		softwrite(arg1);
		return 1;
	}




	//logical 0 (servent 0) (check)
	ret=compare( arg0 , "mount");
	if(ret==0)
	{
		mount(arg1);
		return 1;
	}
	//logical 1 (servent 1) (search)
	ret=compare( arg0 , "ls");
	if(ret==0)
	{
		ls(arg1);
		return 1;
	}
	//logical 2 (servent 2) (enter)
	ret=compare( arg0 , "cd" );
	if(ret==0)
	{
		cd(arg1);
		return 1;
	}
	//logical 3 (servent 3) (get)
	ret=compare( arg0 , "load" );
	if(ret==0)
	{
		load(arg1);
		return 1;
	}
	//logical 4 (servent 4) (put)
	ret=compare( arg0 , "store" );	//very dangerous
	if(ret==0)
	{
		store(arg1);
		return 1;
	}

	return 0;	//ret<=0:failed
}




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
	softinto(0);
}
void killsoftware()
{
}
