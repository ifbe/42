#include <fcntl.h>		//open
#include <unistd.h>		//close
#include <stdio.h>		//printf
#include <stdlib.h>		//malloc
#include <errno.h>
#include <termios.h>		//termios,getchar
#include <sys/ioctl.h>		//ioctl
#include <sys/mman.h>		//mmap
#include <linux/fb.h>		//framebuffer
#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);



static int fbfd = -1;
static u8* fbmem = 0;
//physical info
static u64 paddr = 0;
static int totalbyte = 0;
static int oneline = 0;
//virtual info
static int xmax = 0;
static int ymax = 0;
static int bpp = 0;
void window_update(_obj* wnd)
{
	int x,y,ret;
	u8* canvas = (void*)(wnd->rgbanode.buf);

	//update screen
	if(16 == bpp)
	{
		if(0 == fbmem){
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
		else{
			for(y=0;y<ymax;y++){
				for(x=0;x<xmax;x++){
					*(u16*)(fbmem+y*oneline+x*2) =
					    (canvas[x*4+0]>>3)
					+ ( (canvas[x*4+1]>>2) <<  5 )
					+ ( (canvas[x*4+2]>>3) << 11 );
				}
			}
		}
	}
	else if(24 == bpp)
	{
		if(0 == fbmem){
			x = xmax*bpp/8;
			for(y=0;y<ymax;y++)
			{
				ret = lseek(fbfd, y*oneline, SEEK_SET);
				ret = write(fbfd, canvas + y*x, x);
			}
		}
		else{
			for(y=0;y<ymax;y++){
				for(x=0;x<xmax;x++){
					fbmem[y*oneline+3*x+0] = canvas[y*oneline+4*x+0];
					fbmem[y*oneline+3*x+1] = canvas[y*oneline+4*x+1];
					fbmem[y*oneline+3*x+2] = canvas[y*oneline+4*x+2];
				}
			}
		}
	}
	else if(32 == bpp)
	{
		if(0 == fbmem){
			lseek(fbfd, 0, SEEK_SET);
			write(fbfd, canvas, totalbyte);
		}
		else{
			for(y=0;y<ymax;y++){
				for(x=0;x<xmax;x++){
					fbmem[y*oneline+4*x+0] = canvas[y*oneline+4*x+0];
					fbmem[y*oneline+4*x+1] = canvas[y*oneline+4*x+1];
					fbmem[y*oneline+4*x+2] = canvas[y*oneline+4*x+2];
				}
			}
		}
	}
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//read context
	rgbanode_take(wnd,0, stack,sp, arg,key, buf,len);

	//update area
	window_update(wnd);
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int idx, void* buf,int len)
{
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
void window_delete(_obj* w)
{
	if(fbmem != 0){
		munmap(fbmem, totalbyte);
		fbmem = 0;
	}
	if(fbfd != -1){
		close(fbfd);
		fbfd = -1;
	}
}
void window_create(_obj* sup, void* arg)
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
		printf("FBIOGET_FSCREENINFO:errno=%d\n",errno);
		exit(-1);
	}
	paddr = finfo.smem_start;
	totalbyte = finfo.smem_len;
	oneline = finfo.line_length;
	printf("paddr=%llx,totalbyte=%x,oneline=%x\n",paddr,totalbyte,oneline);
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


	fbmem = mmap(NULL, totalbyte, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	printf("mmap=%p\n",fbmem);


	//
	sup->vfmt = _bgra8888_;

	sup->whdf.width  = xmax;
	sup->whdf.height = ymax;

	sup->whdf.fbwidth = oneline;
	//sup->fbheight = 0;

	sup->rgbanode.buf = malloc(2048*1024*4);
	//for(j=0;j<16;j++)w->input[j].id = 0xffff;
}




void* window_alloc()
{
	return supply_alloc();
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
