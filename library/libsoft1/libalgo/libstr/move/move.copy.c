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




int copypath(u8* path, u8* data)
{
	int j;
	for(j=0;j<127;j++){
		if(data[j] < 0x20)break;
		path[j] = data[j];
	}
	path[j] = 0;
	return j;
}