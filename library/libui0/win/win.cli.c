#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include "arena.h"
u64 startthread(void*, void*);
void stopthread();
//
int lowlevel_input();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




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
void windowstart(struct window* this)
{
	if(this->type == hex32('b', 'u', 'f', 0))
	{
		this->fmt = hex32('c','l','i',0);
		return;
	}
	else
	{
		this->type = hex32('w','i','n',0);
		this->fmt = hex32('c','l','i',0);
		this->buf = 0;
		this->len = 0;

		this->w = 80;
		this->h = 25;
		this->d = 0;

		if(termcount == 0)
		{
			this->thread = startthread(terminalthread, this);
			termcount++;
		}
		else
		{
			CreateProcess("c:\\windows\\system32\\cmd.exe", 0, 0, 0, 0, 
				CREATE_NEW_CONSOLE, 0, 0, &si, &pi);
			printf("GetLastError=%d\n",GetLastError());
			termcount++;
		}
	}
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}
