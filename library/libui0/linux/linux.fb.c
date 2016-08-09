#define QWORD unsigned long long
#define DWORD unsigned int


#include<fcntl.h>		//	open
#include<unistd.h>		//	close
#include<stdio.h>		//	printf
#include<stdlib.h>		//	malloc
#include<sys/ioctl.h>		//	ioctl
#include<linux/fb.h>		//	framebuffer
#include<termios.h>		//	termios,getchar




//输入
static int signal=-1;
static struct termios old;
static struct termios new;

//physical info
static unsigned long long fbaddr=0;
static int fbtotalbyte=0;
static int fboneline=0;

//virtual info
static int xmax=0;
static int ymax=0;
static int bpp=0;

//自己的画板
static int fbfd=-1;
static char* screenbuf;
static int width=1024;
static int height=768;








int history[4]={0,0,0,0};
int uievent(QWORD* first,QWORD* second)
{
	while(1)
	{
		history[0]=history[1];
		history[1]=history[2];
		history[2]=history[3];
		history[3]=getchar();
		if(history[2]==0x1b&&history[3]==0x1b)
		{
			*first=0;
			return 1;
		}
		if(history[1]==0x1b&&history[2]==0x5b)
		{
			*first=0x64626b;
			if(history[3]==0x41)//up
			{
				*second=0x26;
				return 1;
			}
			if(history[3]==0x42)//down
			{
				*second=0x28;
				return 1;
			}
			if(history[3]==0x44)//left
			{
				*second=0x25;
				return 1;
			}
			if(history[3]==0x43)//right
			{
				*second=0x27;
				return 1;
			}
		}
	}
}








void listwindow()
{
}
void changewindow()
{
}








void writewindow()
{
	//
	int x,y,ret;

	//5,6,5
	if(bpp==16)
	{
		for(x=0;x<width*height;x++)
		{
			*(unsigned short*)(screenbuf+x*2)=
				    (screenbuf[x*4+0]>>3)
				+ ( (screenbuf[x*4+1]>>2) <<  5 )
				+ ( (screenbuf[x*4+2]>>3) << 11 );
		}
	}

	//
	for(y=0;y<height;y++)
	{
		ret=lseek(fbfd , y*fboneline , SEEK_SET);
		ret=write(fbfd , screenbuf + y*width*bpp/8 , width*bpp/8);
	}
}
QWORD readwindow(QWORD what)
{
	if(what==0x657a6973)
	{
		return width+(height<<16);
	}
}








void startwindow(DWORD size,char* value)
{
	width=size&0xffff;
	height=(size>>16)&0xffff;

	screenbuf = value;
}
void stopwindow()
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

/*
	//input部分
	inputfp=open("/dev/input/event3",O_RDONLY);
	if(inputfp<=0)
	{
		printf("error4(plese sudo)\n");
		exit(-1);
	}
*/

	//输入方法2
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
}
//__attribute__((destructor)) void destoryfb()
void killwindow()
{
	//close(inputfp);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fbfd!=-1)close(fbfd);
}
