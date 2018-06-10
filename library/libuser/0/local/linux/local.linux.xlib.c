#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "arena.h"
int lowlevel_input();




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
static Atom wmDelete;
static Atom wmCreate;
//
//static void* winmap[0x100];
//static Window fdmap[0x100];
static int fuckyou = 0;




void windowopen(struct arena* win)
{
	//window, gc
	win->fd = XCreateSimpleWindow(
		dsp, RootWindow(dsp,0), 0, 0,
		win->width, win->height,
		1, 0, 0);
	win->gc = (u64)XCreateGC(dsp, win->fd, 0, NULL);

	//intercept window delete event 
	XSetWMProtocols(dsp, win->fd, &wmDelete, 1);

	//
	XSelectInput
	(
		dsp, win->fd,
		PointerMotionMask|
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);

	//
	//fdmap[j] = win->fd;
	XMapWindow(dsp, win->fd);
	fuckyou++;
}
void windowclose(struct arena* win)
{
	XDestroyWindow(dsp, win->fd);
	fuckyou--;

	if(0 == fuckyou)
	{
		XCloseDisplay(dsp);
		eventwrite(0,0,0,0);
	}
}
void windowevent(struct arena* win, XEvent xev)
{
	u64 x,y,k;
	struct event myev;
/*
	Window fd;
	XAnyEvent* pev;
	pev = (void*)&xev;
	fd = pev->window;
	if(xev.type == ClientMessage)printf("abcdef\n");

	win = 0;
	for(k=0;k<0x100;k++)
	{
		if(fdmap[k] == fd)
		{
			win = winmap[k];
			break;
		}
	}

	if(xev.type == ClientMessage)
	{
		if(xev.xclient.data.l[0] == wmCreate)
		{
			printf("wmCreate\n");
			k = xev.xclient.data.l[1];
			createmywindow(k);
			continue;
		}
		else if(xev.xclient.data.l[0] == wmDelete)
		{
			printf("wmDelete:%x,%x\n",k,fd);
			XDestroyWindow(dsp, fd);
			if((k>0)&&(k<256))
			{
				winmap[k] = 0;
				fdmap[k] = 0;
			}

			fuckyou--;
			if(fuckyou <= 1)
			{
				eventwrite(0, 0, 0, 0);
				break;
			}
		}
		else printf("ClientMessage\n");
		continue;
	}
	if(0 == win)continue;
*/
	if(xev.type == ConfigureNotify)
	{
		int x = xev.xconfigure.width;
		int y = xev.xconfigure.height;
		//printf("%d,%d\n",x,y);

		if((x==win->width)&&(y==win->height))return;
		win->width = win->stride = x;
		win->height = y;

		win->ximage = XCreateImage(
			dsp, visual, 24, ZPixmap,
			0, win->buf,
			x, y, 32, 0
		);

		//why = x + (y<<16);
		//what = hex32('s','i','z','e');
		//eventwrite(why, what, where, 0);
	}//ConfigureNotify
	else if(xev.type == Expose)
	{
		XImage* xi;
		if(win == 0)return;

		xi = win->ximage;
		if(xi == 0)return;

		XPutImage(
			dsp, win->fd, (void*)(win->gc), xi,
			0, 0, 0, 0,
			win->width, win->height
		); 
	}//Expose
	else if(xev.type == ButtonPress)
	{
		x = xev.xbutton.x;
		y = xev.xbutton.y;
		k = xev.xbutton.button;
		if(k == Button1)k = 'l';	//left
		else if(k == Button2)k = 'r';	//right
		else if(k == Button3)k = 'm';	//middle
		else if(k == Button4)k = 'f';	//front
		else if(k == Button5)k = 'b';	//back

		myev.why = x + (y<<16) + (k<<48);
		myev.what = hex32('p', '+', 0, 0);
		myev.where = (u64)win;
		actorwrite_ev(&myev);
		//eventwrite(why, what, where, 0);
	}//ButtonPress
	else if(xev.type == ButtonRelease)
	{
		x = xev.xbutton.x;
		y = xev.xbutton.y;
		k = xev.xbutton.button;
		if(k == Button1)k = 'l';	//left
		else if(k == Button2)k = 'r';	//right
		else if(k == Button3)k = 'm';	//middle
		else if(k == Button4)k = 'f';	//front
		else if(k == Button5)k = 'b';	//back

		myev.why = x + (y<<16) + (k<<48);
		myev.what = hex32('p', '-', 0, 0);
		myev.where = (u64)win;
		actorwrite_ev(&myev);
		//eventwrite(why, what, where, 0);
	}//ButtonRelease
	else if(xev.type == MotionNotify)
	{
		x = xev.xbutton.x;
		y = xev.xbutton.y;
		k = 'l';

		myev.why = x + (y<<16) + (k<<48);
		myev.what = hex32('p', '@', 0, 0);
		myev.where = (u64)win;
		actorwrite_ev(&myev);
		//eventwrite(why, what, where, 0);
	}//MotionNotify
	else if(xev.type == KeyPress)
	{
		//KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
		//if (xev.xkey.keycode == keyQ)break;
		//printf("%x\n",xev.xkey.keycode);

		//普通anscii码
		myev.what = hex32('c','h','a','r');
		if((xev.xkey.state&1) == 1)
		{
			myev.why = xlib2upper[xev.xkey.keycode];
			if(myev.why != 0)
			{
				//eventwrite(why, what, where, 0);
				myev.where = (u64)win;
				actorwrite_ev(&myev);
				return;
			}
		}
		else
		{
			myev.why = xlib2anscii[xev.xkey.keycode];
			if(myev.why != 0)
			{
				//eventwrite(why, what, where, 0);
				myev.where = (u64)win;
				actorwrite_ev(&myev);
				return;
			}
		}

		//控制按键
		myev.why = xlib2kbd[xev.xkey.keycode];
		myev.what = hex32('k','b','d',0);
		myev.where = (u64)win;
		actorwrite_ev(&myev);
		//eventwrite(why, what, where, 0);
	}//KeyPress
}
void* windowthread(struct arena* win)
{
	XEvent xev;
	windowopen(win);
	while(1)
	{
		XNextEvent(dsp, &xev);
		if(ClientMessage == xev.type)
		{
			if(wmDelete == xev.xclient.data.l[0])break;
		}
		windowevent(win, xev);

		if(win->deq != win->enq)
		{
			actorread_all(win);
			XPutImage(
				dsp, win->fd, (void*)(win->gc), win->ximage,
				0, 0, 0, 0,
				win->width, win->height
			); 
			win->deq = win->enq;
		}
	}//while
	windowclose(win);
}
void* terminalthread(void* win)
{
	u64 why, what;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		eventwrite(why, what, 0, 0);
	}
}




