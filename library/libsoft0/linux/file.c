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
#include <errno.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void diary(char* fmt,...);

static BYTE* diskinfo;
static int thisfd=-1;
static char diskname[0x20]={'/','d','e','v','/','s','d','a','\0','\0'};




void listfile()
{
	//clean
	struct stat st;
	int i;

	int tempfd;
	int num;
	char* dest=0;
	for(i=0;i<0x10000;i++)
	{
		diskinfo[i]=0;
	}

	//enumerate
	dest=diskinfo;
	for(num=0;num<10;num++)
	{
		diskname[7]=num+'a';
		tempfd = open(diskname,O_RDONLY);
		if(tempfd == -1)break;

		//[0,7]:type
		//[8,f]:subtype
		*(QWORD*)(dest+0)=0x6b736964;
		*(QWORD*)(dest+0x8)=0x3f;

		//size
		stat(diskname,&st);
		*(QWORD*)(dest+0x10)=0;
		*(QWORD*)(dest+0x18)=st.st_size;

		//[0x20,0x3f]:name
		for(i=0;i<0x20;i++)dest[0x20+i]=diskname[i];

		//next
		//printf("%llx,%llx:%s\n",*(QWORD*)(dest+0),*(QWORD*)(dest+8),(char*)(dest+0x10) );
		dest += 0x40;
		close(tempfd);
	}
}




void intofile(char* wantpath)
{
	//先检查再打开新的，否则新的不行老的已去。。。
	//testopen
	int tempfd=open(wantpath,O_RDONLY | O_LARGEFILE);
	if(tempfd == -1)
	{
		diary("can't open:%s\n",wantpath);
		return;
	}
	else close(tempfd);

	//realopen
	if(thisfd!=-1)close(thisfd);
	thisfd=open(wantpath,O_RDONLY | O_LARGEFILE);
}




void readfile(QWORD buf,QWORD sector,QWORD disk,DWORD count)
{
	//disk暂时根本不管是什么，默认就是当前第一个硬盘
	int result;
	result=lseek64(thisfd,sector*0x200,SEEK_SET);
	if(result==-1)
	{
		diary("errno:%d,seek:%llx\n",errno,sector);
		return;
	}

	result=read(thisfd,(void*)buf,count*0x200);
	if(result==-1)
	{
		diary("errno:%d,read:%llx,%llx\n",errno,sector,count);
	}
}




//来源内存地址，目的首扇区，无视，总字节数
void writefile(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	
}




//mem地址，file名字，文件内偏移，总字节数
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	return 0;
}




int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	return 0;
}








void initfile(QWORD addr)
{
	diskinfo=(void*)addr;
	listfile();
}
void killfile()
{
	if(thisfd != -1)
	{
		close(thisfd);
		thisfd=-1;
	}
}
