#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<X11/Xlib.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void eventwrite(u64,u64,u64,u64);
void snatch(int*);
void release(int*);




static char xlib2anscii[0x80]={
0,0,0,0,		0,0,0,0,
0,0,'1','2',		'3','4','5','6',	//0
'7','8','9','0',	'-','=',0x8,0x9,
'q','w','e','r',	't','y','u','i',	//0x10
'o','p','[',']',	0xd,0,'a','s',
'd','f','g','h',	'j','k','l',';',	//0x20
'\'','`',0,'\\',	'z','x','c','v',
'b','n','m',',',	'.','/',0,'*',		//0x30
0,' ',0,0,		0,0,0,0,
0,0,0,0,		0,0,0,'7',		//0x40
'8','9','-','4',	'5','6','+','1',
'2','3','0','.',	0,0,0,0,		//0x50
0,0,0,0,		0,0,0,0,
0,0,0,0,		0,0,0,0,		//0x60,0x6f
0,0,0,0,		0,0,0,0,
0,0,0,0,		0,0,0,0,		//0x70,0x7f
};
static char xlib2kbd[0x80]={
0,0,0,0,		0,0,0,0,
0,0x1b,'1','2',		'3','4','5','6',	//0
'7','8','9','0',	'-','=',0x8,0x9,
'q','w','e','r',	't','y','u','i',	//0x10
'o','p','[',']',	0xd,0xff,'a','s',
'd','f','g','h',	'j','k','l',';',	//0x20
'\'','`',0xff,'\\',	'z','x','c','v',
'b','n','m',',',	'.','/',0xff,'*',	//0x30
0xff,' ',0xff,0x70,	0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,	0x79,0xff,0xff,'7',	//0x40
'8','9','-','4',	'5','6','+','1',
'2','3','0','.',	0xff,0xff,0xff,0xff,	//0x50
0xff,0xff,0xff,0xff,	0,0,0,0,
0,0,0,0,		0,0,0,0x26,		//0x60,0x6f
0,0x25,0x27,0,		0x28,0,0,0,
0,0,0,0,		0,0,0,0,		//0x70,0x7f
};

//
static pthread_t id;
//
static Display* dsp=0;
static Visual *visual=0;
static XImage* ximage=0;
static Window win;
static GC gc;
static Atom wmDelete;
//
static char* pixbuf=0;
static int pixfmt=0;
static int width=512;
static int height=512;
//
static int motioncount=0;
static int oldx=0;
static int oldy=0;




void* uievent(void* p)
{
	XEvent ev;
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

	while(1)
	{
		XNextEvent(dsp, &ev);
		if(ev.type==Expose)
		{
			if(pixbuf==0)continue;
			if(ximage==0)continue;
			XPutImage(
				dsp, win, gc, ximage,
				0, 0, 0, 0,
				width, height
			); 
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

			if( (x==width) && (y==height) )continue;
			width=x;
			height=y;

			ximage=XCreateImage(
				dsp,visual,24,ZPixmap,0,
				pixbuf,x,y,32,0
			);

			eventwrite(x+(y<<16), 0x657a6973, 0, 0);
		}
		else if(ev.type==ButtonPress)
		{
			//printf("buttonpress\n");
			if(ev.xbutton.button==Button4)	//'xyz fron'
			{
				eventwrite(
				ev.xbutton.x + (ev.xbutton.y<<16) + ((u64)4<<48),
				0x2b6d, 0, 0
				);
			}
			else if(ev.xbutton.button==Button5)	//'xyz down'
			{
				eventwrite(
				ev.xbutton.x + (ev.xbutton.y<<16) + ((u64)5<<48),
				0x2b6d, 0, 0
				);
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
					eventwrite(
					ev.xbutton.x + (ev.xbutton.y<<16) + ((u64)1<<48),
					0x2d6d, 0, 0
					);
				}
			}
		}
		else if(ev.type==MotionNotify)
		{
			motioncount = (motioncount+1)%5;
			if(motioncount != 0)continue;

			eventwrite(
			( (ev.xbutton.y-oldy) << 16 ) + ev.xbutton.x-oldx + ((u64)1<<48),
			0x406d, 0, 0
			);
			oldx=ev.xbutton.x;
			oldy=ev.xbutton.y;
		}
		else if(ev.type==KeyPress)
		{
			char temp;
			//KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
			//if (ev.xkey.keycode == keyQ)break;
			//printf("%x\n",ev.xkey.keycode);

			//普通anscii码
			temp=xlib2anscii[ev.xkey.keycode];
			if(temp!=0)eventwrite(temp, 0x72616863, 0, 0);

			//控制按键
			else eventwrite(xlib2kbd[ev.xkey.keycode], 0x64626b, 0, 0);
		}
	}//while

	//
	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);
}




void windowlist()
{
}
void windowchange()
{
}
void windowread()
{
}
void windowwrite()
{
	XEvent ev;
	memset(&ev,0,sizeof(XEvent));
	ev.type = Expose;
	ev.xexpose.display = dsp;
	ev.xexpose.window = win;
	XSendEvent(dsp, win, False, ExposureMask, &ev);
	XFlush(dsp);	//must
}
void windowstart(char* addr, char* fmt, int x, int y)
{
	//wait for pthread inited
	usleep(10000);

	//
	width = x;
	height = y;
	XResizeWindow(dsp, win, width, height);

	//
	pixbuf = addr;
	pixfmt=32;

	//
	ximage=XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		pixbuf, width, height, 32, 0
	);
}
void windowstop()
{
	if(pixfmt == 8)
	{
		pixbuf=0;
	}
	else pixbuf=0;
}
void windowcreate()
{
	//
	//XInitThreads();
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
}
