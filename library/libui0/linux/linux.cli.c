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




//
static int mode = 0;




int uievent(u64* what, u64* who, u64* where, u64* when)
{
	u8* buf = (u8*)what;
	while(1)
	{
		buf[0] = getchar();
		if( (buf[0] == 0) | (buf[0]==0xff) )
		{
			usleep(1000);
			continue;
		}

		if(buf[0] == 0x1b)
		{
			buf[1] = getchar();
			if(buf[1] == 0xff)
			{
				buf[1] = 0;
				break;
			}

			if(buf[1] == 0x5b)
			{
				buf[2] = getchar();

				if( (buf[2]>=0x41) && (buf[2]<=0x44) )
				{
					buf[3] = 0;
					break;
				}
			}//5b
		}//1b

		else
		{
			buf[1] = 0;
			break;
		}
	}

	*who = 0x64626b;
	return 1;
}




void windowchange(int what)
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	mode = what;

	if(mode == 0)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
		t.c_lflag |= ICANON|ECHO;
	}
	else if(mode == 1)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
		t.c_lflag &= ~(ICANON|ECHO);
		t.c_cc[VTIME] = 0;
		t.c_cc[VMIN] = 1;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void windowlist()
{
}
void windowread(char* where)
{
}
void windowwrite()
{
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowstop()
{
}
void windowcreate()
{
	windowchange(1);
}
void windowdelete()
{
	windowchange(0);
}
