#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "arena.h"
//
u64 startthread(void*, void*);
void stopthread();
//
u64* eventread();
void eventwrite(u64,u64,u64,u64);




static u8 xlib2upper[0x80]={
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//0,7
0x00,0x00, '!', '@',	 '#', '$', '%', '^',	//8,f
 '&', '*', '(', ')',	 '_', '+',0x08,0x09,	//10,17
 'Q', 'W', 'E', 'R',	 'T', 'Y', 'U', 'I',	//18,1f
 'O', 'P', '{', '}',	0x0d,0x00, 'A', 'S',	//20,27
 'D', 'F', 'G', 'H',	 'J', 'K', 'L', ':',	//28,2f
'\"', '~',0x00, '|',	 'Z', 'X', 'C', 'V',	//30,37
 'B', 'N', 'M', '<',	 '>', '?',0x00, '*',	//38,3f
0x00, ' ',0x00,0x00,	0x00,0x00,0x00,0x00,	//40,47
0x00,0x00,0x00,0x00,	0x00,0x00,0x00, '7',	//48,4f
 '8', '9', '-', '4',	 '5', '6', '+', '1',	//50,57
 '2', '3', '0', '.',	0x00,0x00,0x00,0x00,	//58,5f
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//60,67
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//68,6f
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//70,77
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//78,7f
};
static u8 xlib2anscii[0x80]={
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//0,7
0x00,0x00, '1', '2',	 '3', '4', '5', '6',	//8,f
 '7', '8', '9', '0',	 '-', '=',0x08,0x09,	//10,17
 'q', 'w', 'e', 'r',	 't', 'y', 'u', 'i',	//18,1f
 'o', 'p', '[', ']',	0x0d,0x00, 'a', 's',	//20,27
 'd', 'f', 'g', 'h',	 'j', 'k', 'l', ';',	//28,2f
'\'', '`',0x00,'\\',	 'z', 'x', 'c', 'v',	//30,37
 'b', 'n', 'm', ',',	 '.', '/',0x00, '*',	//38,3f
0x00, ' ',0x00,0x00,	0x00,0x00,0x00,0x00,	//40,47
0x00,0x00,0x00,0x00,	0x00,0x00,0x00, '7',	//48,4f
 '8', '9', '-', '4',	 '5', '6', '+', '1',	//50,57
 '2', '3', '0', '.',	0x00,0x00,0x00,0x00,	//58,5f
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//60,67
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//68,6f
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//70,77
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//78,7f
};
static u8 xlib2kbd[0x80]={
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//0,7
0x00,0x1b, '1', '2',	 '3', '4', '5', '6',	//8,f
 '7', '8', '9', '0',	 '-', '=',0x08,0x09,	//10,17
 'q', 'w', 'e', 'r',	 't', 'y', 'u', 'i',	//18,1f
 'o', 'p', '[', ']',	0x0d,0xff, 'a', 's',	//20,27
 'd', 'f', 'g', 'h',	 'j', 'k', 'l', ';',	//28,2f
'\'', '`',0xff,'\\',	 'z', 'x', 'c', 'v',	//30,37
 'b', 'n', 'm', ',',	 '.', '/', 0xff,'*',	//38,3f
0xff, ' ',0xff,0xf1,	0xf2,0xf3,0xf4,0xf5,	//40,47
0xf6,0xf7,0xf8,0xf9,	0xfa,0xff,0xff, '7',	//48,4f
 '8', '9', '-', '4',	 '5', '6', '+', '1',	//50,57
 '2', '3', '0', '.',	0xff,0xff,0xff,0xfb,	//58,5f
0xfc,0xff,0xff,0xff,	0x00,0x00,0x00,0x00,	//60,67
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x48,	//68,6f
0x00,0x4b,0x4d,0x00,	0x50,0x00,0x00,0x00,	//70,77
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//78,7f
};
//
static Display* dsp = 0;
static Visual* visual = 0;
static Window root = 0;
static Atom wmDelete;
static Atom wmCreate;
//
static struct window* palette;
static struct window* theone;
static void* maptab[0x1000];




