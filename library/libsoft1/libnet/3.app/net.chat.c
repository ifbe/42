#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int http_read(u8*, int);
int ncmp(u8*,u8*,int);
int cmp(u8*,u8*);
//
int diary(char*, int, char*, ...);
void printmemory(char*,int);
void say(char*, ...);




//why,what,where,when
int serve_first(u64* p, char* buf, int len)
{
	//2?:	https or wss
	if(buf[0] == 0x16)
	{
		p[1] = 0x20;
	}

	//4?:	socks
	else if(0)
	{
		p[1] = 0x40;
	}

	//3?:	shell or proxy
	else if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		p[1] = 0x30;
	}

	//1?:	http or ws
	else if(http_read(buf, len) > 0)
	{
		p[1] = 0x10;
	}

	//default: debug
	else
	{
		p[1] = 1;
	}

	//
	return 1;
}




int serve_chat(u64* p, char* buf, int len)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] < 0x20)k=1;break;
		if(buf[j] > 0x7e)k=1;break;
	}

	//
	if(k == 0)say("[%llx]%s",p[2], buf);
	else printmemory(buf,len);

	//
	return 1;
}
