#define QWORD unsigned long long
#define DWORD unsigned int


#include<fcntl.h>	//	open
#include<unistd.h>	//	close
#include<stdio.h>	//	printf
#include<stdlib.h>	//	malloc
#include<sys/ioctl.h>	//	ioctl
#include<linux/fb.h>	//	framebuffer
#include<linux/input.h>	//	/dev/input/event
#include<termios.h>	//	termios,getchar


//第一种输入方式
//int inputfp=-1;

//第二种输入方式
int signal=-1;
struct termios old;
struct termios new;

//屏幕
int framebufferfp=-1;
int onelinebytes=0;

//自己的画板
static unsigned int* mypixel=NULL;
static int width=1024;
static int height=768;




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
void writewindow(QWORD what)
{
	int y;
	for(y=0;y<height;y++)
	{
		lseek(framebufferfp,y*onelinebytes,SEEK_SET);
		write(framebufferfp,mypixel+y*width,width*4);
	}
}
/*
void waitevent(QWORD* first,QWORD* second)
{
	//use /dev/input/event3
	struct input_event t;
	while(1)
	{
		if(read(inputfp,&t,sizeof(t)) == sizeof(t) )
		{
			if(t.value==1)	//press down
			{
				//printf("code:%x\n",t.code);
				if(t.code==KEY_ESC)
				{
					*first=0;
					return;
				}
				else if(t.code==0x67)	//up
				{
					*first=0x776f727261;
					*second=0x26;
					return;
				}
				else if(t.code==0x6c)	//down
				{
					*first=0x776f727261;
					*second=0x28;
					return;
				}
				else if(t.code==0x69)	//left
				{
					*first=0x776f727261;
					*second=0x25;
					return;
				}
				else if(t.code==0x6a)	//right
				{
					*first=0x776f727261;
					*second=0x27;
					return;
				}
			}
		}
	}
}
*/
int history[4]={0,0,0,0};
void waitevent(QWORD* first,QWORD* second)
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
			return;
		}
		if(history[1]==0x1b&&history[2]==0x5b)
		{
			*first=0x776f727261;
			if(history[3]==0x41)//up
			{
				*second=0x26;
				return;
			}
			if(history[3]==0x42)//down
			{
				*second=0x28;
				return;
			}
			if(history[3]==0x44)//left
			{
				*second=0x25;
				return;
			}
			if(history[3]==0x43)//right
			{
				*second=0x27;
				return;
			}
		//printf("%c\n",ch);
		}
	}
}








//__attribute__((constructor)) void initfb()
void initwindowworker()
{
	//申请内存
	mypixel=(unsigned int*)malloc(0x400000);

	//目的地
	framebufferfp=open("/dev/fb0",O_RDWR);
	if(framebufferfp<0)
	{
		printf("error1(plese sudo)\n");
		exit(-1);
	}
	printf("test1\n");

	//固定参数
	struct fb_fix_screeninfo finfo;
	if(ioctl(framebufferfp,FBIOGET_FSCREENINFO,&finfo))
	{
		printf("error2\n");
		exit(-1);
	}
	onelinebytes=finfo.line_length;
	printf("memlen=%x(%d)\n",finfo.smem_len,finfo.smem_len);
	printf("linelen=%x(%d)\n",finfo.line_length,finfo.line_length);

	//可变参数
	struct fb_var_screeninfo vinfo;
	if(ioctl(framebufferfp,FBIOGET_VSCREENINFO,&vinfo))
	{
		printf("error3\n");
		exit(-1);
	}
	printf("xres=%x(%d)\n",vinfo.xres,vinfo.xres);
	printf("yres=%x(%d)\n",vinfo.yres,vinfo.yres);
	printf("bpp=%x(%d)\n",vinfo.bits_per_pixel,vinfo.bits_per_pixel);

	//input部分
	//inputfp=open("/dev/input/event3",O_RDONLY);
	//if(inputfp<=0)
	//{
	//	printf("error4(plese sudo)\n");
	//	exit(-1);
	//}

	//输入方法2
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
}
//__attribute__((destructor)) void destoryfb()
void killwindowworker()
{
	//close(inputfp);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(framebufferfp!=-1)close(framebufferfp);

	//
	free(mypixel);
}
