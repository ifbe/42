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

static char diskname[10]={'/','d','e','v','/','s','d','a','\0','\0'};
static char status[10];
int fd;

/*
static void enumeratedisk()
{
	printf(diskname);

	FILE* tempfd;
	char i=0;
	for(i=0;i<10;i++)
	{
		diskname[7]=i+'a';
		printf(diskname);
		fd = open(diskname, );
		if(fd != NULL)
		{
			close(tempfd);
			status[i]=1;
			printf("%d    %s\n",i,diskname);
		}
		else
		{
			status[i]=0;
		}
	}
}
*/
__attribute__((constructor)) void initdisk()
{
	fd = open(diskname,O_RDONLY | O_LARGEFILE);
}
__attribute__((destructor)) void destorydisk()
{
	close(fd);
}




void disk(QWORD addr)
{
	//如果是空的，那就只扫描一遍有哪些物理磁盘然后打印一遍就返回不往下走
	if(*(DWORD*)addr == 0xffffffff)
	{
		//enumeratedisk();
		return;
	}

	//收到的地址里面究竟是些什么
	QWORD i=*(DWORD*)addr;

	//如果是小于10的数字
	if(i<10)
	{
	}
	else
	{
	}
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
