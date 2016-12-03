#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int diary(char*, int, char*, ...);
void printmemory(char*,int);
void say(char*, ...);




//why,what,where,when
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
