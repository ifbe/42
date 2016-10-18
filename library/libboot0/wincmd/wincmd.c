#include<stdio.h>
#include<stdarg.h>
#include<windows.h>




//
static HANDLE output=0;
static void attr(int val)
{
	if(output == 0)output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(output, val);
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




//
char last[8]={0};
char buf[0x1000];
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
	j = snprintf(buf,10,"%s",last);
	ret = vsnprintf(buf+j, 0x1000, fmt, args);
	va_end(args);

	for(j=0;j<ret;j++)
	{
		//printf("[%x]",buf[j]);continue;

		if(buf[j] == 0x1b)
		{
			for(k=0;k<5;k++)
			{
				if(buf[j+k] == 0)break;
			}
			if(k<5)
			{
				for(k=0;k<5;k++)last[j+k] = buf[j+k];
				break;
			}

			if(buf[j+1] == 0x5b)
			{
				//????
				if(buf[j+2] == 0x4b)
				{
					j += 2;
				}

				//reset
				else if( (buf[j+2] == '0') && (buf[j+3] == 'm') )
				{
					attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
					j += 3;
				}

				//reset
				else if( (buf[j+2] == '1') && (buf[j+3] == 'm') )
				{
					attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
					j += 3;
				}

				//foreground
				else if( (buf[j+2] == '3') && (buf[j+4] == 'm') )
				{
					//say("[%02x,%02x,%02x,%02x]",buf[j+0],buf[j+1],buf[j+2],buf[j+3]);
					if(buf[j+3] == '0')
					{
						attr(0);
					}
					else if(buf[j+3] == '1')
					{
						attr(FOREGROUND_RED);
					}
					else if(buf[j+3] == '2')
					{
						attr(FOREGROUND_GREEN);
					}
					else if(buf[j+3] == '3')
					{
						attr(FOREGROUND_GREEN|FOREGROUND_RED);
					}
					else if(buf[j+3] == '4')
					{
						attr(FOREGROUND_BLUE);
					}
					else if(buf[j+3] == '5')
					{
						attr(FOREGROUND_BLUE|FOREGROUND_RED);
					}
					else if(buf[j+3] == '6')
					{
						attr(FOREGROUND_BLUE|FOREGROUND_GREEN);
					}
					else if(buf[j+3] == '7')
					{
						attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
					}
					j += 4;
				}

				//background
				else if( (buf[j+2] == '4') && (buf[j+4] == 'm') )
				{
					if(buf[j+3] == '0')
					{
						attr(0);
					}
					else if(buf[j+3] == '1')
					{
						attr(BACKGROUND_RED);
					}
					else if(buf[j+3] == '2')
					{
						attr(BACKGROUND_GREEN);
					}
					else if(buf[j+3] == '3')
					{
						attr(BACKGROUND_GREEN|BACKGROUND_RED);
					}
					else if(buf[j+3] == '4')
					{
						attr(BACKGROUND_BLUE);
					}
					else if(buf[j+3] == '5')
					{
						attr(BACKGROUND_BLUE|BACKGROUND_RED);
					}
					else if(buf[j+3] == '6')
					{
						attr(BACKGROUND_BLUE|BACKGROUND_GREEN);
					}
					else if(buf[j+3] == '7')
					{
						attr(BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
					}
					j += 4;
				}
			}//5b
		}//1b
		else if( (buf[j] == 0x8) | (buf[j] == 0x7f) )
		{
			printf("\b \b");
		}
		else
		{
			printf("%c",buf[j]);
		}
	}
}