#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "libuser.h"
int tuinode_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int tuinode_give(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);




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




static void attr(u8 bg, u8 fg)
{
	//01  23  456  789
	char str[16] = {0x1b,0x5b,  '0',';',  '4','0',';',  '3','7','m',  0};
	if(fg >= 8)str[2] = '1';
	str[5] = '0' + (bg&7);
	str[8] = '0' + (fg&7);
	printf("%s",str);
}
void windowdraw(_obj* wnd)
{
	int x,y;
	u8 ch,bg=0,fg=0;
	u8* p;
	u8* buf;
	printf("\033[H\033[J");

	buf = (u8*)(wnd->textbuf);
	for(y=0;y<wnd->height;y++)
	{
		for(x=0;x<wnd->width;x++)
		{
			p = buf + ((wnd->fbwidth*y + x)<<2);
			if(p[0] > 0x80)
			{
				//先颜色
				if((bg != p[7]) | (fg != p[6]))
				{
					bg = p[7];
					fg = p[6];
					attr(bg, fg);
				}

				//这是汉字
				printf("%s",p);
				x++;
			}
			else
			{
				//先颜色
				if((bg != p[3]) | (fg != p[2]))
				{
					bg = p[3];
					fg = p[2];
					attr(bg, fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				printf("%c",ch);
			}
		}
	}
	printf("\033[0m");
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	wnd->width = ws.ws_col;
	wnd->height= ws.ws_row;
	wnd->fbwidth = ws.ws_col;

	//read context
	tuinode_take(wnd,0, stack,sp, arg,key, buf,len);

	//update screen
	windowdraw(wnd);
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void windowstop()
{
}
void windowstart()
{
}




void windowchange()
{
}
void windowlist()
{
}
void windowdelete(_obj* w)
{
}
void windowcreate(_obj* w)
{
	w->fmt = _tui_;;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	//w->fbwidth = 80;
	//w->fbheight = 0;

	w->textbuf = malloc(0x100000);
	//threadcreate(uievent, w);
}




void freewindow()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
	t.c_lflag |= ICANON|ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void initwindow()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
