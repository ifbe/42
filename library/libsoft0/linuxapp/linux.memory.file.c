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
#include "libsoft.h"





struct item* obj;
void initfilemgr(void* addr)
{
	obj = addr;
}
void freefilemgr()
{
}
void filemanager_registerdisk()
{
}




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
int file_search()
{
	say("@filesearch\n");
	return 0;
}
int file_modify()
{
	return 0;
}




_obj* file_create(void* path, int flag)
{
	int fd;
	char* pp = path;
	if(0 == pp){fd = -0xfff;goto fail;}
	if(0 == pp[0]){fd = -0xffe;goto fail;}

	if('w' == flag){
		fd = open(path, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	}
	else{
		fd = open(path, O_RDONLY | O_LARGEFILE);
	}
	if(fd < 0)goto fail;

	obj[fd].fileinfo.fd = fd;
	return &obj[fd];

fail:
	say("@open:fd=%d,err=%d,path=%s\n", fd, errno, pp);
	return 0;
}
int file_delete(_obj* oo)
{
	int fd = oo->fileinfo.fd;
	return close(fd);
}
int file_reader(_obj* oo, int xx, p64 arg, int cmd, void* buf, int len)
{
	int ret;
	int fd = oo->fileinfo.fd;

	if(_pos_ == cmd){
		ret = lseek64(fd, arg, SEEK_SET);
		if(-1 == ret){
			//say("lseek64: offs=%llx, errno=%d\n", arg, errno);
			return -2;
		}
	}//from head

	ret = read(fd, buf, len);
	if(-1 == ret){
		//say("read: offs=%llx, len=%x, errno=%d\n", arg, len, errno);
		return -1;
	}

	return ret;
}
int file_writer(_obj* oo, int xx, p64 arg, int cmd, void* buf, int len)
{
	int ret;
	int fd = oo->fileinfo.fd;

	if(_pos_ == cmd){
		ret = lseek64(fd, arg, SEEK_SET);
		if(-1 == ret){
			//say("lseek64: offs=%llx, errno=%d\n", arg, errno);
			return -2;
		}
	}//from head

	ret = write(fd, buf, len);
	if(-1 == ret){
		//say("write: offs=%llx, len=%x, errno=%d\n", arg, len, errno);
		return -1;
	}

	return ret;
}
