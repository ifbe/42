#include<stdio.h>
#include<conio.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int show();
int command(void*);
void say(char*,...);




//
static int mode = 0;
static char buffer[256];




int uievent(u64* who, u64* what, u64* where)
{
	int i;
	char* ret;

	if(mode == 0)
	{
/*
		while(1)
		{
			ret=fgets(buffer, 256, stdin);
			if(ret == NULL)return 0;
			if(buffer[0] != 0)
			{
				for(i=0;i<255;i++)
				{
					if(buffer[i]==0xa)
					{
						buffer[i]=0xd;
						break;
					}
				}
				//
				*what = 0x64626b;
				*where = (u64)buffer;
				return 1;
			}
		}
*/
		for(i=0;i<255;i++)
		{
			buffer[i] = getch();
			//printf("%x\n",buffer[i]);
			if( (buffer[i] == 0x3) | (buffer[i] == 9) | (buffer[i] == 0xd) | (buffer[i] == 0x1a) )
			{
				buffer[i+1] = 0;
				break;
			}
		}
		//
		*what = 0x64626b;
		*where = (u64)buffer;
		return 1;
	}
	else if(mode == 1)
	{
		while(1)
		{
			i = getchar();
			printf("%x\n",i);
		}
		return 0;
	}
}




void terminal_passthrough()
{
}
void terminal_normalize()
{
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
}
