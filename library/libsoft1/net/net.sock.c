#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createsocket();
int deletesocket();
int startsocket(char*,int);
int stopsocket();
int readsocket(char*,int);
int writesocket(char*,int);
int listsocket(char*);
int choosesocket(char*, char*, int, char*);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
void printmemory(char*,int);
void say(char*,...);




//
static unsigned char* datahome=0;




static int sock_list(char* p)
{
	listsocket(p);
	return 1;
}
static int sock_choose(char* p)
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
	if(ret <= 0)return choosesocket(0, 0, 0, 0);
	else return choosesocket(type, addr, port, extra);
}
static int sock_read()
{
}
static int sock_write(char* p)
{
	int i=0;
	if(p==0)return 0;

	while(p[i]!=0)i++;
	return writesocket(p,i);
}




static int sock_start(u64 type,char* p)
{
}
static int sock_stop()
{
	stopsocket();
}
int sock_create(char* world, u64* p)
{
	//
	datahome=world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x6b636f73;

	p[10]=(u64)sock_start;
	p[11]=(u64)sock_stop;
	p[12]=(u64)sock_list;
	p[13]=(u64)sock_choose;
	p[14]=(u64)sock_read;
	p[15]=(u64)sock_write;
}
int sock_delete()
{
	sock_stop();
}
