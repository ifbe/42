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
int tls_read(u64 fd, u8* buf, u64 len);
int tls_write(u64 fd, u8* buf, u64 len);
//
int readfile(void* name, void* mem, u64 offset, u64 count);
int writefile(void* name, void* mem, u64 offset, u64 count);
int readsocket(u64 fd, void* addr, u64 offset, u64 count);
int writesocket(u64 fd, void* addr, u64 offset, u64 count);
//
int fmt(void*, int, void*, ...);
void printmemory(void*,int);
void say(void*, ...);




//
static char http_response[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
//
static char* Connection = 0;
static char* Upgrade = 0;
static char* GET = 0;




int http_write_request(u8* buf, int len, char* url, char* host)
{
	if(	(url==0)|
		(url[0]==0) |
		(url[0]=='/'&&url[1]==0)
	)
	{url="index.html";}

	return fmt(buf, 100,
		"GET /%s HTTP/1.1\r\n"
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
	if(GET[0]==' ')
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




int check_http(char* buf, int max)
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
#define HTTP 0x50545448
#define http 0x70747468
#define WS 0x5357
#define ws 0x7377
	if( (GET != 0) && (Connection != 0) && (Upgrade != 0) )return WS;
	if(GET != 0)return HTTP;
	if( (Connection != 0) && (Upgrade != 0) )return ws;
	else return 0;
}
int serve_http(u64 fd, u64 type, u8* buf, int len)
{
	int ret;
	if(type == http)
	{
		printmemory(buf, len);
		goto byebye;
	}

	//
	len = http_read(buf, len);
	if(len <= 0)goto byebye;

	//
	len = http_write_file(buf, len, GET);
	if(len <= 0)goto byebye;

	//
	ret = writesocket(fd, http_response, 0, sizeof(http_response)-1);
	ret = writesocket(fd, buf, 0, len);

byebye:
	return 0;
}
int serve_https(u64 fd, u64 type, u8* buf, int len)
{
	return 0;
}
