#define QWORD unsigned long long
#define DWORD unsigned int


#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>




Display* dsp;
XImage* ximage;
Window win;
GC gc;
Atom wmDelete;

int width=1024;
int height=640;




/*
void point(int x, int y, DWORD color)
{
	mypixel[y*width+x] = color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}*/
void writescreen()
{
	XPutImage(dsp, win, gc, ximage, 0, 0, 0, 0, width, height); 
}
void waitevent(QWORD* first,QWORD* second)
{
	XEvent ev;
	while(1)
	{
		XNextEvent(dsp, &ev);
		if(ev.type==Expose)
		{
			if (ev.xexpose.count == 0) writescreen();
		}
		else if(ev.type==ButtonPress)
		{
			writescreen();
		}
		else if(ev.type==KeyPress)
		{
			KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
			if (ev.xkey.keycode == keyQ)break;
			else writescreen();
		}
		else if(ev.type==ClientMessage)
		{
			if (ev.xclient.data.l[0] == wmDelete)break;
		}
	}
}








void initwindow(QWORD mypixel)
{
	//初始化
	dsp = XOpenDisplay(NULL);
	int screen = DefaultScreen(dsp);
	Visual *visual = DefaultVisual(dsp, 0);
	if(visual->class!=TrueColor)
	{
		fprintf(stderr, "Cannot handle non true color visual ...\n");
		XCloseDisplay(dsp);
		exit(1);
	}

	//pixel,ximage,window,gc
	ximage=XCreateImage(dsp,visual,24,ZPixmap,0,(char*)mypixel,width,height,32,0);
	win=XCreateSimpleWindow(dsp,RootWindow(dsp,0),0,0,width,height,1,0,0);
	gc = XCreateGC(dsp, win, 0, NULL);

	// intercept window delete event 
	wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, win, &wmDelete, 1);
	XSelectInput(dsp, win, KeyPressMask|ButtonPressMask|ExposureMask|StructureNotifyMask);
	XMapWindow(dsp, win);
}
void killwindow()
{
	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);
}
