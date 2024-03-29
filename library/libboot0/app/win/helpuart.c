#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))




//
static HANDLE hStdin;
static HANDLE hStdout;
//
static u8 last[16] = {0};
static int pos = 0;
int arg2utf8(u8* src, u8* dst)
{
	int j,k;
	u32 ret,tmp;

	j = k = 0;
	while(1)
	{
		if(src[j] < 0xa)break;
		if(src[j] < 0x80)
		{
			dst[k] = src[j];
			k++;
			j++;
			continue;
		}

		ret = *(u16*)(src+j);
		j += 2;

		tmp = 0;
		MultiByteToWideChar(
			CP_ACP, 0,
			(void*)&ret, -1,
			(void*)&tmp, 2
		);
		//printf("%x\n", tmp);

		ret = 0;
		WideCharToMultiByte(
			CP_UTF8, 0,
			(void*)&tmp, -1,
			(void*)&ret, 4,
			NULL, NULL
		);
		//printf("%x\n", ret);

		*(u32*)(dst+k) = ret;
		for(ret=0;ret<4;ret++)
		{
			if(dst[k] >= 0xa)k++;
		}
	}

	dst[k] = 0;
	return k;
}




static void attr(int val)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, val);
}
static int escapecolor(u8* p)
{
	//reset
	if(p == 0)
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return 0;
	}

	//reset
	if(p[0] == '0')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(FOREGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(FOREGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(FOREGROUND_GREEN|FOREGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(FOREGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		}
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(BACKGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(BACKGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(BACKGROUND_GREEN|BACKGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(BACKGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
		}
		return 4;
	}
}
static void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &bInfo);

	pos.X = x;
	pos.Y = bInfo.srWindow.Top + y;
	SetConsoleCursorPosition(hStdout, pos);
}
static void clearcmd()
{
	DWORD count;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &bInfo);

	COORD pos;
	pos.X = bInfo.srWindow.Left;
	pos.Y = bInfo.srWindow.Top;
	FillConsoleOutputCharacter(
		hStdout,
		(TCHAR)' ',
		bInfo.dwSize.X * bInfo.dwSize.Y,
		pos,
		&count
	);
}
static int escapeposition(u8* p)
{
	int j, k, x=0, y=0;
	for(k=0;k<4;k++)
	{
		if(p[k] == ';')break;
	}
	if(k>=4)return 0;

	for(j=0;j<k;j++)
	{
		y = (y*10) + p[j] - 0x30;
	}

	for(;k<8;k++)
	{
		if((p[k] == 'H')|(p[k] == 'f'))break;
	}
	if(k>=8)return 0;

	j++;
	for(;j<k;j++)
	{
		x = (x*10) + p[j] - 0x30;
	}

	gotoxy(x-1, y-1);
	return 0;
}
static int escapesequence(u8* p)
{
	int j;
	int x,y;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;
/*
	if(p[2] == '?')
	{
		for(j=3;j<8;j++)
		{
			if( (p[j] == 'h') | (p[j] == 'l') )
			{
				gotoxy(0,0);
				for(y=0;y<128;y++)
				{
					for(x=0;x<79;x++)
					{
						printf(" ");
					}
					printf("\b\n");
				}
				gotoxy(0,0);

				return j+1;
			}
		}
	}
*/
	//1b 5b 4a: Clear screen from cursor down
	if(p[2] == 'J')
	{
		clearcmd();
		return 3;
	}

	//1b 5b 4b: Clear line from cursor right
	if(p[2] == 'K')
	{
		//printf("        \b\b\b\b\b\b\b\b");
		return 3;
	}

	//1b 5b m
	if(p[2] == 'm')
	{
		escapecolor(0);
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		escapecolor(p+2);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		escapecolor(p+2);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+5);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if(p[j] == 'r')return j+1;
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			escapeposition(p+2);
			return j+1;
		}
	}

	return 0;
}




