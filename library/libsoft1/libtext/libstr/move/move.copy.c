#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void logtoall(char*,...);




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




int copypath(u8* dst, u8* src)
{
	int j;
	for(j=0;j<127;j++){
		if(src[j] < 0x20)break;
		dst[j] = src[j];
	}
	dst[j] = 0;
	return j;
}
int copyfourcc(u8* dst, u8* src)
{
	int j;
	for(j=0;j<4;j++)dst[j] = 0;
	for(j=0;j<4;j++){
		if( ('_' == src[j]) |
			( (src[j] >= '0')&&(src[j] <= '9') )|
			( (src[j] >= 'A')&&(src[j] <= 'Z') )|
			( (src[j] >= 'a')&&(src[j] <= 'z') ))
		{
			dst[j] = src[j];
		}
		else break;
	}
	return j;
}
int copyeightcc(u8* dst, u8* src)
{
	int j;
	for(j=0;j<8;j++)dst[j] = 0;
	for(j=0;j<8;j++){
		if( ('_' == src[j]) | (',' == src[j]) |
			( (src[j] >= '0')&&(src[j] <= '9') )|
			( (src[j] >= 'A')&&(src[j] <= 'Z') )|
			( (src[j] >= 'a')&&(src[j] <= 'z') ))
		{
			dst[j] = src[j];
		}
		else break;
	}
	return j;
}