#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<fcntl.h>		//	open
#include<unistd.h>		//	close
#include<stdio.h>		//	printf
#include<stdlib.h>		//	malloc
#include<termios.h>		//	termios,getchar
#include<pthread.h>
#include<sys/ioctl.h>		//	ioctl
#include<linux/fb.h>		//	framebuffer
void eventwrite(u64,u64,u64,u64);




//输入
static pthread_t id;
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








void* uievent(void* p)
{
	u8 ch;
	while(1)
	{
		if(xmax != width)
		{
			width = xmax;
			height = ymax;
			eventwrite(width + (height<<16), 0x657a6973, 0, 0);
		}

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








void windowlist()
{
}
void windowchange()
{
}








void windowwrite()
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
u64 windowread(u64 what)
{
	if(what==0x657a6973)
	{
		return width+(height<<16);
	}
}








void windowstart(char* addr, char* pixfmt, int x, int y)
{
	screenbuf = addr;
	width = x;
	height = y;
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
	pthread_create(&id, NULL, uievent, NULL);
}
//__attribute__((destructor)) void destoryfb()
void windowdelete()
{
	//close(inputfp);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);

	//
	if(fbfd!=-1)close(fbfd);
}
