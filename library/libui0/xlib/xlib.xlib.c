#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#define QWORD unsigned long long
#define DWORD unsigned int




static char xlib2anscii[0x80]={
0,0,0,0,		0,0,0,0,		0,0,'1','2',		'3','4','5','6',//0
'7','8','9','0',	'-','=',0x8,0x9,	'q','w','e','r',	't','y','u','i',//0x10
'o','p','[',']',	0xd,0,'a','s',		'd','f','g','h',	'j','k','l',';',//0x20
'\'','`',0,'\\',	'z','x','c','v',	'b','n','m',',',	'.','/',0,'*',	//0x30
0,' ',0,0,		0,0,0,0,		0,0,0,0,		0,0,0,'7',	//0x40
'8','9','-','4',	'5','6','+','1',	'2','3','0','.',	0,0,0,0,	//0x50
0,0,0,0,		0,0,0,0,		0,0,0,0,		0,0,0,0,	//0x60,0x6f
0,0,0,0,		0,0,0,0,		0,0,0,0,		0,0,0,0,	//0x70,0x7f
};
static char xlib2kbd[0x80]={
0,0,0,0,		0,0,0,0,		0,0x1b,'1','2',		'3','4','5','6',//0
'7','8','9','0',	'-','=',0x8,0x9,	'q','w','e','r',	't','y','u','i',//0x10
'o','p','[',']',	0xd,0xff,'a','s',	'd','f','g','h',	'j','k','l',';',//0x20
'\'','`',0xff,'\\',	'z','x','c','v',	'b','n','m',',',	'.','/',0xff,'*',//0x30
0xff,' ',0xff,0x70,	0x71,0x72,0x73,0x74,	0x75,0x76,0x77,0x78,	0x79,0xff,0xff,'7',//0x40
'8','9','-','4',	'5','6','+','1',	'2','3','0','.',	0xff,0xff,0xff,0xff,//0x50
0xff,0xff,0xff,0xff,	0,0,0,0,		0,0,0,0,		0,0,0,0x26,	//0x60,0x6f
0,0x25,0x27,0,		0x28,0,0,0,		0,0,0,0,		0,0,0,0,	//0x70,0x7f
};

Display* dsp;
Visual *visual;
XImage* ximage;
Window win;
GC gc;
Atom wmDelete;

int width=1024;
int height=768;
int oldx=0;
int oldy=0;




int uievent(QWORD* my1,QWORD* my2)
{
	XEvent ev;
	while(1)
	{
		XNextEvent(dsp, &ev);
		/*
		if(ev.type==Expose)
		{
			if (ev.xexpose.count == 0) writewindow(0,0);
		}
		*/
		if(ev.type==ClientMessage)
		{
			if (ev.xclient.data.l[0] == wmDelete)
			{
				*my1=0;
				return 1;
			}
		}
		else if(ev.type==ButtonPress)
		{
			//printf("buttonpress\n");
			if(ev.xbutton.button==Button4)	//'xyz fron'
			{
				*my1=0x6E6F7266207A7978;
				*my2=ev.xbutton.x + (ev.xbutton.y<<16);
				return 1;
			}
			else if(ev.xbutton.button==Button5)	//'xyz down'
			{
				*my1=0x6B636162207A7978;
				*my2=ev.xbutton.x + (ev.xbutton.y<<16);
				return 1;
			}

			else if(ev.xbutton.button==Button1)
			{
				oldx=ev.xbutton.x;
				oldy=ev.xbutton.y;
			}

			continue;
		}
		else if(ev.type==ButtonRelease)
		{
			//printf("buttonrelease\n");
			if(ev.xbutton.button==Button1)
			{
				if((oldx==ev.xbutton.x)&&(oldy==ev.xbutton.y))
				{
					*my1=0x7466656C207A7978;//'xyz left'
					*my2=ev.xbutton.x + (ev.xbutton.y<<16);
					return 1;
				}
			}
		}
		else if(ev.type==MotionNotify)
		{
			int temp=(ev.xbutton.x-oldx)*(ev.xbutton.y-oldy);
			if(temp<0)temp=-temp;
			if(temp<64)continue;

			*my1=0x65766F6D207A7978;         //'xyz move'
			*my2=( (ev.xbutton.y-oldy) << 16 ) + ev.xbutton.x-oldx;
			oldx=ev.xbutton.x;
			oldy=ev.xbutton.y;
			return 1;
		}
		else if(ev.type==KeyPress)
		{
			char temp;
			//KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
			//if (ev.xkey.keycode == keyQ)break;
			//printf("%x\n",ev.xkey.keycode);


			//普通anscii码
			temp=xlib2anscii[ev.xkey.keycode];
			if(temp!=0)
			{
				*my1=0x72616863;
				*my2=temp;
				return 1;
			}

			//控制按键
			*my1=0x64626b;
			*my2=xlib2kbd[ev.xkey.keycode];
			return 1;
		}
	}
}
QWORD readwindow(QWORD what)
{
	//'size'
	if(what==0x657a6973)
	{
		return width+(height<<16);
	}

	//??????????
	return 0;
}
void writewindow(QWORD type,QWORD value)
{

	//'title'
	if(type==0x656c746974)
	{
		return;
	}

	//'size'
	if(type==0x657a6973)            //'size'
	{
		width=value&0xffff;
		height=(value>>16)&0xffff;
		XResizeWindow(dsp, win, width, height);
		return;
	}

	width=type&0xffff;
	height=(type>>16)&0xffff;

	//
	ximage=XCreateImage(
		dsp,visual,24,ZPixmap,0,
		(char*)value,1024,768,32,0
	);
	XPutImage(dsp, win, gc, ximage, 0, 0, 0, 0, 1024, 768); 
}








void initwindowworker()
{
	//初始化
	dsp = XOpenDisplay(NULL);
	//int screen = DefaultScreen(dsp);

	visual = DefaultVisual(dsp, 0);
	if(visual->class!=TrueColor)
	{
		fprintf(stderr, "Cannot handle non true color visual ...\n");
		XCloseDisplay(dsp);
		exit(1);
	}

	//pixel,ximage,window,gc
	win=XCreateSimpleWindow(dsp,RootWindow(dsp,0),0,0,width,height,1,0,0);
	gc = XCreateGC(dsp, win, 0, NULL);

	// intercept window delete event 
	wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, win, &wmDelete, 1);
	XSelectInput
	(
		dsp, win,
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);
	XMapWindow(dsp, win);
}
void killwindowworker()
{
	//
	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);
}
