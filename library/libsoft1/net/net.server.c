#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createserver();
int deleteserver();
int startserver(void*, int, void*, int);
int stopserver();
int readserver(u8*,int);
int writeserver(u8*,int);
int listserver(u8*);
int chooseserver(u8*, u8*, int, u8*);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
void printmemory(void*, int);
void say(void*, ...);




//
static u8* datahome=0;




static int server_list(u8* p)
{
	return 0;
}
static int server_choose(u8* p)
{
	return 0;
}
static int server_read()
{
	return 0;
}
static int server_write(u8* p)
{
	return 0;
}




static int server_start(u8* p)
{
	//0.0.0.0:2222/sdcard
	return startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
	return stopserver();
}
int server_create(u8* world, u64* p)
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
	return 0;
}
int server_delete()
{
	return 0;
}
