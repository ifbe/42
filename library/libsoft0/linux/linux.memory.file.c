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
int writefile(u64 fd, u64 off, char* buf, u64 len)
{
	int ret;

	ret=lseek64(fd, off, SEEK_SET);
	if(-1 == ret)
	{
		//say("errno:%d,seek:%llx\n", errno, off);
		return -2;
	}

	ret = write(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}
int readfile(u64 fd, u64 off, char* buf, u64 len)
{
	int ret;

	ret = lseek64(fd, off, SEEK_SET);
	if(-1 == ret)
	{
		//say("errno:%d,seek:%llx\n", errno, off);
		return -2;
	}

	ret = read(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}
int stopfile(int fd)
{
	return close(fd);
}
int startfile(char* path, int flag)
{
	if(path == 0)return -3;
	if(path[0] == 0)return -2;

	if('w' == flag)flag = O_CREAT;
	flag |= O_RDWR | O_LARGEFILE;
	return open(path, flag, S_IRWXU|S_IRWXG|S_IRWXO);
}
void deletefile()
{
}
void createfile()
{
}
