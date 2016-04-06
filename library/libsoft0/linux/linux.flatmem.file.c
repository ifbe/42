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
void say(char* fmt,...);




//////////////
static int thisfd=-1;
static char _dev_sd[0x20]={
	'/','d','e','v','/','s','d','a',0,0
};
static char _dev_mmcblk[0x20]={
	'/','d','e','v','/','m','m','c','b','l','k','0',0,0
};
static char _dev_block_mmcblk[0x20]={
	'/','d','e','v','/','b','l','o','c','k','/','m','m','c','b','l','k','0',0,0
};




//mem地址，file名字，文件内偏移，总字节数
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	int thisfile;
	int ret;
	thisfile=open(filename,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
	if(thisfile==-1)
	{
		printf("(mem2file fail)open\n");
		return -1;
	}

	ret=lseek( thisfile , offset , SEEK_SET);
	if(ret==-1)
	{
		printf("(mem2file fail)lseek\n");
		return -2;
	}

	ret=write( thisfile , memaddr , count);
	if(ret==-1)
	{
		printf("(mem2file fail)write\n");
	}
	//printmemory(memaddr,0x200);

	close(thisfile);
	return 0;
}
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	return 0;
}
static int trythis(char* src,char* dest)
{
	int i;
	int ret;
	struct stat st;

	//使用stat去得到是否存在以及文件大小
	//say("%s\n",src);
	ret=stat(src , &st);
	if(ret<0)return 0;

	//[0,7]:type
	//[8,f]:subtype
	*(QWORD*)(dest+0)=0x6b736964;
	*(QWORD*)(dest+0x8)=0x3f;

	//size
	*(QWORD*)(dest+0x10)=0;
	*(QWORD*)(dest+0x18)=st.st_size;

	//[0x20,0x3f]:name
	for(i=0;i<0x20;i++)dest[0x20+i]=src[i];

	//success,next
	return 0x40;
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void listfile(char* dest)
{
	//clean
	int num;
	for(num=0;num<0x10000;num++)
	{
		dest[num]=0;
	}

	//		/dev/sd?
	for(num=0;num<10;num++)
	{
		_dev_sd[0x7]=num + 'a';
		dest += trythis(_dev_sd , dest);
	}

	//		/dev/mmcblk?
	for(num=0;num<10;num++)
	{
		_dev_mmcblk[0xb]=num + '0';
		dest += trythis(_dev_mmcblk , dest);
	}

	//		/dev/block/mmcblk?
	for(num=0;num<10;num++)
	{
		_dev_block_mmcblk[0x11]=num + '0';
		dest += trythis(_dev_block_mmcblk , dest);
	}

	//		special
	dest += trythis("/dev/xvda" , dest);
}
void switchfile()
{
}
int readfile(char* buf,QWORD sector,QWORD disk,DWORD count)
{
	int result;

	result=lseek64(thisfd,sector*0x200,SEEK_SET);
	if(result==-1)
	{
		//say("errno:%d,seek:%llx\n",errno,sector);
		return -1;
	}

	result=read(thisfd,buf,count*0x200);
	if(result==-1)
	{
		//say("errno:%d,read:%llx,%llx\n",errno,sector,count);
		return -2;
	}

	//
	return 1;
}
//来源内存地址，目的首扇区，无视，总字节数
void writefile(char* buf,QWORD startsector,QWORD ignore,DWORD count)
{
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








void initfile()
{
}
void killfile()
{
	if(thisfd != -1)
	{
		close(thisfd);
		thisfd=-1;
	}
}
int startfile(char* wantpath)
{
	//先检查
	if(wantpath[0]==0)return -1;

	//测试打开新的
	int tempfd=open(wantpath,O_RDONLY | O_LARGEFILE);
	if(tempfd == -1)
	{
		//say("(openfile error)%s\n",wantpath);
		return -2;
	}
	else close(tempfd);

	//真正打开新的
	if(thisfd!=-1)close(thisfd);
	thisfd=open(wantpath,O_RDONLY | O_LARGEFILE);

	//
	//say("thisfd=%d\n",thisfd);
	return 1;
}
void stopfile()
{
	if(thisfd!=-1)
	{
		close(thisfd);
		thisfd=-1;
	}
}
