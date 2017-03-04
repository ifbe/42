#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createclient();
int deleteclient();
u64 startclient(int, u8*, int, u8*);
int stopclient();
int listclient(u8*);
int chooseclient(u8*,int);
int readclient(u8*,int);
int writeclient(u8*,int);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* datahome=0;




static int client_read()
{
	return 0;
}
static int client_write(u8* p)
{
	int i=0;
	if(p==0)return 0;

	while(p[i]!=0)i++;
	return writeclient(p,i);
}
static int client_list(u8* p)
{
	return listclient(p);
}
static int client_choose(u8* p)
{
	//
	u64 fd;
	int ret;

	//
	u8 buf[128];
	int port;
	u8* ip = buf+0x10;
	u8* cfg = buf+0x80;

	//parse
	ret = buf2net(p, 128, buf, ip, &port, cfg);
	if(ret <= 0)return 0;
	//say("type=%s, addr=%s, port=%d, extra=%s\n", buf, ip, port, cfg);

	//compare
	if(ncmp(buf, "raw", 3) == 0)ret = 'r';
	else if(ncmp(buf, "udp", 3) == 0)ret = 'u';
	else if(ncmp(buf, "tcp", 3) == 0)ret = 't';
	else if(ncmp(buf, "https", 4) == 0)ret = 't';
	else if(ncmp(buf, "http", 4) == 0)ret = 't';
	else if(ncmp(buf, "wss", 4) == 0)ret = 't';
	else if(ncmp(buf, "ws", 4) == 0)ret = 't';

	//start
	fd = startclient(ret, ip, port, cfg);
	say("fd=%llx\n", fd);

	//
	return 0;
}




static int client_start(u64 type, u8* p)
{
	return 0;
}
static int client_stop()
{
	return stopclient();
}
int client_create(u8* world, u64* p)
{
	//
	datahome = world+0x300000;

	//
	p[0]=0x74656e;
	p[1]=0x746e65696c63;

	p[10]=(u64)client_start;
	p[11]=(u64)client_stop;
	p[12]=(u64)client_list;
	p[13]=(u64)client_choose;
	p[14]=(u64)client_read;
	p[15]=(u64)client_write;
	return 0;
}
int client_delete()
{
	client_stop();
	return 0;
}
