#include <conio.h>
#include <stdio.h>
#include <windows.h>
#define QWORD unsigned long long
#define DWORD unsigned int




//
static HANDLE h;
static int lastwidth=0,lastheight=0;
static int width,height;
//
static char* content;




int uievent(QWORD* first,QWORD* second)
{
	unsigned char ch;

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
		ch = getch();
		printf("%x",ch);
		if(ch==3)
		{
			*first = 0;
			return 1;
		}

		else if(ch == 0x1b)
		{
			*first=0x64626b;
			*second=0x1b;
			return 1;
		}

		else if(ch == 0xe0)
		{
			ch = getch();
			printf("%x",ch);

			*first = 0x64626b;
			if(ch == 0x48)	//up
			{
				*second = 0x26;
				return 1;
			}
			else if(ch == 0x50)	//down
			{
				*second = 0x28;
				return 1;
			}
			else if(ch == 0x4b)	//left
			{
				*second = 0x25;
				return 1;
			}
			else if(ch == 0x4d)	//right
			{
				*second = 0x27;
				return 1;
			}
		}

		else
		{
			*first = 0x72616863;
			*second = ch;
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




void windowread(char* where)
{
}
void windowwrite()
{
	COORD pos = {0,0};
	SetConsoleCursorPosition(h,pos);

	content[width*height]=0;
	printf("%s",content);
	fflush(stdout);
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
	content = addr;
	*(unsigned int*)pixfmt = 0x74786574;
}
void windowstop()
{
}




void windowinit()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &bInfo );
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;

}
void windowkill()
{
}
