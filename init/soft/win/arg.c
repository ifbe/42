#include<stdio.h>
#include<string.h>
#include <windows.h>
#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void printmemory(char*,int);
void diary(char* fmt,...);




char* explainarg()
{
	//拿到进程的输入arg,决定默认打开谁
	char* inputarg=GetCommandLine();

	//第一个是双引号说明是双击打开的,没找不到其他办法......
	if(inputarg[0]=='\"')
	{
		//如果是双击打开的就干掉终端
		FreeConsole();
		//HWND consolewindow=GetConsoleWindow();
		//ShowWindow(consolewindow,SW_HIDE);
	}
	//否则就是在cmd里面打开的
	else
	{
		//终端回来
		AllocConsole();
		freopen("CONIN$","r",stdin);
		freopen("CONOUT$","w",stdout);
		//HWND consolewindow=GetConsoleWindow();
		//ShowWindow(consolewindow,SW_SHOW);
	}
	printmemory(inputarg,100);

	//
	int i;
	for(i=0;i<100;i++)
	{
		if(inputarg[i]==0)return 0;
		if(inputarg[i]==0x20)break;
	}

	if(inputarg[i+2]<=0x20)return 0;
	else return inputarg+i+2;	//两个空格
}