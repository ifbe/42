#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "arena.h"
//
u64 startthread(void*, void*);
void stopthread();
//
int lowlevel_input();
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




//
static HANDLE output;
static int lastwidth=0,lastheight=0;
static int width,height;




DWORD WINAPI terminalthread(void* win)
{
	u64 why, what, where;
	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			eventwrite(width + (height<<16), 0x657a6973, 0, 0);
		}

		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		where = (u64)win;
		eventwrite(why, what, where, 0);
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




void windowwrite(struct window* dst, struct window* src)
{
	int x,y;
	u8 ch,bg=0,fg=0;
	COORD pos = {0,0};
	u8* p;
	u8* buf = (u8*)(src->buf);
	SetConsoleCursorPosition(output,pos);
/*
	//
	buf[width*height]=0;
	printf("%s",buf);
*/
	//
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = buf + ((width*y + x)<<2);
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
void windowread()
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstart(struct window* this)
{
	if(this->type == hex32('b','u','f',0))
	{
		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');
		return;
	}
	else
	{
		this->type = hex32('w','i','n',0);
		this->fmt = hex32('t','u','i',0);

		this->w = width;
		this->h = height;
		this->d = 0;

		this->thread = startthread(terminalthread, this);
	}
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
}
void windowdelete()
{
}
