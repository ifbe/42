#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char


QWORD diskinfo;
int type=1;




void whereisdiskinfo(QWORD* in);
void enumeratedisk();
void enumerateprocess();
void choosedisk(char* in);
void readdisk(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void myreadprocessmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);

void say(char* fmt,...);
void initmemory();
void initlog();
void initwindow();
void initdisk();
void initprocess();
void killmemory();
void killwindow();





void listall()
{
	//清理
	char* p=(char*)diskinfo;
	int i;
	for(i=0;i<100*0x100;i++)p[i]=0;

	//扫描
	enumeratedisk();
	enumerateprocess();
}
void choosetarget(QWORD in)
{
	if( in >100 )		//是一个内存地址
	{
		//第1种可能：文件的路径（比如d:\image\name.img）
		say("file:%s\n",(char*)in);
		type=1;
		choosedisk((char*)in);
	}
	else		//是一个数字
	{
		char* path=(char*)(diskinfo+0x100*in);

		//第2种可能：是个硬盘(比如："\\.\PHYSICALDRIVE0")
		if( *(DWORD*)path == 0x5c2e5c5c )
		{
			say("disk:%s\n",path);
			type=2;
			choosedisk(path);
		}

		//第3种可能：是某个进程的虚拟内存（比如："a.exe"）
		else
		{
			type=3;
			say("process:\n");
			//chooseprocess(number);
		}
	}
}
//内存地址，第一扇区，请无视，总字节数
void readmemory(QWORD buf,QWORD startsector,QWORD disk,DWORD count)
{
	if(type==1 | type==2)
	{
		readdisk(buf,startsector,buf,count);
	}
	else if(type==3)
	{
		myreadprocessmemory(buf, startsector*0x200, buf, count*0x200 );
	}
}
















//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
//choosetarget((QWORD)lpCmdLine);
__attribute__((constructor)) void initroot()
{
	initmemory();	//初始化各个部分，放第一个，必须!必须!必须!重要说3遍!
	initlog();	//日志，放第二个，必须!必须!必须!重要说3遍!

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
__attribute__((destructor)) void killroot()
{
	//清理
	//killdisk();
	killwindow();
	killmemory();
}
