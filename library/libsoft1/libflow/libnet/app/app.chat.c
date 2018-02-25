#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printmemory(void*, int);
void say(void*, ...);




#define CHAT 0x54414843
#define chat 0x74616863
int chat_client(void* p, int fd, char* buf, int len)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] < 0x20)k=1;break;
		if(buf[j] > 0x7e)k=1;break;
	}

	if(k == 0)say("[%llx]%.*s", fd, len, buf);
	else printmemory(buf, len);

	return chat;
}
int chat_server(void* p, int fd, char* buf, int len)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if(buf[j] < 0x20)k=1;break;
		if(buf[j] > 0x7e)k=1;break;
	}

	if(k == 0)say("[%llx]%.*s", fd, len, buf);
	else printmemory(buf, len);

	return CHAT;
}