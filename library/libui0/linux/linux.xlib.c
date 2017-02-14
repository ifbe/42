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
static Visual *visual=0;
//
struct abcd
{
	//lib
	pthread_t id;
	Window win;
	GC gc;
	Atom wmDelete;
	XImage* ximage;

	//cfg
	char* pixbuf;
	int pixfmt;
	int width;
	int height;

	//tmp
	int motioncount;
	int oldx;
	int oldy;
};
static struct abcd haha[1];




void* uievent(void* p)
{
	XEvent ev;

	//pixel,ximage,window,gc
	haha[0].win = XCreateSimpleWindow(
		dsp, RootWindow(dsp,0), 0, 0,
		haha[0].width, haha[0].height,
		1, 0, 0);
	haha[0].gc = XCreateGC(dsp, haha[0].win, 0, NULL);

	// intercept window delete event 
	haha[0].wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, haha[0].win, &haha[0].wmDelete, 1);
	XSelectInput
	(
		dsp, haha[0].win,
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		ButtonMotionMask|
		ExposureMask|
		StructureNotifyMask
	);
	XMapWindow(dsp, haha[0].win);

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
				haha[0].oldx=ev.xbutton.x;
				haha[0].oldy=ev.xbutton.y;
			}

			continue;
		}
		else if(ev.type==ButtonRelease)
		{
			//printf("buttonrelease\n");
			if(ev.xbutton.button==Button1)
			{
				if((haha[0].oldx==ev.xbutton.x)&&
				   (haha[0].oldy==ev.xbutton.y) )
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
			if (ev.xclient.data.l[0] == haha[0].wmDelete)
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

			if( (x==haha[0].width) && (y==haha[0].height) )continue;
			haha[0].width=x;
			haha[0].height=y;

			haha[0].ximage=XCreateImage(
				dsp, visual, 24, ZPixmap,
				0, haha[0].pixbuf,
				x,y,32,0
			);

			eventwrite(x+(y<<16), 0x657a6973, 0, 0);
		}
		else if(ev.type==Expose)
		{
			if(haha[0].pixbuf==0)continue;
			if(haha[0].ximage==0)continue;
			XPutImage(
				dsp, haha[0].win, haha[0].gc, haha[0].ximage,
				0, 0, 0, 0,
				haha[0].width, haha[0].height
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
			haha[0].motioncount = (haha[0].motioncount+1)%5;
			if(haha[0].motioncount != 0)continue;

			eventwrite(
			( (ev.xbutton.y - haha[0].oldy) << 16 )
			+ (ev.xbutton.x - haha[0].oldx)
			+ ((u64)1<<48),

			0x406d, 0, 0
			);
			haha[0].oldx = ev.xbutton.x;
			haha[0].oldy = ev.xbutton.y;
		}
	}//while

	//
	XDestroyWindow(dsp, haha[0].win);
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
	ev.xexpose.window = haha[0].win;
	XSendEvent(dsp, haha[0].win, False, ExposureMask, &ev);
	XFlush(dsp);	//must
}
void windowstart(char* addr, char* fmt, int x, int y)
{
	//wait for pthread inited
	haha[0].width = x;
	haha[0].height = y;
	haha[0].pixbuf = addr;
	haha[0].pixfmt = 32;
	haha[0].ximage=XCreateImage(
		dsp, visual, 24, ZPixmap, 0,
		haha[0].pixbuf, haha[0].width, haha[0].height,
		32, 0
	);

	pthread_create(&haha[0].id, NULL, uievent, NULL);
}
void windowstop()
{
	if(haha[0].pixfmt == 8)
	{
		haha[0].pixbuf = 0;
	}
	else haha[0].pixbuf = 0;
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
