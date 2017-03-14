#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int findzero(char* p);
int findhead(char* p);
int findtail(char* p);
int cmp(void*,void*);
int ncmp(void*,void*,int);
//
int readfile(void* name, void* mem, u64 off, u64 len);
int writefile(void* name, void* mem, u64 off, u64 len);
int readsocket(int fd, void* mem, int off, int len);
int writesocket(int fd, void* mem, int off, int len);
//
int fmt(void*, int, void*, ...);
void printmemory(void*,int);
void say(void*, ...);




static char* Connection = 0;
static char* Upgrade = 0;
static char* GET = 0;




int http_write_request(u8* buf, int len, char* url, char* host)
{
	if((url==0)|(url[0]==0)){url="/";}

	return fmt(buf, 100,
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Range: bytes=%d-%d\r\n"
		"\r\n",
		url, host, 0, 0xfff
	);
}
int http_write_file(u8* buf, int len, char* name)
{
	int ret = readfile(name, buf, 0, 0x100000);
	if(ret <= 0)
	{
		*(u32*)buf = 0x343034;
		return 3;
	}
	return ret;
}
int http_read(u8* buf, int max)
{
	int j;
	if(GET == 0)return 0;
	if(GET[0] != '/')return 0;
	GET++;

	//
	if(GET[0]<=' ')
	{
		GET = "42.html";
		return 7;
	}

	//
	for(j=0;j<256;j++)
	{
		if(GET[j] <= 0x20)break;
	}
	GET[j] = 0;
	return j;
}




#define HTTP 0x50545448
#define http 0x70747468
#define WS 0x5357
#define ws 0x7377
u64 check_http(u64 fd, u64 type, char* buf, int max)
{
	int ret;
	char* p;

	GET = 0;
	Connection = 0;
	Upgrade = 0;

	p = buf;
	while(1)
	{
		if(ncmp(p, "GET ", 4) == 0)GET = p+4;
		else if(ncmp(p, "Connection: ", 12) == 0)Connection = p+12;
		else if(ncmp(p, "Upgrade: ", 9) == 0)Upgrade = p+9;

		ret = findhead(p);
		if(ret <= 0)break;

		p += ret;
		if(p > buf+max)break;
	}//while
/*
	say("GET@%llx,Connection@%llx,Upgrade@%llx\n",
		(u64)GET,
		(u64)Connection,
		(u64)Upgrade
	);
*/
	//
	if( (GET != 0) && (Connection != 0) && (Upgrade != 0) )return WS;
	if(GET != 0)return HTTP;
	if( (Connection != 0) && (Upgrade != 0) )return ws;
	else return 0;
}
u64 serve_http(u64 fd, u64 type, u8* buf, int len)
{
	int ret;
	if(type == http)
	{
		printmemory(buf, len);
		goto byebye;
	}

	//get filename
	len = http_read(buf, len);
	if(len <= 0)goto byebye;

	//read body
	len = http_write_file(buf, len, GET);
	if(len <= 0)goto byebye;

	//read head
	ret = fmt(buf+len, 0x100000-len,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Content-Length: %d\r\n"
		"\r\n",

		len
	);

	//send head
	ret = writesocket(fd, buf+len, 0, ret);

	//send body
	ret = writesocket(fd, buf, 0, len);

byebye:
	return 0;
}
u64 serve_https(u64 fd, u64 type, u8* buf, int len)
{
	return 0;
}
