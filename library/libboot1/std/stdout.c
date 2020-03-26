#define s8 char
#define s16 short
#define s32 int
#define s64 long long
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int myvsnprintf(u8* buf, int len, u8* fmt, __builtin_va_list arg);
int mysnprintf(u8* buf, int len, u8* fmt, ...);
void lowlevel_input(void*);
void lowlevel_output(void*, int);




static void* outputqueue;
static int outcur;
void initstdout(void* addr)
{
	outputqueue = addr;
	outcur = 0;
}
void freestdout()
{
}
void* getstdout()
{
	return outputqueue;
}
int getcurout()
{
	return outcur;
}




void say(u8* fmt, ...)
{
	int cur,ret;
	__builtin_va_list arg;

	//read position
	cur = outcur;

	//
	__builtin_va_start(arg, fmt);

	//snprintf
	//ret = diary(outputqueue+cur, 0x1000, fmt, arg, cur%0x80);
	ret = myvsnprintf(outputqueue+cur, 0x1000, fmt, arg);

	//
	__builtin_va_end(arg);

	//debugport
	//printout(cur, ret);
	lowlevel_output(outputqueue+cur, ret);

	//write position
	cur = cur+ret;
	if(cur > 0x3f000)cur = 0;
	outcur = cur;
}
void printbigint(u8* buf, int len)
{
	int j;
	if(len<=0)return;

	say((void*)"0x");
	for(j=len-1;j>=0;j--)say((void*)"%02x", buf[j]);
}
void printmemory(u8* buf, int len)
{
	u8 c;
	int j,k;
	u8 tmp[128];

	while(1)
	{
		if(len <= 0)break;
		if(len <= 16)k = len;
		else k = 16;

		j = mysnprintf(tmp, 80, (void*)"@%-13llx", buf);
		for(;j<80;j++)tmp[j]=0x20;

		for(j=0;j<k;j++)
		{
			c = ((buf[j]>>4)&0xf)+0x30;
			if(c > 0x39)c += 7;
			tmp[(3*j) + 14] = c;

			c = (buf[j]&0xf)+0x30;
			if(c > 0x39)c += 7;
			tmp[(3*j) + 15] = c;

			c = buf[j];
			if((c<0x20)|(c>=0x7f))c = '.';
			tmp[14+48+j] = c;
		}
		say((void*)"%.*s\n", 14+48+16, tmp);

		buf += 16;
		len -= 16;
	}
}
