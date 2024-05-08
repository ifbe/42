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



/*
struct item* obj;
void filemanager_registerdisk()
{
}*/
void initosfile(void* addr)
{
}
void freeosfile()
{
}


int file_search(void* buf, int len)
{
	logtoall("@filesearch\n");
	return 0;
}
int file_modify(void* buf, int len)
{
	return 0;
}




void* system_fd2obj(int fd);
_obj* file_create(void* path, int flag)
{
	//logtoall("@startfile:%s,%x\n", path, flag);
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

	_obj* oo = system_fd2obj(fd);
	//logtoall("file_create:obj=%p,fd=%d\n",&obj[fd], fd);
	if(0 == oo)return 0;

	oo->fileinfo.fd = fd;
	return oo;

fail:
	printf("@open:fd=%d,err=%d,path=%s\n", fd, errno, pp);
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
	//logtoall("obj=%p,fd=%d\n", oo, fd);

	if(_pos_ == cmd){
		ret = lseek(fd, arg, SEEK_SET);
		if(-1 == ret){
			//logtoall("lseek64: offs=%llx, errno=%d\n", arg, errno);
			return -2;
		}
	}//from head

	ret = read(fd, buf, len);
	if(-1 == ret){
		//logtoall("read: offs=%llx, len=%x, errno=%d\n", arg, len, errno);
		return -1;
	}

	return ret;
}
int file_writer(_obj* oo, int xx, p64 arg, int cmd, void* buf, int len)
{
	int ret;
	int fd = oo->fileinfo.fd;

	if(_pos_ == cmd){
		ret = lseek(fd, arg, SEEK_SET);
		if(-1 == ret){
			//logtoall("lseek64: offs=%llx, errno=%d\n", arg, errno);
			return -2;
		}
	}//from head

	ret = write(fd, buf, len);
	if(-1 == ret){
		//logtoall("write: offs=%llx, len=%x, errno=%d\n", arg, len, errno);
		return -1;
	}

	return ret;
}
