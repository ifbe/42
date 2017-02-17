#include <conio.h>
#include <stdio.h>
#include <windows.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void eventwrite(u64,u64,u64,u64);




//
static HANDLE thread=0;
static HANDLE output;
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
			eventwrite(width + (height<<16), 0x657a6973, 0, 0);
		}

		ch = getch();
		if(ch==3)eventwrite(0,0,0,0);
		else if(ch == 0x1b)
		{
			eventwrite(0x1b, 0x64626b, 0, 0);
		}
		else if(ch == 0xe0)
		{
			ch = getch();
			if(ch == 0x48)	//up
			{
				eventwrite(0x26, 0x64626b, 0, 0);
			}
			else if(ch == 0x50)	//down
			{
				eventwrite(0x28, 0x64626b, 0, 0);
			}
			else if(ch == 0x4b)	//left
			{
				eventwrite(0x25, 0x64626b, 0, 0);
			}
			else if(ch == 0x4d)	//right
			{
				eventwrite(0x27, 0x64626b, 0, 0);
			}
		}
		else
		{
			eventwrite(ch, 0x72616863, 0, 0);
		}
	}
}
static void windowsutf8(char* utf8)
{
	int ret;
	char unicode[8];
	char gbk[8];
	ret = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (void*)unicode, 4);
	ret = WideCharToMultiByte(CP_ACP, 0, (void*)unicode, -1, gbk, 4, NULL, NULL);
	printf("%s",gbk);
}
static void attr(u8 bg,u8 fg)
{
	int color;
	if(bg == 7)color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	else if(bg == 6)color = BACKGROUND_BLUE | BACKGROUND_GREEN;
	else if(bg == 5)color = BACKGROUND_BLUE | BACKGROUND_RED;
	else if(bg == 4)color = BACKGROUND_BLUE;
	else if(bg == 3)color = BACKGROUND_GREEN | BACKGROUND_RED;
	else if(bg == 2)color = BACKGROUND_GREEN;
	else if(bg == 1)color = BACKGROUND_RED;
	else color = 0;

	//
	SetConsoleTextAttribute(output, color | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}




void windowwrite()
{
	int x,y;
	u8* p;
	u8 ch,bg=0,fg=0;
	COORD pos = {0,0};
	SetConsoleCursorPosition(output,pos);
/*
	//
	content[width*height]=0;
	printf("%s",content);
*/
	//
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = content + ((width*y + x)<<2);
			if(p[0] > 0x80)
			{
				//先颜色
				if(bg != p[7])
				{
					bg = p[7];
					attr(bg,fg);
				}

				//这是汉字
				windowsutf8(p);
				x++;
			}
			else
			{
				//先颜色
				if(bg != p[3])
				{
					bg = p[3];
					attr(bg,fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				printf("%c",ch);
			}
		}
	}
	if(bg != 0)attr(0,0);

	SetConsoleCursorPosition(output,pos);
}
void windowread(char* where)
{
}




void windowlist()
{
}
void windowchange()
{
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

	output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(output, &bInfo );
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;

	thread = CreateThread(NULL, 0, uievent, NULL, 0, NULL);
}
void windowdelete()
{
}