static int fuckyou=0;
void createmywindow(struct window* this)
{
	//window, gc
	this->fd = XCreateSimpleWindow(
		dsp, RootWindow(dsp,0), 0, 0,
		this->w, this->h,
		1, 0, 0);
	this->gc = (u64)XCreateGC(dsp, this->fd, 0, NULL);

	//intercept window delete event 
	XSetWMProtocols(dsp, this->fd, &wmDelete, 1);

	//
	XSelectInput
	(
		dsp, this->fd,
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);

	//
	XMapWindow(dsp, this->fd);
	maptab[(this->fd) & 0xfff] = this;
	fuckyou++;
}
void* uievent(struct window* this)
{
	u64 x,y,k;
	u64 why, what, where;
	Window win;
	XEvent ev;
	XAnyEvent* pev;
	pev = (void*)&ev;

	createmywindow(this);
	while(1)
	{
		XNextEvent(dsp, &ev);
		win = pev->window;
		this = maptab[win&0xfff];
		where = (u64)this;

		if(ev.type == ClientMessage)
		{
			//printf("ClientMessage\n");
			if(ev.xclient.data.l[0] == wmCreate)
			{
				createmywindow((void*)theone + ev.xclient.data.l[1]);
			}
			else if(ev.xclient.data.l[0] == wmDelete)
			{
				maptab[((u64)win) & 0xfff] = 0;
				XDestroyWindow(dsp, win);
				fuckyou--;

				if(fuckyou <= 0)
				{
					eventwrite(0,0,0,0);
					break;
				}
			}
		}//ClientMessage
		else if(ev.type == ConfigureNotify)
		{
			int x = ev.xconfigure.width;
			int y = ev.xconfigure.height;
			//printf("%d,%d\n",x,y);

			if( (x == this->w) && (y == this->h) )continue;
			this->w = x;
			this->h = y;

			palette->ximage = (u64)XCreateImage(
				dsp, visual, 24, ZPixmap,
				0, (void*)(palette->buf),
				x,y,32,0
			);

			why = x + (y<<16);
			what = hex32('s','i','z','e');
			eventwrite(why, what, where, 0);
		}//ConfigureNotify
		else if(ev.type == Expose)
		{
			XImage* xi;
			if(palette == 0)continue;

			xi = (void*)(palette->ximage);
			if(xi == 0)continue;

			XPutImage(
				dsp, this->fd, (void*)(this->gc), xi,
				0, 0, 0, 0,
				this->w, this->h
			); 
		}//Expose
		else if(ev.type == ButtonPress)
		{
			//printf("buttonpress\n");
			if(ev.xbutton.button == Button4)	//front
			{
				x = (ev.xbutton.x<<16) / (this->w);
				y = (ev.xbutton.y<<16) / (this->h);
				k = 'f';

				why = x + (y<<16) + (k<<48);
				what = hex32('p', '+', 0, 0);
				eventwrite(why, what, where, 0);
			}
			else if(ev.xbutton.button == Button5)	//back
			{
				x = (ev.xbutton.x<<16) / (this->w);
				y = (ev.xbutton.y<<16) / (this->h);
				k = 'b';

				why = x + (y<<16) + (k<<48);
				what = hex32('p', '+', 0, 0);
				eventwrite(why, what, where, 0);
			}

			continue;
		}//ButtonPress
		else if(ev.type == ButtonRelease)
		{
			if(ev.xbutton.button == Button1)
			{
			}
		}//ButtonRelease
		else if(ev.type == MotionNotify)
		{
			x = (ev.xbutton.x<<16) / (this->w);
			y = (ev.xbutton.y<<16) / (this->h);
			k = 'l';

			why = x + (y<<16) + (k<<48);
			what = hex32('p', '@', 0, 0);
			eventwrite(why, what, where, 0);
		}//MotionNotify
		else if(ev.type == KeyPress)
		{
			//KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
			//if (ev.xkey.keycode == keyQ)break;
			//printf("%x\n",ev.xkey.keycode);

			//普通anscii码
			what = hex32('c','h','a','r');
			if((ev.xkey.state&1) == 1)
			{
				why = xlib2upper[ev.xkey.keycode];
				if(why != 0)
				{
					eventwrite(why, what, where, 0);
					continue;
				}
			}
			else
			{
				why = xlib2anscii[ev.xkey.keycode];
				if(why != 0)
				{
					eventwrite(why, what, where, 0);
					continue;
				}
			}

			//控制按键
			why = xlib2kbd[ev.xkey.keycode];
			what = hex32('k','b','d',0);
			eventwrite(why, what, where, 0);
		}//KeyPress
	}//while

	XCloseDisplay(dsp);
}




void windowwrite(struct window* dst, struct window* src)
{
	XEvent ev;
	u64 fd = dst->fd;
	if(maptab[fd & 0xfff] == 0)return;

	memset(&ev,0,sizeof(XEvent));
	ev.type = Expose;
	ev.xexpose.display = dsp;
	ev.xexpose.window = fd;
	XSendEvent(dsp, fd, False, ExposureMask, &ev);
	XFlush(dsp);	//must

	usleep(10000);
}
void windowread()
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
	int j;
	this->w = 512;
	this->h = 512;
	if(this->type == hex32('b','u','f',0))
	{
		palette = this;
		palette->ximage = (u64)XCreateImage(
			dsp, visual, 24, ZPixmap, 0,
			(void*)(palette->buf), 512, 512,
			32, 0
		);
		return;
	}
	else
	{
		this->type = hex32('w', 'i', 'n', 0);
		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');

                for(j=0;j<16;j++)
                {
                        (this->touch[j]).id = 0xffff;
                }
		if(fuckyou == 0)
		{
			theone = this;
			this->thread = startthread(uievent, this);
		}
		else
		{
			XClientMessageEvent ev;
			memset(&ev,0,sizeof(XClientMessageEvent));
			ev.type = ClientMessage;
			ev.window = root;
			ev.format = 32;
			ev.data.l[0] = wmCreate;
			ev.data.l[1] = (void*)this - (void*)theone;
			XSendEvent(dsp, theone->fd, False, 0, (void*)&ev);
			XFlush(dsp);	//must
			printf("%llx\n", (u64)this);
		}
	}
}
void windowstop()
{
}
void windowcreate()
{
	int j;
	for(j=0;j<0x1000;j++)maptab[j] = 0;

	//must
	XInitThreads();

	//display
	dsp = XOpenDisplay(NULL);
	root = DefaultRootWindow(dsp);
	//int screen = DefaultScreen(dsp);

	//visual
	visual = DefaultVisual(dsp, 0);
	if(visual->class!=TrueColor)
	{
		fprintf(stderr, "Cannot handle non true color visual ...\n");
		XCloseDisplay(dsp);
		exit(1);
	}

	//
	wmDelete = XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	wmCreate = XInternAtom(dsp, "WM_CREATE_WINDOW", True);
}
void windowdelete()
{
}
