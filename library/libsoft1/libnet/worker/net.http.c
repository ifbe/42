#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int findzero(char* p);
int findtail(char* p);
//
int readfile(char* name, char* mem, int offset, int count);
int writefile(char* name, char* mem, int offset, int count);
//
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
int net_stop(u64 x);
//
int diary(char*, int, char*, ...);
void say(char*, ...);




//
static u8* datahome;
//
static char* http_response = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size;
static char* http_context;
static int http_context_size;




void handshake_http(int fd, char* GET)
{
	int ret,count;

	for(ret=0;ret<0x1000;ret++)
	{
		if(GET[ret] <= 0xd)goto byebye;
		if(GET[ret] == 0x20)
		{
				GET[ret] = 0;
				break;
		}
	}
	say("%s\n",GET);

	//home page
	if( (GET[0]=='/')&&(GET[1]==0) )
	{
		diary(GET+1, 16, "42.html");
	}
	say("%s\n",GET+1);

	//
	count = readfile(GET+1, datahome, 0, 0x1000);
	if(ret <= 0)
	{
		say("error@readfile\n");
		goto byebye;
	}

	//send
	ret = writeserver(fd, http_response, 0, findzero(http_response) );
	say("writing http_response\n");
	ret = writeserver(fd, datahome, 0, count);
	say("writing http_context\n");

byebye:
	net_stop(fd);
	say("[%d]done->close\n\n\n\n\n", fd);
}





static void http_list()
{
}
static void http_choose()
{
}
static void http_read()
{
}
static void http_write()
{
}
static void http_start(u64 type,char* p)
{
}
static void http_stop()
{
}
void http_create(char* world,u64* p)
{
	datahome = world + 0x300000;
/*
	p[0]=0x74656e;		//type
	p[1]=0x70747468;	//id

	p[10]=(u64)http_start;
	p[11]=(u64)http_stop;
	p[12]=(u64)http_list;
	p[13]=(u64)http_choose;
	p[14]=(u64)http_read;
	p[15]=(u64)http_write;
*/
}
void http_delete()
{
	http_stop();
}
