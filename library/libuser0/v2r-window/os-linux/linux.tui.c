#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<signal.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#include"libuser.h"
int lowlevel_input();
void tuinode_read(void*, void*);
void tuinode_write(void*, void*);




//窗口
static struct winsize w;
static int width,height;
static int lastwidth=0,lastheight=0;
//输入
static int flag=-1;
static struct termios old;
static struct termios new;




static void newsize(int num)
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
}
void* textuithread(void* win)
{
	u64 why, what, where;
	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			eventwrite(width + (height<<16), 0x657a6973, 0, 0);
		}

		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		where = (u64)win;
		eventwrite(why, what, where, 0);

	}//while
}




static void attr(u8 bg, u8 fg)
{
	char str[6] = {0x1b, 0x5b, '4', 0, 'm', 0};
	if( (bg > 0) && (bg <8) )
	{
		str[3] = 0x30 + bg;
		printf("%s", str);
	}
	else printf("\033[0m");
}
void windowdraw(struct arena* src)
{
	int x,y;
	u8 ch,bg=0,fg=0;
	u8* p;
	u8* buf;
	printf("\033[H\033[J");
/*
	printf("%s",src->buf);
	fflush(stdout);
*/
	buf = (u8*)(src->buf);
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = buf + ((width*y + x)<<2);
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
	if(bg != 0)attr(0,0);
}




void windowread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct arena* win = self->pchip;

	//read context
	tuinode_read(win, 0);

	//update screen
	windowdraw(win);
}
void windowwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
void windowstop()
{
}
void windowstart()
{
}




void windowlist()
{
}
void windowchange()
{
}
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('t','u','i',0);

	w->width = w->stride = width;
	w->height = height;

	w->buf = malloc(0x100000);
	threadcreate(textuithread, w);
}




void initwindow()
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
	signal(SIGWINCH,newsize);

	//
	flag=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}
void freewindow()
{
	if(flag!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
