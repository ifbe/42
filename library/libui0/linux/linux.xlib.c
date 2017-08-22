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
'\'', '~',0x00,'\\',	 'Z', 'X', 'C', 'V',	//30,37
 'B', 'N', 'M', ',',	 '.', '/',0x00, '*',	//38,3f
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
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x26,	//68,6f
0x00,0x25,0x27,0x00,	0x28,0x00,0x00,0x00,	//70,77
0x00,0x00,0x00,0x00,	0x00,0x00,0x00,0x00,	//78,7f
};
//
static Display* dsp=0;
static Visual* visual=0;
static Atom wmDelete;
//
static struct window* palette;




void* uievent(struct window* this)
{
	XEvent ev;
	u64 x,y,k;
	u64 why, what, where;

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

	where = (u64)this;
	while(1)
	{
	XNextEvent(dsp, &ev);
	if(ev.type==ButtonPress)
	{
		//printf("buttonpress\n");
		if(ev.xbutton.button==Button4)	//front
		{
			x = (ev.xbutton.x<<16) / (this->w);
			y = (ev.xbutton.y<<16) / (this->h);
			k = 'f';

			why = x + (y<<16) + (k<<48);
			what = hex32('p', '+', 0, 0);
			eventwrite(why, what, where, 0);
		}
		else if(ev.xbutton.button==Button5)	//back
		{
			x = (ev.xbutton.x<<16) / (this->w);
			y = (ev.xbutton.y<<16) / (this->h);
			k = 'b';

			why = x + (y<<16) + (k<<48);
			what = hex32('p', '+', 0, 0);
			eventwrite(why, what, where, 0);
		}
/*
		else if(ev.xbutton.button==Button1)
		{
			this->oldx=ev.xbutton.x;
			this->oldy=ev.xbutton.y;
		}
*/
		continue;
	}
	else if(ev.type==ButtonRelease)
	{
		//printf("buttonrelease\n");
		if(ev.xbutton.button==Button1)
		{
/*
		if((this->oldx==ev.xbutton.x)&&
		   (this->oldy==ev.xbutton.y) )
		{
			x = (ev.xbutton.x<<16) / (this->w);
			y = (ev.xbutton.y<<16) / (this->h);
			k = 'l';

			why = x + (y<<16) + (k<<48);
			what = hex32('p', '-', 0, 0);
			eventwrite(why, what, where, 0);
		}
*/
		}
	}
	else if(ev.type==MotionNotify)
	{
/*
		this->motioncount = (this->motioncount+1)%5;
		if(this->motioncount != 0)continue;
*/
		x = (ev.xbutton.x<<16) / (this->w);
		y = (ev.xbutton.y<<16) / (this->h);
		k = 'l';

		why = x + (y<<16) + (k<<48);
		what = hex32('p', '@', 0, 0);
		eventwrite(why, what, where, 0);
/*
		this->oldx = ev.xbutton.x;
		this->oldy = ev.xbutton.y;
*/
	}
	else if(ev.type==ClientMessage)
	{
		if (ev.xclient.data.l[0] == wmDelete)
		{
			eventwrite(0,0,0,0);
			break;
		}
	}
	else if(ev.type==ConfigureNotify)
	{
		int x = ev.xconfigure.width;
		int y = ev.xconfigure.height;
		//printf("%d,%d\n",x,y);

		if( (x==this->w) && (y==this->h) )continue;
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
	}
	else if(ev.type==Expose)
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
	}
	else if(ev.type==KeyPress)
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
	}
	}//while

	//
	XDestroyWindow(dsp, this->fd);
	XCloseDisplay(dsp);
}




void windowwrite(struct window* this)
{
	XEvent ev;
	memset(&ev,0,sizeof(XEvent));
	ev.type = Expose;
	ev.xexpose.display = dsp;
	ev.xexpose.window = this->fd;
	XSendEvent(dsp, this->fd, False, ExposureMask, &ev);
	XFlush(dsp);	//must
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

		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');
		return;
	}
	else
	{
		this->type = hex32('w', 'i', 'n', 0);
                this->fmt = hex32('x', 'l', 'i', 'b');

                for(j=0;j<16;j++)
                {
                        (this->touch[j]).id = 0xffff;
                }
		this->thread = startthread(uievent, this);
	}
}
void windowstop()
{
}
void windowcreate()
{
	//must
	XInitThreads();

	//display
	dsp = XOpenDisplay(NULL);
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
	wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
}
void windowdelete()
{
}
