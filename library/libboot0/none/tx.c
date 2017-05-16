#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdarg.h>




/*
int fmt(char* mem, int max, char* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say(char* fmt , ...)
{
	va_list args;

	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}
*/




int lowlevel_input(char* buf)
{
    return 0;
}
void lowlevel_output(char* buf, int len)
{
	int j;
	for(j=0;j<len;j++)printf("%c",buf[j]);
}
void deleteserial()
{
}
void createserial()
{
}
