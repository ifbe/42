#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<X11/Xlib.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
u64* eventread();
void eventwrite(u64,u64,u64,u64);
//
u64 startthread(void*, void*);
void stopthread();




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

//global
static Display* dsp=0;
static Visual* visual=0;
//
struct xlibdata
{
	//cfg
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u64 thread;
	int motioncount;
	int oldx;
	int oldy;

	//lib
	Window win;
	GC gc;
	Atom wmDelete;
	XImage* ximage;
};




void* uievent(struct xlibdata* p)
{
	XEvent ev;

	//ximage
	p->ximage = XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		(void*)p->buf, p->w, p->h,
		32, 0
	);

	//window, gc
	p->win = XCreateSimpleWindow(
		dsp, RootWindow(dsp,0), 0, 0,
		p->w, p->h,
		1, 0, 0);
	p->gc = XCreateGC(dsp, p->win, 0, NULL);

	//intercept window delete event 
	p->wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, p->win, &(p->wmDelete), 1);
	XSelectInput
	(
		dsp, p->win,
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);
	XMapWindow(dsp, p->win);

	while(1)
	{
		XNextEvent(dsp, &ev);
		if(ev.type==ButtonPress)
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
				p->oldx=ev.xbutton.x;
				p->oldy=ev.xbutton.y;
			}

			continue;
		}
		else if(ev.type==ButtonRelease)
		{
			//printf("buttonrelease\n");
			if(ev.xbutton.button==Button1)
			{
				if((p->oldx==ev.xbutton.x)&&
				   (p->oldy==ev.xbutton.y) )
				{
					eventwrite(
					ev.xbutton.x + (ev.xbutton.y<<16) + ((u64)1<<48),
					0x2d6d, 0, 0
					);
				}
			}
		}
		else if(ev.type==ClientMessage)
		{
			if (ev.xclient.data.l[0] == p->wmDelete)
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

			if( (x==p->w) && (y==p->h) )continue;
			p->w=x;
			p->h=y;

			p->ximage = XCreateImage(
				dsp, visual, 24, ZPixmap,
				0, (void*)p->buf,
				x,y,32,0
			);

			eventwrite(x+(y<<16), 0x657a6973, 0, 0);
		}
		else if(ev.type==Expose)
		{
			if(p->buf==0)continue;
			if(p->ximage==0)continue;
			XPutImage(
				dsp, p->win, p->gc, p->ximage,
				0, 0, 0, 0,
				p->w, p->h
			); 
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
		else if(ev.type==MotionNotify)
		{
			p->motioncount = (p->motioncount+1)%5;
			if(p->motioncount != 0)continue;

			eventwrite(
			( (ev.xbutton.y - p->oldy) << 16 )
			+ (ev.xbutton.x - p->oldx)
			+ ((u64)1<<48),

			0x406d, 0, 0
			);
			p->oldx = ev.xbutton.x;
			p->oldy = ev.xbutton.y;
		}
	}//while

	//
	XDestroyWindow(dsp, p->win);
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
void windowwrite(struct xlibdata* p)
{
	XEvent ev;
	memset(&ev,0,sizeof(XEvent));
	ev.type = Expose;
	ev.xexpose.display = dsp;
	ev.xexpose.window = p->win;
	XSendEvent(dsp, p->win, False, ExposureMask, &ev);
	XFlush(dsp);	//must
}
void windowstart(struct xlibdata* p)
{
	int j = sizeof(struct xlibdata);
	if(j > 256)
	{
		printf("error@256<%d\n", j);
	}

	//malloc
	p->buf = (u64)malloc(2048*1024*4);
	if(p->buf == 0)printf("error@malloc\n");

	//
	p->thread = startthread(uievent, p);
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
}
void windowdelete()
{
}
