#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<termios.h>
void say(char*,...);




//
static int lastwidth=0,lastheight=0;
static int width,height;
static char* textbuf=0;
//输入
static int signal=-1;
static struct termios old;
static struct termios new;
static int history[4]={0,0,0,0};




int uievent(QWORD* first,QWORD* second)
{
        int i;
        char* ret;

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




void windowinit()
{
	//
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row -1;

	//
	signal=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
}
void windowkill()
{
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
