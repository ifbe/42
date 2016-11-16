#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




//
void copy(char* dst, char* src)
{
	int j=0;
	while(1)
	{
		dst[j] = src[j];
		if(src[j] == 0)break;

		j++;
	}
}
void ncopy(char* dst, char* src, int max)
{
	int j;
	for(j=0;j<max;j++)
	{
		dst[j] = src[j];
		if(src[j] == 0)break;
	}
}
void movsb(u8* rdi, u8* rsi, int rcx)
{
	int j;
	if(rdi < rsi)
	{
		for(j=0;j<rcx;j++)
		{
			rdi[j] = rsi[j];
		}
	}
	else
	{
		for(;rcx>0;rcx--)
		{
			rdi[rcx-1] = rsi[rcx-1];
		}
	}
}
