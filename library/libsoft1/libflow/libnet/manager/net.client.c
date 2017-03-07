#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int http_read(void*, int);
int http_write(void*, int, void*, int);
//
int startsocket(u8*, int, int);
int stopsocket();
int listsocket(u8*);
int choosesocket(u8*, int);
int readsocket(u64 fd, u8* buf, u64 off, u64 len);
int writesocket(u64 fd, u8* buf, u64 off, u64 len);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* datahome=0;
char indexhtml[]="index.html";




static int client_read()
{
	return 0;
}
static int client_write(u8* p)
{
	return 0;
}
static int client_list(u8* p)
{
	return 0;
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
	u8* str = buf+0x80;

	//parse
	ret = buf2net(p, 128, buf, ip, &port, str);
	if(ret <= 0)return 0;
	say("type=%s, addr=%s, port=%d, extra=%s\n", buf, ip, port, str);

	//compare
	if(ncmp(buf, "raw", 3) == 0)
	{
		fd = startsocket(ip, port, 'r');
	}
	else if(ncmp(buf, "udp", 3) == 0)
	{
		fd = startsocket(ip, port, 'u');
	}
	else
	{
		fd = startsocket(ip, port, 't');
		if(fd == 0)return -1;

		if(ncmp(buf, "http", 4) == 0)
		{
			str = (u8*)indexhtml;
			ret = http_write(datahome, 0x100000, str, 10);
			ret = writesocket(fd, datahome, 0, ret);
		}
	}

	//start
	return 0;
}




static int client_start(u64 type, u8* p)
{
	return 0;
}
static int client_stop()
{
	return 0;
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
	return 0;
}