void lowlevel_output(u8* buf, int len)
{
	int i,j,k=0;
	if(pos != 0)
	{
		if(len < 8)
		{
			for(i=0;i<len;i++)last[pos+i] = buf[i];
		}
		else
		{
			for(i=0;i<8;i++)last[i] = buf[i];
			buf += 8;
			len -= 8;
		}

		j=0;
		while(1)
		{
			if(j>=pos)
			{
				for(j=0;j<pos;j++)last[pos] = 0;
				pos = 0;
				return;
			}

			if(buf[j] != 0x1b)
			{
				printf("%c", buf[j]);
				j++;
				continue;
			}

			i = escapesequence(buf+j);
			if(i != 0)
			{
				//printf("?");
				j += i;
				continue;
			}

			if(pos-j < 8)
			{
				for(i=0;i<pos-j;i++)last[i] = last[j+i];
				for(i=j;i<pos;i++)last[i] = 0;
				pos = j;
				return;
			}
		}
	}

	for(j=0;j<len;j++)
	{
		if(buf[j] >= 0xc0)
		{
			int ret;
			char utf8[8];
			char unicode[8];
			char gbk[8];
			if(j>k)printf("%.*s", j-k, buf+k);

			if(buf[j] < 0xe0)i = 2;
			else if(buf[j] < 0xf0)i = 3;
			else if(buf[j] < 0xf8)i = 4;
			else if(buf[j] < 0xfc)i = 5;
			else if(buf[j] < 0xfe)i = 6;
			k = j+i;

			utf8[i] = 0;
			for(i=i-1;i>=0;i--)utf8[i]=buf[j+i];
			j = k-1;

			ret = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (void*)unicode, 4);
			ret = WideCharToMultiByte(CP_ACP, 0, (void*)unicode, -1, gbk, 4, NULL, NULL);
			printf("%s",gbk);
		}
		else if(buf[j] == 0x1b)
		{
			if(j+7 >= len)
			{
				for(i=0;i<len-j;i++)last[pos+i] = buf[j+i];
				pos += len-j;
				break;
			}
			if(j>k)printf("%.*s", j-k, buf+k);

			i = escapesequence(buf+j);
			if(i != 0)
			{
				k = j+i;
				j = k-1;
			}
		}
	}
	if(j>k)printf("%.*s", j-k, buf+k);
}
int lowlevel_input()
{
	DWORD num;
	int ret,tmp;
	INPUT_RECORD irInBuf[2];
	KEY_EVENT_RECORD k0, k1;

	while(1)
	{
		if(!ReadConsoleInput(hStdin, irInBuf, 1, &num))
		{
			printf("ReadConsoleInput");
			return 0;
		}
		if(KEY_EVENT != irInBuf[0].EventType)continue;
//printmemory(&irInBuf[0], sizeof(INPUT_RECORD));
//printmemory(&irInBuf[1], sizeof(INPUT_RECORD));

		k0 = irInBuf[0].Event.KeyEvent;
		if(0 == k0.bKeyDown)continue;
//printf("%x,%x,%x,%x\n", k0.wVirtualKeyCode, k0.uChar.UnicodeChar, k1.wVirtualKeyCode, k1.uChar.UnicodeChar);

		if(k0.uChar.AsciiChar == 0)
		{
			ret = k0.wVirtualKeyCode;

			if(ret == 0x26)return 0x415b1b;
			else if(ret == 0x28)return 0x425b1b;
			else if(ret == 0x27)return 0x435b1b;
			else if(ret == 0x25)return 0x445b1b;
			else
			{
				//printf("kbd:%x\n", ret);
				continue;
			}
		}
		else if(k0.uChar.UnicodeChar < 0x80)
		{
			if( (4 == k0.uChar.UnicodeChar) && (k0.dwControlKeyState&8) )return 0;
			return k0.uChar.UnicodeChar;
		}
		else
		{
			if(!ReadConsoleInput(hStdin, &irInBuf[1], 1, &num)){
				printf("read unicode error\n");
				return 0;
			}
			k1 = irInBuf[1].Event.KeyEvent;
			if(0x10 == k0.wVirtualKeyCode)return k1.uChar.UnicodeChar;

			ret = ret | (k1.uChar.UnicodeChar << 8);
			//printf("%x\n", ret);

			tmp = 0;
			MultiByteToWideChar(
				CP_ACP, 0,
				(void*)&ret, -1,
				(void*)&tmp, 2
			);
			//printf("%x\n", tmp);

			ret = 0;
			WideCharToMultiByte(
				CP_UTF8, 0,
				(void*)&tmp, -1,
				(void*)&ret, 4,
				NULL, NULL
			);
			//printf("%x\n", ret);

			return ret;
		}
	}
	return 0;
}




void freeserial()
{
}
void initserial()
{
	//opened?
	HWND consolewindow = GetConsoleWindow();
	if(NULL == consolewindow)
	{
		//new
		AllocConsole();
		freopen("CONIN$","r",stdin);
		freopen("CONOUT$","w",stdout);

		//hide
		consolewindow = GetConsoleWindow();
		ShowWindow(consolewindow,SW_SHOW);
	}

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hStdin)
	{
		printf("error@hStdin\n");
		exit(-1);
	}

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hStdout)
	{
		printf("error@hStdout\n");
		exit(-1);
	}
}
