#include<stdlib.h>
#define QWORD unsigned long long
#define DWORD unsigned int




void disklist();
void diskchoose(QWORD);
void diskread(QWORD,QWORD,QWORD,QWORD);

void explainarg();

void initdisk(QWORD);
void killdisk();

void initprocess(QWORD);
void killprocess();

void say(char* , ...);




static unsigned char* world;
	//[+0x000000,+0x0fffff]:		某个硬盘(或文件)的分区情况
	//[+0x100000,+0x1fffff]:		某个分区的结构
	//[+0x200000,+0x2fffff]:		当前目录里面的文件
	//[+0x300000,+0x3fffff]:		buffer
	//[+0x400000,+0x4fffff]:		终端
	//[+0x500000,+0x5fffff]:		日志
	
	//[+0x700000,+0x7fffff]:		当前系统内有多少个能读写的东西




__attribute__((constructor)) void initeverything()
{
	world = (unsigned char*)malloc(0x800000);		//8M
	say("beforemain(){\n");
	say("inited memory\n");

	//只是拿地址
	initdisk( (QWORD)world+0x700000 );
	initprocess( (QWORD)world+0x700000 );

	//不管是不是只有arg0
	disklist();
	explainarg();

	say("}\n");
}
__attribute__((destructor)) void killeverything()
{
	say("aftermain(){\n");

	killdisk();
	killprocess();

	free(world);
	say("killed memory\n");

	say("}\n");
}




QWORD whereisworld()
{
	return (unsigned long long)world;
}
void choosetarget(QWORD in)
{
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		say("file:%s\n",(char*)in);
		diskchoose(in);
	}
	else		//是一个数字
	{
		QWORD path=(QWORD)world+0x700000+0x100*in;

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		if( *(DWORD*)path == 0x5c2e5c5c )
		{
			say("disk:%s\n",path);
			diskchoose(path);
		}
	}
}
//内存地址，第一扇区，请无视，总字节数
void readmemory(QWORD buf,QWORD startsector,QWORD ignore,DWORD count)
{
	diskread(buf,startsector,0,count);
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