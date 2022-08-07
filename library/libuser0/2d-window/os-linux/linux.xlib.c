#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <X11/Xlib.h>
#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);




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





static void restorestackdeliverevent(_obj* wnd, struct event* ev)
{
	struct rgbaxlib* per = &wnd->rgbaxlib;
	if(0 == per->save_stack){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	rgbanode_give(wnd,0, per->save_stack,per->save_sp, 0,0, ev,0);
}
void windowevent(_obj* wnd, XEvent xev)
{
	u64 x,y,k;
	struct event myev;
	struct rgbaxlib* per = &wnd->rgbaxlib;

	if(ClientMessage == xev.type)
	{
		if(wmDelete == xev.xclient.data.l[0])
		{
			eventwrite(0,0,0,0);
			return;
		}
	}
	else if(ConfigureNotify == xev.type)
	{
		int x = xev.xconfigure.width;
		int y = xev.xconfigure.height;
		//printf("%d,%d\n",x,y);

		if((x==wnd->whdf.width)&&(y==wnd->whdf.height))return;
		wnd->whdf.width = x;
		wnd->whdf.height = y;

		wnd->whdf.fbwidth = x*4;
		//wnd->fbheight = 0;

		per->ximage = XCreateImage(
			dsp, visual, 24, ZPixmap,
			0, per->buf,
			x, y, 32, 0
		);

		//why = x + (y<<16);
		//what = hex32('s','i','z','e');
		//eventwrite(why, what, where, 0);
	}//ConfigureNotify
	else if(Expose == xev.type)
	{
		XImage* xi;
		if(0 == wnd)return;

		xi = per->ximage;
		if(xi == 0)return;

		XPutImage(
			dsp, per->xlibfd, per->xlibgc, xi,
			0, 0, 0, 0,
			wnd->whdf.width, wnd->whdf.height
		); 
	}//Expose
	else if(ButtonPress == xev.type)
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
		myev.where = (u64)wnd;
		restorestackdeliverevent(wnd, &myev);
		//eventwrite(why, what, where, 0);
	}//ButtonPress
	else if(ButtonRelease == xev.type)
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
		myev.where = (u64)wnd;
		restorestackdeliverevent(wnd, &myev);
		//eventwrite(why, what, where, 0);
	}//ButtonRelease
	else if(MotionNotify == xev.type)
	{
		x = xev.xbutton.x;
		y = xev.xbutton.y;
		k = 'l';

		myev.why = x + (y<<16) + (k<<48);
		myev.what = hex32('p', '@', 0, 0);
		myev.where = (u64)wnd;
		restorestackdeliverevent(wnd, &myev);
		//eventwrite(why, what, where, 0);
	}//MotionNotify
	else if(KeyPress == xev.type)
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
				myev.where = (u64)wnd;
				restorestackdeliverevent(wnd, &myev);
				return;
			}
		}
		else
		{
			myev.why = xlib2anscii[xev.xkey.keycode];
			if(myev.why != 0)
			{
				//eventwrite(why, what, where, 0);
				myev.where = (u64)wnd;
				restorestackdeliverevent(wnd, &myev);
				return;
			}
		}

		//控制按键
		myev.why = xlib2kbd[xev.xkey.keycode];
		myev.what = hex32('k','b','d',0);
		myev.where = (u64)wnd;
		restorestackdeliverevent(wnd, &myev);
		//eventwrite(why, what, where, 0);
	}//KeyPress
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct rgbaxlib* per = &wnd->rgbaxlib;
	//update screen
	rgbanode_take(wnd,0, stack,sp, arg,key, buf,len);
	XPutImage(
		dsp, per->xlibfd, per->xlibgc, per->ximage,
		0, 0, 0, 0,
		wnd->whdf.width, wnd->whdf.height
	); 

	//save stack to temp
	per->save_stack = stack;
	per->save_sp = sp;

	//handle event
	XEvent xev;
	while(XPending(dsp))
	{
		XNextEvent(dsp, &xev);
		windowevent(wnd, xev);
	}//while

	//clear temp
	per->save_stack = 0;
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct rgbaxlib* per = &wnd->rgbaxlib;
/*
	XEvent xev;
	memset(&xev,0,sizeof(XEvent));
	xev.type = Expose;
	xev.xexpose.display = dsp;
	xev.xexpose.window = per->xlibfd;
	XSendEvent(dsp, per->xlibfd, False, ExposureMask, &xev);
	XFlush(dsp);	//must
*/
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
void window_delete(_obj* wnd)
{
	struct rgbaxlib* per = &wnd->rgbaxlib;
	XDestroyWindow(dsp, per->xlibfd);
	fuckyou--;

	if(0 == fuckyou)
	{
		XCloseDisplay(dsp);
		eventwrite(0,0,0,0);
	}
}
void window_create(_obj* wnd)
{
	struct rgbaxlib* per = &wnd->rgbaxlib;

	wnd->hfmt = _rgba_;
	wnd->vfmt = hex64('b','g','r','a','8','8','8','8');

	wnd->whdf.width = 1024;
	wnd->whdf.height = 768;

	wnd->whdf.fbwidth = 1024*4;
	//wnd->whdf.fbheight = 0;

	//
	per->buf = malloc(2048*2048*4);
	per->ximage = XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		per->buf, 1024, 768,
		32, 0
	);

	//window, gc
	per->xlibfd = XCreateSimpleWindow(
		dsp, RootWindow(dsp,0), 0, 0,
		wnd->whdf.width, wnd->whdf.height,
		1, 0, 0);
	per->xlibgc = XCreateGC(dsp, per->xlibfd, 0, NULL);

	//intercept window delete event 
	XSetWMProtocols(dsp, per->xlibfd, &wmDelete, 1);

	//
	XSelectInput
	(
		dsp, per->xlibfd,
		PointerMotionMask|
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);

	//
	//fdmap[j] = per->xlibfd;
	XMapWindow(dsp, per->xlibfd);
	fuckyou++;
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
	Window root;
	XInitThreads();

	//display
	dsp = XOpenDisplay(NULL);
	if(0 == dsp){
		printf("errno=%d@XOpenDisplay\n",errno);
		return;
	}

	root = DefaultRootWindow(dsp);
	if(0 == root){
		printf("errno=%d@DefaultRootWindow\n",errno);
		return;
	}
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
}
void freewindow()
{
}
