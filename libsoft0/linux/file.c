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
static char diskname[10]={'/','d','e','v','/','s','d','a','\0','\0'};




//mem地址，file名字，文件内偏移，总字节数
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	return 0;
}
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count)
{
	return 0;
}








void filelist()
{
	//clean
	int tempfd;
	struct stat st;
	QWORD source=0,dest=0;
	for(dest=0;dest<0x100*10;dest++)
	{
		diskinfo[dest]=0;
	}

	//enumerate
	dest=(QWORD)diskinfo;
	for(source=0;source<10;source++)
	{
		diskname[7]=source+'a';
		//printf("diskname:%s\n",diskname);
		tempfd = open(diskname,O_RDONLY);
		if(tempfd == -1)break;
		else
		{
			close(tempfd);

			//[0,7]:id
			*(QWORD*)(dest+0)=source;

			//[8,f]:size
			stat(diskname,&st);
			*(QWORD*)(dest+8)=st.st_size;

			//[0x10,0xff(0x3f)]:name
			*(QWORD*)(dest+0x10)=*(QWORD*)diskname;
			*(WORD*)(dest+0x18)=0;

			//next
			printf( "%x,%x:%s\n" , *(QWORD*)(dest+0) , *(QWORD*)(dest+8) , (char*)(dest+0x10) );
			dest += 0x100;
		}
	}
}
void filetarget(char* wantpath)
{
	//testopen
	int tempfd=open(wantpath,O_RDONLY | O_LARGEFILE);
	if(tempfd == -1)
	{
		diary("can't open:%s\n",wantpath);
	}
	else close(tempfd);

	//realopen
	if(thisfd!=-1)close(thisfd);
	thisfd=open(wantpath,O_RDONLY | O_LARGEFILE);
}
void fileread(QWORD buf,QWORD sector,QWORD disk,DWORD count)
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
void filewrite(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	
}








void fileinit(QWORD addr)
{
	diary("file initing\n");

	diskinfo=(void*)addr;
	filelist();
}
void filekill()
{
	if(thisfd != -1)close(thisfd);
}
