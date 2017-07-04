#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include "arena.h"




//
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




DWORD WINAPI uievent(LPVOID pM)
{
	u8 ch;

	//
	while(1)
	{
		ch = getch();
		if(ch == 0xe0)
		{
			ch = getch();
			if(ch == 0x48)	//up
			{
				eventwrite(0x415b1b, 0x72616863, 0, 0);
			}
			else if(ch == 0x50)	//down
			{
				eventwrite(0x425b1b, 0x72616863, 0, 0);
			}
			else if(ch == 0x4d)	//right
			{
				eventwrite(0x435b1b, 0x72616863, 0, 0);
			}
			else if(ch == 0x4b)	//left
			{
				eventwrite(0x445b1b, 0x72616863, 0, 0);
			}
		}
		else
		{
			if(ch == 0x8)ch = 0x7f;
			eventwrite(ch, 0x72616863, 0, 0);
		}
	}//while
}




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart(struct window* win)
{
	win->type = 0;
	win->fmt = 0x696c63;
	win->buf = 0;
	win->len = 0;

	win->w = 80;
	win->h = 25;
	win->d = 0;
	win->dim = 1;

	win->thread = startthread(uievent, win);
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
