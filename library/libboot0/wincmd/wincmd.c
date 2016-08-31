#include<stdio.h>
#include<stdarg.h>
#include<windows.h>




int diary(char* mem, int max, char* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say(char* fmt , ...)
{
	printf(fmt);
}




void createserial(char* arg)
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
void deleteserial()
{
}
