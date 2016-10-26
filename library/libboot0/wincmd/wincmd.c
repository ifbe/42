#include<stdio.h>
#include<stdarg.h>
#include<windows.h>




//
static HANDLE output=0;
//
char last[8]={0};
char buf[0x1000];
//
void attr(int val)
{
	if(output == 0)output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(output, val);
}
void escapecolor(char* p)
{
	//reset
	if(p[0] == '0')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return;
	}

	//heavy
	else if(p[0] == '1')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
		return;
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
		return;
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
		return;
	}
}
int escapesequence(char* p)
{
	//????: 1b 5b 4b
	if(p[2] == 0x4b)
	{
		printf(" \b");
		return 2;
	}

	//right: 1b 5b 43
	else if(p[2] == 0x43)
	{
		return 2;
	}

	//1b 5b ? m
	else if(p[3] == 'm')
	{
		escapecolor(p+2);
		return 3;
	}

	//1b 5b ? ? m
	else if(p[4] == 'm')
	{
		escapecolor(p+2);
		return 4;
	}

	//1b 5b ? ; ? ? m
	else if( (p[3] == ';') && (p[6] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+4);
		return 6;
	}

	//1b 5b ? ? ; ? ? m
	else if( (p[4] == ';') && (p[7] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+5);
		return 7;
	}

	return 0;
}




void createserial(char* arg)
{
	//opened?
	HWND consolewindow = GetConsoleWindow();
	if(consolewindow != NULL)return;

	//new
	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);

	//hide
	consolewindow = GetConsoleWindow();
	ShowWindow(consolewindow,SW_HIDE);
}
void deleteserial()
{
}
int diary(char* mem, int max, char* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say(char* fmt , ...)
{
	int j,k,ret;
	va_list args;

	va_start(args,fmt);
	if(last[0] != 0)
	{
		j = snprintf(buf,10,"%s",last);
		ret = j + vsnprintf(buf+j, 0x1000, fmt, args);
		last[0] = 0;
	}
	else
	{
		ret = vsnprintf(buf, 0x1000, fmt, args);
	}
	va_end(args);

	for(j=0;j<ret;j++)
	{
		//printf("{%x}",buf[j]);

		if(buf[j] == 0x1b)
		{
			//escape sequence must complete
			for(k=1;k<8;k++)
			{
				if(buf[j+k] == 0)
				{
					for(;k>=0;k--)last[k] = buf[j+k];
					return;
				}
			}
			//printf("{%x,%x,%x,%x,%x}",buf[j+0],buf[j+1],buf[j+2],buf[j+3],buf[j+4]);

			if(buf[j+1] == 0x5b)
			{
				j += escapesequence(buf+j);
			}//5b
		}//1b
		else if(buf[j] == 0x8)
		{
			printf("\b");
		}
		else if(buf[j] == 0x7f)
		{
			printf("\b \b");
		}
		else
		{
			printf("%c",buf[j]);
		}
	}
}