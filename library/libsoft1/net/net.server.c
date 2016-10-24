#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createserver();
int deleteserver();
int startserver(char*, int, char*, int);
int stopserver();
int readserver(char*,int);
int writeserver(char*,int);
int listserver(char*);
int chooseserver(char*, char*, int, char*);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
void printmemory(char*,int);
void say(char*,...);




//
static unsigned char* datahome=0;




static int server_list(char* p)
{
}
static int server_choose(char* p)
{
}
static int server_read()
{
}
static int server_write(char* p)
{
}




static int server_start(char* p)
{
	//0.0.0.0:2222/sdcard
	startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
	stopserver();
}
int server_create(char* world, u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x73;

	p[10]=(u64)server_start;
	p[11]=(u64)server_stop;
	p[12]=(u64)server_list;
	p[13]=(u64)server_choose;
	p[14]=(u64)server_read;
	p[15]=(u64)server_write;
}
int server_delete()
{
}
