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
void* supply_alloc();
void* supply_recycle(void*);
void tuinode_take( void*,int, void*,int, void*,int, void*,int);
void tuinode_give(void*,int, void*,int, void*,int, void*,int);




//输入
static int flag=-1;
static struct termios old;
static struct termios new;



/*
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
}*/




static void attr(u8 bg, u8 fg)
{
	//01  23  456  789
	char str[16] = {0x1b,0x5b,  '0',';',  '4','0',';',  '3','7','m',  0};
	if(fg >= 8)str[2] = '1';
	str[5] = '0' + (bg&7);
	str[8] = '0' + (fg&7);
	printf("%.16s",str);
}
void windowdraw(_obj* wnd)
{
	int x,y,w,h;
	u8 ch,bg=0,fg=0;
	u8* p;
	u8* buf;

	w = wnd->whdf.width;
	h = wnd->whdf.height;
	buf = wnd->tuitext.buf;
	p   = wnd->tuitext.conv;
/*
	write(1, "\033[H\033[J", 6);
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			ch = buf[(w*y + x)<<2];
			if( (ch<0x20) | (ch>0x7e) )ch = 0x20;
			p[w*y+x] = ch;
		}
	}
	write(1, p, w*y);
	write(1, "\033[0m", 4);
	return;
*/
	printf("\033[H\033[J");
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			p = buf + ((w*y + x)<<2);
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
				printf("%.6s",p);
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




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	wnd->whdf.width = ws.ws_col;
	wnd->whdf.height = ws.ws_row;

	//read context
	tuinode_take(wnd,0, stack,sp, arg,key, buf,len);

	//update screen
	windowdraw(wnd);

	usleep(20*1000);
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
void window_attach()
{
}
void window_detach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* w)
{
}
void window_create(_obj* w)
{
	w->hfmt = _tui_;
	w->vfmt = 0;

	w->whdf.width = 80;
	w->whdf.height= 25;

	//w->fbwidth = 80*8;
	//w->fbheight= 25*16;

	w->tuitext.buf = malloc(0x100000);
	w->tuitext.conv= malloc(0x100000);
	//threadcreate(textuithread, w);
}




void* window_alloc()
{
	return supply_alloc();
}



/*
static void newsize(int num)
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
}*/
void initwindow()
{
	//signal(SIGWINCH,newsize);

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
