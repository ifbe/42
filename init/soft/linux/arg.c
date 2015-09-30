#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void diary(char* fmt,...);




static char buffer[100];
char* explainarg()
{
	//cmdline
	int temp;
	for(temp=0;temp<100;temp++)buffer[temp]=0;
	temp=open("/proc/self/cmdline",O_RDONLY);
	if(temp==-1)diary("error reading cmd line\n");
	else
	{
		memset(buffer,0,100);
		int i=read(temp,buffer,100);
		close(temp);
		printmemory(buffer,20);
		//diary("cmdline:%s\n",buffer);
	}

	//
	for(temp=0;temp<100;temp++)
	{
		if(buffer[temp]==0)break;
	}
	//diary("    arg0:%s,arg1:%s\n",buffer,buffer+temp);

	if( buffer[temp+1] != 0 )return buffer+temp+1;
	else return "/dev/sda";
}