void windowread(void* dc,void* df,void* sc,void* sf)
{
/*
	XEvent xev;
	struct arena* win = sc;
	actorread_all(win);

	memset(&xev,0,sizeof(XEvent));
	xev.type = Expose;
	xev.xexpose.display = dsp;
	xev.xexpose.window = win->fd;
	XSendEvent(dsp, win->fd, False, ExposureMask, &xev);
	XFlush(dsp);	//must
*/
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len)
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstop(struct arena* win)
{
	XDestroyWindow(dsp, win->fd);
}
void windowstart(struct arena* win)
{
	int j;
	win->type = hex32('w', 'i', 'n', 0);
	win->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');

	win->width = win->stride = 512;
	win->height = 512;

	win->buf = malloc(0x1000000);
	win->ximage = XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		win->buf, 512, 512,
		32, 0
	);
/*
	if(fuckyou <= 1)
	{
		winmap[1] = win;
		threadcreate(windowthread, win);
	}
	else
	{
		for(j=1;j<0x100;j++)
		{
			if(0 == winmap[j])break;
		}
		winmap[j] = win;

		XClientMessageEvent xev;
		memset(&xev, 0, sizeof(XClientMessageEvent));
		xev.type = ClientMessage;
		xev.window = fdmap[0];
		xev.format = 32;
		xev.data.l[0] = wmCreate;
		xev.data.l[1] = j;
		XSendEvent(dsp, fdmap[1], False, 0, (void*)&xev);
		XFlush(dsp);	//must
	}
*/
	for(j=0;j<16;j++)
	{
		win->touchdown[j].id = 0xffff;
		win->touchmove[j].id = 0xffff;
	}
	threadcreate(windowthread, win);
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
{
	Window root;
/*
	int j;
	for(j=0;j<0x100;j++)
	{
		winmap[j] = 0;
		fdmap[j] = 0;
	}
*/
	//must
	XInitThreads();

	//display
	dsp = XOpenDisplay(NULL);
	root = DefaultRootWindow(dsp);
	//int screen = DefaultScreen(dsp);

	//visual
	visual = DefaultVisual(dsp, 0);
	if(visual->class != TrueColor)
	{
		fprintf(stderr, "Cannot handle non true color visual ...\n");
		XCloseDisplay(dsp);
		exit(1);
	}

	//
	wmDelete = XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	wmCreate = XInternAtom(dsp, "WM_CREATE_WINDOW", True);

	//
	//winmap[0] = (void*)0x1000;
	//fdmap[0] = root;

	//
	fuckyou = 0;
	threadcreate(terminalthread, 0);
}
void freewindow()
{
}
void inittray()
{
}
void freetray()
{
}
