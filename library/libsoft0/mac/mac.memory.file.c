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
void filemanager_registersupplier()
{
}




_obj* file_create(void* path, int flag)
{
	//say("@startfile:%s,%x\n", path, flag);
	int fd;
	char* pp = path;
	if(0 == pp){fd = -0xfff;goto fail;}
	if(0 == pp[0]){fd = -0xffe;goto fail;}

	if('w' == flag){
		fd = open(pp, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	}
	else{
		fd = open(pp, O_RDONLY);
	}
	if(fd < 0)goto fail;

	obj[fd].fileinfo.fd = fd;
	return &obj[fd];

fail:
	printf("@open:fd=%d,err=%d,path=%s\n", fd, errno, pp);
	return 0;
}
int file_delete(_obj* oo)
{
	int fd = oo->fileinfo.fd;
	return close(fd);
}
int file_search(void* buf, int len)
{
	say("@filesearch\n");
	return 0;
}
int file_modify(void* buf, int len)
{
	return 0;
}




int file_link()
{
	return 0;
}
int file_gone()
{
	return 0;
}
int file_take(_obj* oo, int xx, void* arg, int off, void* buf, int len)
{
	int ret;
	int fd = oo->fileinfo.fd;
	say("fd=%d\n");

	if(arg){
		ret = lseek(fd, off, SEEK_SET);
		if(-1 == ret){
			//say("errno:%d,seek:%llx\n", errno, off);
			return -2;
		}
	}//from head

	ret = read(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}
int file_give(_obj* oo, int xx, void* arg, int off, void* buf, int len)
{
	int ret;
	int fd = oo->fileinfo.fd;

	if(arg){
		ret = lseek(fd, off, SEEK_SET);
		if(-1 == ret){
			//say("errno:%d,seek:%llx\n", errno, off);
			return -2;
		}
	}//from head

	ret = write(fd, buf, len);
	if(-1 == ret)
	{
		//say("errno:%d,read:%llx,%llx\n", errno, off, len);
		return -1;
	}

	return ret;
}