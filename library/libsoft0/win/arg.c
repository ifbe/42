#include<stdio.h>
#include<string.h>
#include <windows.h>
#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void target(char*);
void printmemory(char*,int);
void diary(char* fmt,...);




static char mystring[100];
char* explainarg()
{
	//拿到进程的输入arg,决定默认打开谁
	int i,j,wantlog;
	char* result=0;
	char* inputarg=GetCommandLine();
	strncpy(mystring,inputarg,100);




	//if(mystring[0]=='\"')
	//{
		//如果是双击打开的就干掉终端
		//HWND consolewindow=GetConsoleWindow();
		//ShowWindow(consolewindow,SW_HIDE);
		//FreeConsole();
	//}
	wantlog=0;
	for(i=0;i<100;i++)
	{
		if(mystring[i]==0)break;
		if(mystring[i]==0x20)
		{
			//只要发现空格后面跟着的是-
			if(mystring[i+1]=='-')wantlog=1;
		}
	}
	if(wantlog==1)
	{
		//终端回来
		AllocConsole();
		//HWND consolewindow=GetConsoleWindow();
		//ShowWindow(consolewindow,SW_SHOW);
		freopen("CONIN$","r",stdin);
		freopen("CONOUT$","w",stdout);

		//必须有console才能显示
		printmemory(mystring,100);
	}




	//可能有多个空格
	for(i=0;i<100;i++)
	{
		if(mystring[i]==0)
		{
			result=0;
			break;
		}
		if(mystring[i]==0x20)
		{
			//只要发现空格后面跟着的不是\0,不是0x20,不是-
			if( 
				( mystring[i+1] != 0 ) &&
				( mystring[i+1] != 0x20 ) &&
				( mystring[i+1] != '-' )
			)
			{
				for(j=0;j<20;j++)
				{
					if(mystring[i+1+j]==0x20)mystring[i+1+j]=0;
				}
				result=mystring+i+1;
				break;
			}
		}
	}

	//-----
	target(result);
}