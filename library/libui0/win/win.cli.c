#include<stdio.h>
#include<windows.h>
#define u64 unsigned long long




//
static int mode = 0;
static char buffer[256];




int uievent(u64* who, u64* what, u64* key)
{
	int i;
	char* ret;

	if(mode == 0)
	{
		while(1)
		{
			ret=fgets(buffer, 256, stdin);
			//say("uievent.ret=%x\n",ret);

			if(ret == NULL)return 0;
			if(buffer[0] != 0)return 1;
		}
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
