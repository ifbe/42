#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
void say(char*,...);




//
static int lastwidth=0,lastheight=0;
static int width,height;
static char* textbuf=0;
//
char clibuffer[128];




int uievent(QWORD* first,QWORD* second)
{
        int i;
        char* ret;
	//say("@uievent.start\n");

	if(lastwidth != width)
	{
		lastwidth = width;
		lastheight = height;
		first[0] = 0x657a6973;
		second[0] = width + (height<<16);
		return 1;
	}

        for(i=0;i<128;i++)clibuffer[i]=0;
        while(1)
        {
                ret=fgets(clibuffer,128,stdin);
		//say("uievent.ret=%x\n",ret);

		if( ret == NULL )
		{
			first[0]=0;
			break;
		}
                if( clibuffer[0] != 0 )
		{
			first[0]=0x727473;
			second[0]=(QWORD)clibuffer;
			break;
		}
        }

	//say("@uievent.return\n");
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
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row -1;
	say("width=%d,height=%d\n",width,height);
}
void windowkill()
{
	
}
