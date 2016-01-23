#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void printmemory(char* addr,int count);
void say(char* fmt,...);




static char buffer[100];
char* explainarg()
{
	//clean
	char* result=0;
	int temp;
	for(temp=0;temp<100;temp++)buffer[temp]=0;

	//open,read,close
	temp=open("/proc/self/cmdline",O_RDONLY);
	if(temp==-1)say("error reading cmd line\n");
	else
	{
		memset(buffer,0,100);
		int i=read(temp,buffer,100);
		close(temp);
		printmemory(buffer,0x20);
		//say("cmdline:%s\n",buffer);
	}

	//
	for(temp=0;temp<100;temp++)
	{
		if( buffer[temp] == 0)
		{
			if( buffer[temp+1] != 0 )
			{
				result=buffer+temp+1;
			}
		}
	}

	printf("%llx\n",(QWORD)result);
	return result;
}
