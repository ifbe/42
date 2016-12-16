#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int check_http(void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int diary(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);




//why,what,where,when
int serve_first(u64* p, char* buf, int len)
{
	int ret;

	//2?:	https or wss
	if(buf[0] == 0x16)
	{
		p[1] = 0x20;
		return 0x20;
	}

	//4?:	socks
	if(0)
	{
		p[1] = 0x40;
		return 0x40;
	}

	//3?:	shell or proxy
	ret = ncmp(buf, "SSH-2.0-", 8);
	if(ret == 0)
	{
		p[1] = 0x30;
		return 0x30;
	}

	//1?:	http or ws
	ret = check_http(buf, len);
	if(ret > 0)
	{
		p[1] = ret;
		return ret;
	}

	//default: debug
	p[1] = 1;
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
