#include<stdio.h>
#include<stdarg.h>
#include<windows.h>




void rx(char* p)
{
}
void tx(char* fmt , ...)
{
	printf(fmt);
}




//debug用，打印从addr开始的总共size个字节
void printmemory(BYTE* addr,int size)
{
	int i,j;

	//打印的很少就不显示这些了
	if(size>0x10)
	{
		printf("[----addr----]");
		for(i=0;i<=0xf;i++)
		{
			printf("%2x ",i);
		}
		printf("[----anscii----]\n");
	}

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)printf("@%-13llx",addr+j*16);
		else printf("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			printf("%.2x ",addr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			BYTE ch=addr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			printf("%c",ch);
		}
		printf("\n",addr[16*j+15]);
	}
}




void basicinit(char* arg)
{
	//
	HWND consolewindow;

	//opened?
	consolewindow=GetConsoleWindow();
	if(consolewindow!=NULL)return;

	//new
	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);

	//hide
	consolewindow=GetConsoleWindow();
	ShowWindow(consolewindow,SW_HIDE);
}
void basickill()
{
}
