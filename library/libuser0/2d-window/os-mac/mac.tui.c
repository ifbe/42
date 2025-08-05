#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "libuser.h"
void stdout_sethelpserial(void*);
void* supply_alloc();
void* supply_recycle(void*);
int tuinode_take(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len);
int tuinode_give(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len);




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




static void attr(_obj* wnd, u8 bg, u8 fg)
{
	if(_tui256_ == wnd->vfmt){
		char str[32] = {};
		int ret = snprintf(str, 32, "\033[48;5;%dm\033[38;5;%dm", bg, fg);
		write(1, str, ret);
	}
	else{
		//01  23  456  789
		char str[16] = {0x1b,0x5b,  '0',';',  '4','0',';',  '3','7','m',  0};
		if(fg >= 8)str[2] = '1';
		str[5] = '0' + (bg&7);
		str[8] = '0' + (fg&7);
		write(1, str, 10);
		//printf("%s",str);
	}
}
void windowdraw(_obj* wnd)
{
	int x,y;
	u8 ch,bg=0,fg=0;
	u8* p;
	u8* buf;
	write(1, "\033[H\033[J", 6);

	buf = (u8*)(wnd->tuitext.buf);
	for(y=0;y<wnd->whdf.height;y++)
	{
		for(x=0;x<wnd->whdf.width;x++)
		{
			p = buf + ((wnd->whdf.fbwidth*y + x)<<2);
			//printf("buf=%p,p=%p\n",buf,p);
			if(p[0] > 0x80)
			{
				//先颜色
				if((bg != p[7]) | (fg != p[6]))
				{
					bg = p[7];
					fg = p[6];
					attr(wnd, bg, fg);
				}

				//这是汉字
				//printf("%s",p);
				while(p[0]>0x80){
					write(1, p, 1);
					p++;
				}
				x++;
			}
			else
			{
				//先颜色
				if((bg != p[3]) | (fg != p[2]))
				{
					bg = p[3];
					fg = p[2];
					attr(wnd, bg, fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				if(ch > 0x7e)ch = 0x7e;
				write(1, &ch, 1);
				//printf("%c",ch);
			}
		}
	}
	write(1, "\033[0m", 4);
	//fflush(stdout);
}




void window_attach()
{
}
void window_detach()
{
}
void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	wnd->whdf.width = ws.ws_col;
	wnd->whdf.height= ws.ws_row;
	wnd->whdf.fbwidth = ws.ws_col;

	//read context
	tuinode_take(wnd,0, stack,sp, arg,key, buf,len);

	stdout_sethelpserial(0);

	//update screen
	windowdraw(wnd);

	//stdout_sethelpserial((void*)1);

	usleep(50*1000);
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}




void window_delete(_obj* w)
{
}
void window_create(_obj* w)
{
	w->type = _wnd_;
	w->vfmt = _tui256_;

	w->whdf.width = 80;
	w->whdf.height = 25;

	w->whdf.fbwidth = 80;
	//w->fbheight = 0;

	w->tuitext.buf = malloc(0x100000);
	//threadcreate(uievent, w);

	int j;
	printf("will print 64 empty lines\n");
	for(j=0;j<64;j++)printf("%d\n", j);
}
void window_reader()
{
}
void window_writer()
{
}




void* window_alloc()
{
	return supply_alloc();
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
