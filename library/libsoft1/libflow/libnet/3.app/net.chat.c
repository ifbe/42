#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define tls_new 0x300
#define ssh_new 0x500
#define socks_new 0x600
//
int check_http(void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




int serve_first(u64 fd, u64 type, char* buf, int len)
{
	int ret;

	//https or wss
	if(buf[0] == 0x16)
	{
		return tls_new;
	}

	//shell or proxy
	ret = ncmp(buf, "SSH-2.0-", 8);
	if(ret == 0)
	{
		return ssh_new;
	}

	//socks
	if(0)
	{
		return socks_new;
	}

	//1?:	http or ws
	ret = check_http(buf, len);
	if(ret > 0)
	{
		return ret;
	}

	//default: debug
	return 1;
}




int serve_chat(u64 fd, u64 type, char* buf, int len)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] < 0x20)k=1;break;
		if(buf[j] > 0x7e)k=1;break;
	}

	//
	if(k == 0)say("[%llx]%s", fd, buf);
	else printmemory(buf, len);

	//
	return 1;
}
