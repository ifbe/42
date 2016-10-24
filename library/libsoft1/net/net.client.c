#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createclient();
int deleteclient();
int startclient(char*,int);
int stopclient();
int readclient(char*,int);
int writeclient(char*,int);
int listclient(char*);
int chooseclient(char*, char*, int, char*);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
void printmemory(char*,int);
void say(char*,...);




//
static unsigned char* datahome=0;




static int client_list(char* p)
{
	listclient(p);
	return 1;
}
static int client_choose(char* p)
{
	int ret;

	u8 type[64];
	u8 addr[64];
	int port;
	u8 extra[64];

	ret = buf2net(
		p,64,
		type, addr, &port, extra
	);
	if(ret <= 0)return chooseclient(0, 0, 0, 0);
	else return chooseclient(type, addr, port, extra);
}
static int client_read()
{
}
static int client_write(char* p)
{
	int i=0;
	if(p==0)return 0;

	while(p[i]!=0)i++;
	return writeclient(p,i);
}




static int client_start(u64 type,char* p)
{
}
static int client_stop()
{
	stopclient();
}
int client_create(char* world, u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x63;

	p[10]=(u64)client_start;
	p[11]=(u64)client_stop;
	p[12]=(u64)client_list;
	p[13]=(u64)client_choose;
	p[14]=(u64)client_read;
	p[15]=(u64)client_write;
}
int client_delete()
{
	client_stop();
}
