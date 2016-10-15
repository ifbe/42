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
int show();
int command(void*);
void say(char*,...);




//
static int mode = 0;
static char buffer[256];




int uievent(u64* who, u64* first, u64* second)
{
	while(1)
	{
		buffer[0] = getchar();
		if(buffer[0] == -1)
		{
			usleep(1000);
			continue;
		}

		if(buffer[0] == 0x1b)
		{
			buffer[1] = getchar();
			if(buffer[1] == -1)
			{
				buffer[1] = 0;
				break;
			}

			if(buffer[1] == 0x5b)
			{
				buffer[2] = getchar();

				if( (buffer[2]>=0x41) && (buffer[2]<=0x44) )
				{
					buffer[3] = 0;
					break;
				}
			}//5b
		}//1b

		else
		{
			buffer[1] = 0;
			break;
		}
	}

	*first = 0x64626b;
	*second = (u64)buffer;
	return 1;
}




void terminal_passthrough()
{
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag &= ~(ICANON|ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        //fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}
void terminal_normalize()
{
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag |= ICANON|ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void windowchange(int what)
{
	mode = what;
	if(mode == 0)terminal_normalize();
	else if(mode == 1)terminal_passthrough();
}
void windowlist()
{
}




void windowread(char* where)
{
	show();
}
void windowwrite(u64 who, u64 what, u64 how)
{
	int ret;
	if(what == 0x64626b)
	{
		ret = command((char*)how);
	}
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowstop()
{
}




void windowcreate()
{
}
void windowdelete()
{
	windowchange(0);
}
