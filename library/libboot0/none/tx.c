#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdarg.h>




void say(char* fmt , ...)
{
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
	//printf(fmt);
}




void deleteserial()
{
}
void createserial()
{
}
