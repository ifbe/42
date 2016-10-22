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
#include<pthread.h>
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
static pthread_t id;




static void newsize(int num)
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
}
void* uievent(void* p)
{
	u8 ch;

	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			eventwrite(width + (height<<16), 0x657a6973);
		}

		ch = getchar();
		if( (ch == 0xff) | (ch == 0) )
		{
			usleep(1000);
			continue;
		}
		else if(ch==0x1b)
		{
			ch = getchar();
			if( (ch == 0xff) | (ch == 0) )
			{
				usleep(1000);
				ch = getchar();
				if( (ch == 0xff) | (ch == 0) )
				{
					eventwrite(0x1b, 0x64626b);
				}
			}

			else if(ch==0x5b)
			{
				ch = getchar();
				if(ch == 0x41)//up
				{
					eventwrite(0x26, 0x64626b);
				}
				if(ch == 0x42)//down
				{
					eventwrite(0x28, 0x64626b);
				}
				if(ch == 0x44)//left
				{
					eventwrite(0x25, 0x64626b);
				}
				if(ch == 0x43)//right
				{
					eventwrite(0x27, 0x64626b);
				}
			}//5b
		}//1b
		else
		{
			if(ch == 0x7f)ch = 8;
			if(ch == 0xa)ch = 0xd;

			eventwrite(ch, 0x72616863);
		}
	}//while
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

	//
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
	if(flag!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
