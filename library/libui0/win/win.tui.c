#include <conio.h>
#include <stdio.h>
#include <windows.h>
#define u64 unsigned long long
#define u32 unsigned int
void eventwrite(u64,u64);




//
static HANDLE thread=0;
static HANDLE h;
//
static int lastwidth=0,lastheight=0;
static int width,height;
//
static char* content;




DWORD WINAPI uievent(LPVOID pM)
{
	unsigned char ch;

	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			eventwrite(width + (height<<16), 0x657a6973);
		}

		ch = getch();
		if(ch==3)eventwrite(0,0);
		else if(ch == 0x1b)
		{
			eventwrite(0x1b, 0x64626b);
		}
		else if(ch == 0xe0)
		{
			ch = getch();
			if(ch == 0x48)	//up
			{
				eventwrite(0x26, 0x64626b);
			}
			else if(ch == 0x50)	//down
			{
				eventwrite(0x28, 0x64626b);
			}
			else if(ch == 0x4b)	//left
			{
				eventwrite(0x25, 0x64626b);
			}
			else if(ch == 0x4d)	//right
			{
				eventwrite(0x27, 0x64626b);
			}
		}
		else
		{
			eventwrite(ch, 0x72616863);
		}
	}
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

	SetConsoleCursorPosition(h,pos);
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
	content = addr;
	*(unsigned int*)pixfmt = 0x74786574;
}
void windowstop()
{
}




void windowcreate()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &bInfo );
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;

	thread = CreateThread(NULL, 0, uievent, NULL, 0, NULL);
}
void windowdelete()
{
}
