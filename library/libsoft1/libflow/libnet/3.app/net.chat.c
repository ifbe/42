#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64 check_http(void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void say(void*, ...);




#define chat 0x74616863
#define TLS 0x534c54
#define SSH 0x485353
#define SOCKS 0x534b434f53
u64 serve_first(u64 fd, u64 type, char* buf, int len)
{
	u64 ret;

	//tls server's connection
	if(buf[0] == 0x16)
	{
		return TLS;
	}

	//ssh server's connection
	ret = ncmp(buf, "SSH-2.0-", 8);
	if(ret == 0)
	{
		return SSH;
	}

	//socks server's connection
	if(0)
	{
		return SOCKS;
	}

	//1?:	http or ws
	ret = check_http(buf, len);
	if(ret > 0)
	{
		return ret;
	}

	//default: debug
	return chat;
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
	return type;
}
