#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void target(char*);







void explainarg()
{
	//cmdline
	char buffer[100];
	int temp;
	for(temp=0;temp<100;temp++)buffer[temp]=0;
	temp=open("/proc/self/cmdline",O_RDONLY);
	if(temp==-1)diary("error reading cmd line\n");
	else
	{
		int i=read(temp,buffer,100);
		close(temp);
		//printmemory(buffer,100);
		diary("cmdline:%s\n",buffer);
	}

	//
	int signal=0;
	for(temp=0;temp<100;temp++)
	{
		if(buffer[temp]==0)
		{
			signal=1;
		}
		else            //!=0
		{
			if(signal==1)
			{
				signal=2;
				break;
			}
		}
	}
	//diary("    arg0:%s,arg1:%s\n",buffer,buffer+temp);

	if(signal==2)target(buffer+temp);
	else target("/dev/sda");
}
