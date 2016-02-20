#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void initreal(char*);
void initlogic(char*);
void say(char*,...);




//
static char* diskhome=0;
static char* fshome=0;
static char* dirhome=0;
static char* datahome=0;




int file_open(char* p)
{
	say("opening\n");
}
int file_close(char* p)
{
	say("closing\n");
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
	initlogic(world);
}
void killsoftware()
{
}
