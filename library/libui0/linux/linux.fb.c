#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<fcntl.h>		//	open
#include<unistd.h>		//	close
#include<stdio.h>		//	printf
#include<stdlib.h>		//	malloc
#include<termios.h>		//	termios,getchar
#include<sys/ioctl.h>		//	ioctl
#include<linux/fb.h>		//	framebuffer
u64* eventread();
void eventwrite(u64,u64,u64,u64);
//
u64 startthread(void*, void*);
void stopthread();




//输入
static u64 thread;
static int signal=-1;
static struct termios old;
static struct termios new;

//physical info
static int fbfd=-1;
static u64 fbaddr=0;
static int fbtotalbyte=0;
static int fboneline=0;

//virtual info
static int xmax=0;
static int ymax=0;
static int bpp=0;

//自己的画板
struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;
};








void* uievent(void* p)
{
	u8 ch;
	while(1)
	{
		ch = getchar();
		if( (ch == 0xff) | (ch == 0) )
		{
			usleep(10000);
			continue;
		}
		else if(ch==0x1b)
		{
			ch = getchar();
			if( (ch == 0xff) | (ch == 0) )
			{
				usleep(10000);
				ch = getchar();
				if( (ch == 0xff) | (ch == 0) )
				{
					eventwrite(0x1b, 0x64626b, 0, 0);
				}
			}

			else if(ch==0x5b)
			{
				ch = getchar();
				if(ch == 0x41)//up
				{
					eventwrite(0x26, 0x64626b, 0, 0);
				}
				if(ch == 0x42)//down
				{
					eventwrite(0x28, 0x64626b, 0, 0);
				}
				if(ch == 0x44)//left
				{
					eventwrite(0x25, 0x64626b, 0, 0);
				}
				if(ch == 0x43)//right
				{
					eventwrite(0x27, 0x64626b, 0, 0);
				}
			}//5b
		}//1b
		else
		{
			if(ch == 0x7f)ch = 8;
			if(ch == 0xa)ch = 0xd;

			eventwrite(ch, 0x72616863, 0, 0);
		}
	}//while
}








void windowwrite(struct window* p)
{
	//
	int x,y,ret;
	u8* buf = (void*)(p->buf1);

	//5,6,5
	if(bpp==16)
	{
		for(x=0;x<xmax*ymax;x++)
		{
			*(u16*)(buf+x*2) =
				    (buf[x*4+0]>>3)
				+ ( (buf[x*4+1]>>2) <<  5 )
				+ ( (buf[x*4+2]>>3) << 11 );
		}
	}

	//
	x = xmax*bpp/8;
	for(y=0;y<ymax;y++)
	{
		ret=lseek(fbfd, y*fboneline, SEEK_SET);
		ret=write(fbfd, buf + y*x, x);
	}
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
void windowstart(struct window* p)
{
	p->buf1 = (u64)malloc(xmax*ymax*4);
	p->w = xmax;
	p->h = ymax;
}
void windowstop()
{
}
void windowcreate()
{
	//目的地
	fbfd=open("/dev/fb0",O_RDWR);
	if(fbfd<0)
	{
		printf("error1(plese sudo)\n");
		exit(-1);
	}

	//固定参数
	struct fb_fix_screeninfo finfo;
	if(ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo))
	{
		printf("error2\n");
		exit(-1);
	}
	fbaddr=finfo.smem_start;
	fbtotalbyte=finfo.smem_len;
	fboneline=finfo.line_length;
	printf("fbaddr=%llx,fbtotalbyte=%x,fboneline=%x\n",fbaddr,fbtotalbyte,fboneline);
	printf("linelen=%x(%d)\n",finfo.line_length,finfo.line_length);

	//可变参数
	struct fb_var_screeninfo vinfo;
	if(ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo))
	{
		printf("error3\n");
		exit(-1);
	}
	xmax=vinfo.xres;
	ymax=vinfo.yres;
	bpp=vinfo.bits_per_pixel;
	printf("xmax=%d,ymax=%d,bpp=%d\n",xmax,ymax,bpp);

	//输入方法2
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

	//
	thread = startthread(uievent, 0);
}
//__attribute__((destructor)) void destoryfb()
void windowdelete()
{
	//close(inputfp);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fbfd!=-1)close(fbfd);
}
