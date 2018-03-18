#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include "arena.h"
int lowlevel_input();




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




DWORD WINAPI terminalthread(void* win)
{
	u64 why, what, where;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		where = (u64)win;
		eventwrite(why, what, where, 0);
	}
}




void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstop()
{
}
void windowstart(struct window* this)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex32('c','l','i',0);
	this->buf = 0;
	this->len = 0;

	this->width = this->stride = 80;
	this->height = 25;

	if(termcount == 0)
	{
		u64 thread = startthread(terminalthread, this);
		termcount++;
	}
	else
	{
		CreateProcess(
			"c:\\windows\\system32\\cmd.exe",
			0, 0, 0, 0,
			CREATE_NEW_CONSOLE, 0, 0,
			&si, &pi
		);
		printf("GetLastError=%d\n",GetLastError());
		termcount++;
	}
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
{
}
void freewindow()
{
}
