#include<stdlib.h>




static unsigned char* world;
static unsigned char* screen;




void initeverything()
{
	world = (unsigned char*)malloc(0x800000);		//8M
	screen = (unsigned char*)malloc(0x1000000);		//16M

	initlog();

	say("beforemain(){\n");
	say("inited memory\n");
	say("inited log\n");

	//必须在log之后不管几个
	initwindow();

	//只是拿地址
	initdisk();

	//只是拿地址
	initprocess();

	//列出所有能发现的
	whereisdiskinfo(&diskinfo);		//必须!
	listall();

	//拿到进程的输入arg,决定默认打开谁
	char* inputarg=GetCommandLine();
	say("%s\n",inputarg);

	//
	int i;
	int signal=0;
	while(1)
	{
		if(inputarg[i]==0)break;
		else if(inputarg[i]==0x22)
		{
			//记录引号的个数，引号出现在arg0
			signal++;
		}
		else if(inputarg[i]==0x20)
		{
			//碰到两次引号之后出现空格，要注意了不出意外就是后面一个！
			//要是多次空格保持不变0xff就行
			if(signal==2)signal=0xff;
		}
		else
		{
			//不是引号不是空格的正常字符 && 此时有信号
			//就能开干了
			if(signal==0xff)break;
		}

		i++;
	}
	if(inputarg[i]==0)
	{
		//"d:\code\file\a.exe"
		//比如上面这种，就默认打开扫描到的第一个磁盘
		choosetarget(0);
	}
	else
	{
		//"d:\code\file\a.exe" d:\code\1.txt
		//比如上面这种，就默认打开d:\code\1.txt
		choosetarget( (QWORD)(inputarg+i) );
	}

	say("}\n");
}
void killeverything()
{
	free(world);
	free(screen);
}




void whereisworld(unsigned long long* p)
{
	*p=(unsigned long long)world;
}
void whereisscreen(unsigned long long* p)
{
	*p=(unsigned long long)screen;
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