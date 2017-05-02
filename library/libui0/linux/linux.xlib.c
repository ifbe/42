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
			if((ev.xkey.state&1) == 1)
			{
			temp=xlib2upper[ev.xkey.keycode];
			if(temp!=0)
			{
				eventwrite(temp, 0x72616863, 0, 0);
				continue;
			}
			}
			else
			{
			temp=xlib2anscii[ev.xkey.keycode];
			if(temp!=0)
			{
				eventwrite(temp, 0x72616863, 0, 0);
				continue;
			}
			}

			//控制按键
			eventwrite(xlib2kbd[ev.xkey.keycode], 0x64626b, 0, 0);
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
