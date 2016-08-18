#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<sys/select.h>
void say(char*,...);




//
static int lastwidth=0,lastheight=0;
static int width,height;
static char* textbuf=0;
//输入
static int signal=-1;
static struct termios old;
static struct termios new;




int uievent(QWORD* first,QWORD* second)
{
	char a,b,c,d;

	if(lastwidth != width)
	{
		lastwidth = width;
		lastheight = height;
		first[0] = 0x657a6973;
		second[0] = width + (height<<16);
		return 1;
	}

	while(1)
	{
		usleep(1000);

		a=getchar();
		if(a == -1)continue;

		if(a==0x1b)
		{
			b=getchar();
			if(b == -1)
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
			printf("%x",a);
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
	printf("\033[H\033[J");
	//printf("%c[%d;%df",0x1B,height,width);
	textbuf[width*height] = 0;
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
	//
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;

	//
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}
void windowdelete()
{
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
