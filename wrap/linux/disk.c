//听说必须在头文件的前面
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char


static BYTE* diskinfo;
static char diskname[10]={'/','d','e','v','/','s','d','a','\0','\0'};
int fd;

void enumeratedisk()
{
	//clean
	int i=0,j=0;
	for(i=0;i<0x100*10;i++)
	{
		diskinfo[i]=0;
	}

	//enumerate
	FILE* tempfd;
	for(i=0;i<10;i++)
	{
		diskname[7]=i+'a';
		printf(diskname);
		tempfd = open(diskname,O_RDONLY | O_LARGEFILE);
		if(tempfd != NULL)
		{
			printf("%d    %s\n",i,diskname);
			close(tempfd);

			*(QWORD*)(diskinfo+0x100*j)=*(QWORD*)diskname;
			*(WORD*)(diskinfo+0x100*j+8)=*(WORD*)(diskname+8);
			j++;
		}
	}
}




void choosedisk(char* wantpath)
{
	//testopen
	//int tempfd=open();

	//realopen
	//if(fd!=0)open();
}
void readdisk(QWORD buf,QWORD sector,QWORD disk,DWORD count)
{
	int result;
	//printf("read to %llx,from %llx,result:%d\n",buf,sector,result);

	//disk暂时根本不管是什么，默认就是当前第一个硬盘
	result=lseek64(fd,sector*0x200,SEEK_SET);
	printf("seek:%llx\n",result);

	result=read(fd,(void*)buf,count*0x200);
	printf("read:%llx\n",result);
}
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count)
{

}








void initdisk()
{
	whereisdiskinfo(&diskinfo);
	say("inited disk\n");
}
void destorydisk()
{
	close(fd);
}
