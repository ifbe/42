#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
char* listenbuf=0;
void initlisten(char* addr)
{
	listenbuf=addr;
}




void waitinput(char* p)
{
	int i;
	char* ignore;
	for(i=0;i<128;i++)p[i]=0;
	while(1)
	{
		ignore=fgets(p,128,stdin);
		if( p[0] != 0 )break;
	}
	for(i=0;i<128;i++)
	{
		if(p[i]<=0xd)p[i]=0;
	}
}

