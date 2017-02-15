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
int tls_read(u64* p, u8* buf, u64 len);
int tls_write(u64* p, u8* buf, u64 len);
//
int readfile(void* name, void* mem, u64 offset, u64 count);
int writefile(void* name, void* mem, u64 offset, u64 count);
int readserver(u64 fd, void* addr, u64 offset, u64 count);
int writeserver(u64 fd, void* addr, u64 offset, u64 count);
//
int diary(char*, int, char*, ...);
void printmemory(char*,int);
void say(char*, ...);




//
static char http_response[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size=sizeof(http_response) -1;
//
static char* Connection = 0;
static char* Upgrade = 0;
static char* GET = 0;




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
int http_write(u8* buf, int len)
{
	int ret;
	say("path=%s\n",GET);

	//
	ret = readfile(GET, buf, 0, 0x100000);
	if(ret <= 0)
	{
		*(u32*)buf = 0x343034;
		return 3;
	}

	//
	return ret;
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
	if( (Connection != 0) && (Upgrade != 0) )return 0x11;
	else if(GET != 0)return 0x10;
	else return 0;
}
int serve_http(u64* p, u8* buf, int len)
{
	int ret;

	//
	len = http_read(buf, len);
	if(len <= 0)goto byebye;

	//
	len = http_write(buf, len);
	if(len <= 0)goto byebye;

	//
	ret = writeserver(p[2], http_response, 0, http_response_size);
	ret = writeserver(p[2], buf, 0, len);

byebye:
	//
	p[1] = 0;
	return 1;
}
int serve_https(u64* p, u8* buf, int len)
{
	//tls >>>> ascii
	len = tls_read(p, buf, len);
	if(len < 0)goto error;
/*
	if(len > 0)
	{
		//ascii >>>> path
		len = http_read(buf, len);
		if(len <= 0)goto error;

		//path >>>> bin
		len = http_write(buf, len);
		if(len <= 0)goto error;
	}
*/
	//bin >>>> tls
	len = tls_write(p, buf, len);
	if(len <= 0)goto error;

good:
	//send
	writeserver(p[2], buf, 0, len);
	return 0;

error:
	//
	p[1] = 0;
	return 0;
}
