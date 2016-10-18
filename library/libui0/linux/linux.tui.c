#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<signal.h>
#include<sys/ioctl.h>
#include<sys/select.h>
void say(char*,...);




//窗口
static struct winsize w;
static int width,height;
static int lastwidth=0,lastheight=0;
//输入
static int flag=-1;
static struct termios old;
static struct termios new;
//
static char* textbuf=0;




static void newsize(int num)
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
}
int uievent(u64* who, u64* first, u64* second)
{
	u8 a,b,c,d;

	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			first[0] = 0x657a6973;
			second[0] = width + (height<<16);
			return 1;
		}

		a=getchar();
		if( (a==0xff) | (a==0) )
		{
			usleep(1000);
			continue;
		}

		if(a==0x1b)
		{
			b=getchar();

			if( (b==0xff) | (b==0) )
			{
				*first=0x64626b;
				*second=0x1b;
				return 1;
			}

			if(b==0x5b)
			{
				c=getchar();

				*first=0x64626b;
				if(c==0x41)//up
				{
					*second=0x26;
					return 1;
				}
				if(c==0x42)//down
				{
					*second=0x28;
					return 1;
				}
				if(c==0x44)//left
				{
					*second=0x25;
					return 1;
				}
				if(c==0x43)//right
				{
					*second=0x27;
					return 1;
				}
			}//5b
		}//1b

		else
		{
			if(a == 0x7f)a = 8;
			if(a == 0xa)a = 0xd;

			*first = 0x72616863;
			*second = a;
			return 1;
		}
	}

	return 1;
}




void windowlist()
{
}
void windowchange()
{
}




void windowread(char* addr)
{
}
void windowwrite()
{
	textbuf[width*height] = 0;

	printf("\033[H\033[J");
	printf("%s",textbuf);

	fflush(stdout);
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
	textbuf = addr;
	*(unsigned int*)pixfmt = 0x74786574;
}
void windowstop()
{
}




void windowcreate()
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
	signal(SIGWINCH,newsize);

	//
	flag=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}
void windowdelete()
{
	if(flag!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
