#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>




static unsigned char* world;
static unsigned char* screen;




__attribute__((constructor)) void initeverything()
{
	world = (unsigned char*)malloc(0x800000);		//8M
	screen = (unsigned char*)malloc(0x400000);		//8M

	//
	initlog(world+0x500000);
	say("beforemain(){\n");
	say("inited memory\n");

	//只是拿地址
	initdisk();
	enumeratedisk();

	//
	initwindow(screen);

	//initarg
	explainarg();

	say("}\n");
}
__attribute__((destructor)) void killeverything()
{
	killwindow();
	killdisk();
	free(world);
	free(screen);
}




unsigned long long whereisworld()
{
	return (unsigned long long)world;
}
unsigned long long whereisscreen()
{
	return (unsigned long long)screen;
}
/*
static unsigned char* diskbuf;
static unsigned char* logbuf;
static unsigned char* realworld;
	//buffer0@[+0,+0xffff]:		1个硬盘(或者虚拟磁盘文件) = 很多个分区(ext/fat/hfs/ntfs)
	//buffer1@[+0x10000,+0x1ffff]:		1个分区(或者某格式的文件) = 很多个区域(头/索引区/数据区/尾)
	//buffer2@[+0x20000,+0x2ffff]:		1个索引(mft/inode/btnode) = 很多信息(名字，唯一id，时间，扇区位置等)
	//buffer3@[+0x30000,+0x3ffff]:		未用
static unsigned char* logicworld;
	//readbuffer@[+0,+0xfffff]:		往这儿读，只临时用一下（当心别人也用）
	//dirbuffer@[+0x100000,+0x1fffff]:		名，id，种类，大小
	//fsbuffer@[+0x200000,+0x2fffff]:		缓存几千几万个fat/mft/btnode/inode
	//unusedbuf@[0x300000,0x3fffff]:		未用




//__attribute__((constructor)) void initmemory()
void initmemory()
{
	logbuf=(unsigned char*)malloc(0x100000);
	diskbuf=(unsigned char*)malloc(0x100*100);
	realworld =(unsigned char*)malloc(0x100000);		//1M
	logicworld=(unsigned char*)malloc(0x400000);		//4M
}
//__attribute__((destructor)) void freememory()
void killmemory()
{
	free(logbuf);
	free(diskbuf);
	free(realworld);
	free(logicworld);
}




void whereisdiskinfo(unsigned long long* p)
{
	*p=(unsigned long long)diskbuf;
}
void whereislogbuf(unsigned long long* p)
{
	*p=(unsigned long long)logbuf;
}
void whereisrealworld(unsigned long long* p)
{
	*p=(unsigned long long)realworld;
}
void whereislogicworld(unsigned long long* p)
{
	*p=(unsigned long long)logicworld;
}
*/
