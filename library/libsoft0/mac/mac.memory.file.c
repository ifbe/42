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




int readfile(void* obj, int fd, void* arg, int off, char* buf, int len)
{
	int ret;

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
int writefile(void* obj, int fd, void* arg, int off, char* buf, int len)
{
	int ret;

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
int stopfile(int fd)
{
	return close(fd);
}
int startfile(char* path, int flag)
{
	int ret;
	//say("@startfile:%s,%x\n", path, flag);
	if(0 == path){ret = -0xfff;goto fail;}
	if(0 == path[0]){ret = -0xffe;goto fail;}

	if('w' == flag){
		ret = open(path, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	}
	else{
		ret = open(path, O_RDONLY);
	}
	if(ret > 0)return ret;

fail:
	printf("%d,%d@open:%s\n", ret, errno, path);
	return ret;
}




int filesearch(void* buf, int len)
{
	say("@filesearch\n");
	return 0;
}
int filemodify(void* buf, int len)
{
	return 0;
}
void deletefile()
{
}
void createfile()
{
}




void filemanager_registersupplier()
{
}
void initfilemgr()
{
}
void freefilemgr()
{
}
