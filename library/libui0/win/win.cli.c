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




DWORD WINAPI uievent(void* win)
{
	u64 why,what,where;

	//
	where = (u64)win;
	while(1)
	{
		why = getch();
		if(why == 0xe0)
		{
			why = getch();
			if(why == 0x48)	//up
			{
				why = 0x415b1b;
			}
			else if(why == 0x50)	//down
			{
				why = 0x425b1b;
			}
			else if(why == 0x4d)	//right
			{
				why = 0x435b1b;
			}
			else if(why == 0x4b)	//left
			{
				why = 0x445b1b;
			}
			else continue;

			what = hex32('k', 'b', 'd', 0);
		}
		else
		{
			if(why == 0x8)why = 0x7f;
			what = hex32('c', 'h', 'a', 'r');
		}

		//send
		eventwrite(why, what, where, 0);
	}//while
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

		this->thread = startthread(uievent, this);
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
