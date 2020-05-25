#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libuser.h"
int tuinode_read(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int tuinode_write(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);




static int mode = 0;




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
void windowdraw(struct supply* wnd)
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




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//read context
	tuinode_read(wnd,0, stack,sp, arg,key, buf,len);

	//update screen
	windowdraw(wnd);
}
void windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void windowstop()
{
}
void windowstart()
{
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
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	w->fmt = _tui_;;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;

	w->textbuf = malloc(0x100000);
	//threadcreate(uievent, w);
}




void initwindow()
{
	windowchange(1);
}
void freewindow()
{
	windowchange(0);
}
