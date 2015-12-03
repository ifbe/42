#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdarg.h>
static char* diarybuf=0;
void initdiary()
{
}




void diary(char* fmt , ...)
{
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}




void printmemory(QWORD addr,int size)
{
//debug用，打印从addr开始的总共size个字节
	BYTE* printaddr=(BYTE*)addr;
	int i,j;

	//打印的很少就不显示这些了
	if(size>0x10)
	{
		diary("[----addr----]");
		for(i=0;i<=0xf;i++)
		{
			diary("%2x ",i);
		}
		diary("[----anscii----]\n");
	}

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)diary("@%-13llx",addr+j*16);
		else diary("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			diary("%.2x ",printaddr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			unsigned char ch=printaddr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			diary("%c",ch);
		}
		diary("\n",printaddr[16*j+15]);
	}
}
