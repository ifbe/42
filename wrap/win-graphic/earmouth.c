#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

#include<stdio.h>
#include<stdlib.h>

static unsigned char* logbuf;
static QWORD inneroffset;


__attribute__((constructor)) void initlog()
{
	logbuf=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void destorylog()
{
	free(logbuf);
}








void whereislogbuf(unsigned long long* p)
{
	*p=(unsigned long long)logbuf;
}/*
void waitinput(char* p)
{
	int i;
	for(i=0;i<128;i++)p[i]=0;
	while(1)
	{
		gets(p);
		if( p[0] != 0 )break;
	}
}*/
void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	printf(rcx,rdx,r8,r9);
/*
	int i=0;
	while(1)
	{
		if(rcx[i]==0)break;
		else logbuf[inneroffset+i]=rcx[i];

		i++;
	}
*/
	int i=0;
	while(1)
	{
		if(rcx[i] == 0)break;
		int huanhang;
		snprintf(logbuf+inneroffset,0x80,rcx,rdx,r8,r9);
		
	}
	inneroffset=(inneroffset+0x80)%0x100000;
	//printf("inneroffset:%llx\n",inneroffset);
}