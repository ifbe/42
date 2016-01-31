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
//int signal=-1;
//struct termios old;
//struct termios new;

//屏幕
int fbfd=-1;
int fbtotal=0;
int fbline=0;
unsigned long long fbaddr=0;

int xmax=0;
int ymax=0;
int bpp=0;

//自己的画板
static unsigned int* mypixel=NULL;
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
void writewindow(QWORD what)
{
	int y,ret;
	int xxxx,yyyy;

	if(xmax<width)xxxx=xmax;
	else xxxx=width;
	if(ymax<height)yyyy=ymax;
	else yyyy=height;

	for(y=0;y<yyyy;y++)
	{
		ret=lseek(fbfd , y*fbline , SEEK_SET);
		ret=write(fbfd , mypixel+y*width , xxxx*4);
	}
}
QWORD readwindow(QWORD what)
{
	if(what==0x6572656877)
	{
		return (QWORD)mypixel;
	}
	else if(what==0x657a6973)
	{
		return width+(height<<16);
	}
}








void initwindowworker()
{
	//申请内存
	mypixel=(unsigned int*)malloc(0x400000);
	if(mypixel==NULL)
	{
		printf("malloc window failed\n");
		exit(-1);
	}

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
	fbtotal=finfo.smem_len;
	fbline=finfo.line_length;
	printf("@%llx,%x,%x\n",fbaddr,fbtotal,fbline);
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
	printf("xmax=%x,ymax=%x,bpp=%x\n",xmax,ymax,bpp);
/*
	//input部分
	inputfp=open("/dev/input/event3",O_RDONLY);
	if(inputfp<=0)
	{
		printf("error4(plese sudo)\n");
		exit(-1);
	}

	//输入方法2
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
*/
}
//__attribute__((destructor)) void destoryfb()
void killwindowworker()
{
	//close(inputfp);
	//if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fbfd!=-1)close(fbfd);
	free(mypixel);
}
