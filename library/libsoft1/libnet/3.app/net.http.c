#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int findzero(char* p);
int findhead(char* p);
int findtail(char* p);
int cmp(u8*,u8*);
int ncmp(u8*,u8*,int);
//
int tls_read(u64* p, u8* buf, u64 len);
int tls_write(u64* p, u8* buf, u64 len);
//
int readfile(char* name, char* mem, u64 offset, u64 count);
int writefile(char* name, char* mem, u64 offset, u64 count);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
//
int diary(char*, int, char*, ...);
void printmemory(char*,int);
void say(char*, ...);




//
static char http_response[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size=sizeof(http_response) -1;
//
static char* SSH = 0;
static char* GET = 0;
static char* Connection = 0;
static char* Upgrade = 0;




static void http_read(char* buf, int max)
{
	int ret;
	char* p;

	SSH = 0;
	GET = 0;
	Connection = 0;
	Upgrade = 0;

	p = buf;
	while(1)
	{
		if(ncmp(p, "SSH-2.0-", 8) == 0)SSH = p;
		else if(ncmp(p, "GET ", 4) == 0)GET = p+4;
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
}
static int http_write(u64 fd, u8* buf, int len)
{
	int ret,count;
	u8 file[256];

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
	count = readfile(file, buf, 0, 0x100000);
	if(count <= 0)
	{
		//say("error@readfile\n");
		goto byebye;
	}

	//send
	ret = writeserver(fd, http_response, 0, http_response_size);
	ret = writeserver(fd, buf, 0, count);

byebye:
	return 0;
}




int serve_https(u64* p, u8* buf, int len)
{
	int ret;

	ret = tls_read(p, buf, len);

	ret = tls_write(p, buf, len);

	writeserver(p[2], buf, 0, ret);
	p[1] = 0;
	return 0;
}
int serve_http(u64* p, u8* buf, int len)
{
	//https
	if(buf[0] == 0x16)
	{
		p[1] = 0x30;
		goto byebye;
	}

	//.............................................
	http_read(buf,len);

	//ssh
	if(SSH != 0)
	{
		p[1] = 0x20;
	}

	//ws
	else if( (Connection != 0) && (Upgrade != 0) )
	{
		p[1] = 0x10;
	}

	//http
	else if(GET != 0)
	{
		return http_write(p[2], buf, len);
	}

	//chat
	else
	{
		p[1] = 1;
	}

byebye:
	return 1;
}
