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

#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
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
	*(u64*)(dest+0)=0x6b736964;
	*(u64*)(dest+0x8)=0x3f;

	//size
	*(u64*)(dest+0x10)=0;
	*(u64*)(dest+0x18)=st.st_size;

	//[0x20,0x3f]:name
	for(i=0;i<0x20;i++)dest[0x20+i]=src[i];

	//success,next
	return 0x40;
}




//file名字，mem地址，文件内偏移，总字节数
int writefile(u8* file, u8* mem, u64 offset, u64 count)
{
	int fd;
	int ret;

	if(file == 0)
	{
		ret=lseek64(thisfd, offset, SEEK_SET);
		if(ret==-1)
		{
			//say("errno:%d,seek:%llx\n",errno,sector);
			return -2;
		}

		ret=write(thisfd, mem, count);
		if(ret==-1)
		{
			//say("errno:%d,read:%llx,%llx\n",errno,sector,count);
			return -1;
		}
	}
	else
	{
		fd = open(file, O_WRONLY|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
		if(fd==-1)
		{
			printf("fail@open\n");
			return -3;
		}

		ret=lseek64(fd, offset, SEEK_SET);
		if(ret==-1)
		{
			printf("fail@lseek\n");
			close(fd);
			return -2;
		}

		ret=write(fd, mem, count);
		if(ret==-1)
		{
			printf("fail@write\n");
			close(fd);
			return -1;
		}

		close(fd);
	}

	//
	return ret;
}
int readfile(u8* file, u8* mem, u64 offset, u64 count)
{
	int fd;
	int ret;

	if(file == 0)
	{
		ret=lseek64(thisfd, offset, SEEK_SET);
		if(ret==-1)
		{
			//say("errno:%d,seek:%llx\n",errno,sector);
			return -2;
		}

		ret=read(thisfd, mem, count);
		if(ret==-1)
		{
			//say("errno:%d,read:%llx,%llx\n",errno,sector,count);
			return -1;
		}
	}
	else
	{
		fd = open(file, O_RDONLY);
		if(fd==-1)
		{
			printf("fail@open\n");
			return -3;
		}

		ret = lseek64(fd, offset, SEEK_SET);
		if(ret==-1)
		{
			printf("fail@lseek\n");
			close(fd);
			return -2;
		}

		ret = read(fd, mem, count);
		if(ret==-1)
		{
			printf("fail@write\n");
			close(fd);
			return -1;
		}

		close(fd);
	}

	//
	return ret;
}




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
void choosefile()
{
}




int startfile(char* path)
{
	//先检查
	if(path == 0)return -3;
	if(path[0] == 0)return -2;

	//测试打开新的
	int tempfd=open(path,O_RDONLY | O_LARGEFILE);
	if(tempfd == -1)
	{
		//say("(openfile error)%s\n",path);
		return -1;
	}
	else close(tempfd);

	//真正打开新的
	if(thisfd != -1)close(thisfd);
	thisfd=open(path,O_RDONLY | O_LARGEFILE);

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
void createfile()
{
}
void deletefile()
{
	if(thisfd != -1)
	{
		close(thisfd);
		thisfd=-1;
	}
}
