#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(u8* out, u8* in, int len);
//
int findzero(char* p);
int findhead(char* p);
int findtail(char* p);
int cmp(u8*,u8*);
int ncmp(u8*,u8*,int);
//
int readfile(char* name, char* mem, u64 offset, u64 count);
int writefile(char* name, char* mem, u64 offset, u64 count);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
int notify_delete(u64 x);
//
int diary(char*, int, char*, ...);
void say(char*, ...);




//
static u8* datahome;
static u8 file[256];
static u8 buf1[256];
static u8 buf2[256];
//
static char http_response[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size=sizeof(http_response) -1;
//
static char* GET = 0;
static char* Connection = 0;
static char* Upgrade = 0;
static char* Sec_WebSocket_Key = 0;




//
static void explainstr(char* buf, int max)
{
	int ret;
	char* p;

	GET = 0;
	Connection = 0;
	Upgrade = 0;
	Sec_WebSocket_Key = 0;

	p = buf;
	while(1)
	{
		if(ncmp(p, "Sec-WebSocket-Key: ", 19) == 0)Sec_WebSocket_Key = p+19;
		else if(ncmp(p, "Connection: ", 12) == 0)Connection = p+12;
		else if(ncmp(p, "Upgrade: ", 9) == 0)Upgrade = p+9;
		else if(ncmp(p, "GET ", 4) == 0)GET = p+4;

		ret = findhead(p);
		if(ret <= 0)break;

		p += ret;
		if(p > buf+max)break;
	}//while

	say("GET@%llx,Connection@%llx,Upgrade@%llx,Sec-WebSocket-Key@%llx\n",
		(u64)GET,
		(u64)Connection,
		(u64)Upgrade,
		(u64)Sec_WebSocket_Key
	);
}





int handshake_websocket(int fd)
{
	int j=0;
	j = findtail(Sec_WebSocket_Key);
	Sec_WebSocket_Key[j] = 0;

	j = diary(buf2, 256,"%s%s",
		Sec_WebSocket_Key,
		"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
	);
	say("%s\n",buf2);

	sha1sum(buf1, buf2, j);
	for(j=0;j<20;j++)say("%.2x",buf1[j]);
	say("\n");

	base64_encode( buf2 ,buf1, 20 );
	say("%s\n",buf2);

	j = diary(datahome, 0x10000,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n\r\n",

		buf2
	);
	j = writeserver(fd, datahome, 0, j);
	say("%s", datahome);

	//handshake
	say("[%d]staging\n\n", fd);
	//clienttype[fd] = 0x10;

	return 0x10;
}
int handshake_http(int fd)
{
	int ret,count;

	if( (GET[0]=='/')&&(GET[1]==' ') )
	{
		diary(file, 16, "42.html");
	}
	else
	{
		for(ret=0;ret<256;ret++)
		{
			if(GET[ret+1] <= 0x20)
			{
				file[ret] = 0;
				break;
			}
			else file[ret] = GET[ret+1];
		}
	}
	say("file=%s\n",file);

	//
	count = readfile(file, datahome, 0, 0x100000);
	if(count <= 0)
	{
		//say("error@readfile\n");
		goto byebye;
	}

	//send
	ret = writeserver(fd, http_response, 0, http_response_size);
	ret = writeserver(fd, datahome, 0, count);

byebye:
	notify_delete(fd);
	return 0;
}
int serve_http(u64 fd, char* buf, int len)
{
	explainstr(buf,len);

	if( (Upgrade != 0) && (Sec_WebSocket_Key != 0) )
	{
		return handshake_websocket(fd);
	}

	else if(GET != 0)
	{
		return handshake_http(fd);
	}
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
