#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void logtoall(char*,...);




//比如aa zec，得到aa\0ec
void blank2zero(char* addr,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(addr[i] == 0x20)
		{
			addr[i]=0;
		}
	}
}




//比如ac\0be，得到ac be
void zero2blank(char* addr,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(addr[i] == 0)
		{
			addr[i]=0x20;
		}
	}
}




//比如abcefg，变成ABCEFG
void lower2upper(char* addr,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(addr[i]>='a' && addr[i]<='z')
		{
			addr[i] -= 0x20;
		}
	}
}




//比如ABCEFG，变成abcefg
void upper2lower(char* addr,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(addr[i]>='A' && addr[i]<='Z')
		{
			addr[i] += 0x20;
		}
	}
}
