#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

#include<stdio.h>
#include<stdlib.h>

static unsigned char* buf1d;
static QWORD offset;


__attribute__((constructor)) void initlog()
{
	buf1d=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void destorylog()
{
	free(buf1d);
}









void say(char* p,...)
{
	asm("jmp printf");
}/*
void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	sprintf(buf1d+offset,rcx,rdx,r8);
	offset+=0x80;
}*/
void waitinput(char* p)
{
	int i;
	for(i=0;i<128;i++)p[i]=0;
	while(1)
	{
		gets(p);
		if( p[0] != 0 )break;
	}
}