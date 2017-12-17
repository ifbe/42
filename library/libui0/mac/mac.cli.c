#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include"arena.h"




static int mode = 0;
static u64 thread;




void* uievent(void* p)
{
	u8 buf[8];
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
				eventwrite(buf[0], 0x72616863, 0, 0);
			}
			
			if(buf[1] == 0x5b)
			{
				buf[2] = getchar();
				
				if( (buf[2]>=0x41) && (buf[2]<=0x44) )
				{
					eventwrite((buf[2]<<16)+0x5b1b,	0x72616863, 0, 0);
				}
			}//5b
		}//1b
		
		else
		{
			eventwrite(buf[0], 0x72616863, 0, 0);
		}
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
void windowread()
{
}
void windowwrite()
{
}
void windowstart(struct window* win)
{
	win->fmt = 0x696c63;

	win->w = 80;
	win->h = 25;
	win->d = 0;
	win->t = 0;
}
void windowstop()
{
}
void windowcreate()
{
	windowchange(1);
	thread = startthread(uievent, 0);
}
void windowdelete()
{
	windowchange(0);
}
