#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include "arena.h"
//
u64 startthread(void*, void*);
void stopthread();
//
u64* eventread();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




//
static int mode = 0;
static u64 thread;




void* uievent(void* win)
{
	u8 buf[8];
	u64 why,what,where;

	//
	where = (u64)win;
	while(1)
	{
		buf[0] = getchar();
		if( (buf[0] == 0) | (buf[0]==0xff) )
		{
			usleep(10000);
			continue;
		}

		if(buf[0] == 0x1b)
		{
			buf[1] = getchar();
			if(buf[1] == 0xff)
			{
				why = buf[0];
			}
			else if(buf[1] == 0x5b)
			{
				buf[2] = getchar();

				if( (buf[2]>=0x41) && (buf[2]<=0x44) )
				{
					why = (buf[2]<<16)+0x5b1b;
				}
			}//5b
			else continue;

			what = hex32('k','b','d',0);
		}//1b

		else
		{
			what = hex32('c','h','a','r');
			why = buf[0];
		}

		//send
		eventwrite(why, what, where, 0);
	}
}




void windowchange(int what)
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	mode = what;

	if(mode == 0)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
		t.c_lflag |= ICANON|ECHO;
	}
	else if(mode == 1)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
		t.c_lflag &= ~(ICANON|ECHO);
		t.c_cc[VTIME] = 0;
		t.c_cc[VMIN] = 1;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void windowlist()
{
}
void windowread(char* where)
{
}
void windowwrite()
{
}




void windowstart(struct window* this)
{
	if(this->type == hex32('b','u','f',0))
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
	windowchange(1);
}
void windowdelete()
{
	windowchange(0);
}
