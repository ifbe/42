#include<stdio.h>
#include<stdarg.h>
#include<windows.h>




void say(char* fmt , ...)
{
	printf(fmt);
}




void initserial(char* arg)
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
void killserial()
{
}
