#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include"libuser.h"




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
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('c','l','i',0);;

	w->len = 0;
	w->buf = 0;

	w->width = w->stride = 80;
	w->height = 25;

	thread = startthread(uievent, w);
}




void initwindow()
{
	windowchange(1);
}
void freewindow()
{
	windowchange(0);
}