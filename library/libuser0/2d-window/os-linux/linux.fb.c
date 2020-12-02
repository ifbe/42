#include <fcntl.h>		//open
#include <unistd.h>		//close
#include <stdio.h>		//printf
#include <stdlib.h>		//malloc
#include <termios.h>		//termios,getchar
#include <sys/ioctl.h>		//ioctl
#include <linux/fb.h>		//framebuffer
#include "libuser.h"
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);



//physical info
static u64 fbaddr = 0;
static int fbfd = -1;
static int fbtotalbyte = 0;
static int fboneline = 0;
//virtual info
static int xmax = 0;
static int ymax = 0;
static int bpp = 0;




void window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	int x,y,ret;
	u8* canvas;

	//read context
	rgbanode_take(wnd,0, stack,sp, arg,key, buf,len);

	//update screen
	canvas = (void*)(wnd->rgbabuf);
	if(16 == bpp)
	{
		for(x=0;x<xmax*ymax;x++)
		{
			*(u16*)(canvas+x*2) =
			    (canvas[x*4+0]>>3)
			+ ( (canvas[x*4+1]>>2) <<  5 )
			+ ( (canvas[x*4+2]>>3) << 11 );
			//*(u16*)(canvas+x*2) = 0x4567;
		}
		lseek(fbfd, 0, SEEK_SET);
		write(fbfd, canvas, xmax*ymax*2);
	}
	else if(24 == bpp)
	{
		x = xmax*bpp/8;
		for(y=0;y<ymax;y++)
		{
			ret = lseek(fbfd, y*fboneline, SEEK_SET);
			ret = write(fbfd, canvas + y*x, x);
		}
	}
	else if(32 == bpp)
	{
		lseek(fbfd, 0, SEEK_SET);
		write(fbfd, canvas, fbtotalbyte);
	}
}
void window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len)
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
void windowstart()
{
}
void windowdelete(struct supply* w)
{
	if(fbfd != -1)close(fbfd);
}
void windowcreate(struct supply* sup, void* arg)
{
	int j;
	if(0 == arg)arg = "/dev/fb0";

	//目的地
	fbfd = open(arg, O_RDWR);
	if(fbfd < 0)
	{
		printf("chmod /dev/fb0, or sudo!\n");
		exit(-1);
	}

	//固定参数
	struct fb_fix_screeninfo finfo;
	if(ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo))
	{
		printf("error2\n");
		exit(-1);
	}
	fbaddr = finfo.smem_start;
	fbtotalbyte = finfo.smem_len;
	fboneline = finfo.line_length;
	printf("fbaddr=%llx,fbtotalbyte=%x,fboneline=%x\n",fbaddr,fbtotalbyte,fboneline);
	printf("linelen=%x(%d)\n",finfo.line_length,finfo.line_length);

	//可变参数
	struct fb_var_screeninfo vinfo;
	if(ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo))
	{
		printf("error3\n");
		exit(-1);
	}
	xmax = vinfo.xres;
	ymax = vinfo.yres;
	bpp = vinfo.bits_per_pixel;
	printf("xmax=%d,ymax=%d,bpp=%d\n",xmax,ymax,bpp);




	//
	sup->fmt = _rgba_;
	sup->vfmt = hex64('b','g','r','a','8','8','8','8');

	sup->width  = xmax;
	sup->height = ymax;

	sup->fbwidth = fboneline;
	//sup->fbheight = 0;

	sup->rgbabuf = malloc(2048*1024*4);
	//for(j=0;j<16;j++)w->input[j].id = 0xffff;
}




void initwindow()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void freewindow()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
	t.c_lflag |= ICANON|ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
