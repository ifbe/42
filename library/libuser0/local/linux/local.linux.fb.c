#include <fcntl.h>		//open
#include <unistd.h>		//close
#include <stdio.h>		//printf
#include <stdlib.h>		//malloc
#include <termios.h>		//termios,getchar
#include <sys/ioctl.h>		//ioctl
#include <linux/fb.h>		//framebuffer
#include "arena.h"
int lowlevel_input();




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




void* terminalthread(void* win)
{
	u64 why, what, where;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		where = (u64)win;
		eventwrite(why, what, where, 0);
	}//while
}








void windowwrite(struct window* dst, struct window* src)
{
	//
	int x,y,ret;
	u8* buf = (void*)(src->buf);

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
void windowstop()
{
}
void windowstart(struct window* this)
{
	int j;
	this->w = xmax;
	this->h = ymax;
	if(this->type == hex32('b','u','f',0))
	{
		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');
		return;
	}
	else
	{
		this->type = hex32('w', 'i', 'n', 0);
		this->fmt = hex32('f', 'b', 0, 0);

		for(j=0;j<16;j++)
		{
			(this->touch[j]).id = 0xffff;
		}
	}
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
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
	thread = startthread(terminalthread, 0);
}
//__attribute__((destructor)) void destoryfb()
void freewindow()
{
	//close(inputfp);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fbfd!=-1)close(fbfd);
}
