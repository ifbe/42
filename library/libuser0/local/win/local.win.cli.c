#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include "arena.h"
int lowlevel_input();




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




void joyprint(int id, XINPUT_GAMEPAD g)
{
	int j;
	if(	(0 == g.wButtons) &&
		(8 >= g.bLeftTrigger) &&
		(8 >= g.bRightTrigger) &&
		(4096 >= g.sThumbLX) &&
		(4096 >= g.sThumbLY) &&
		(4096 >= g.sThumbRX) &&
		(4096 >= g.sThumbRY) )
	{
		return;
	}

	printf(
		"%x:\n"
		"	%x,%x\n"
		"	%d,%d\n"
		"	%d,%d\n",
		id,
		g.bLeftTrigger, g.bRightTrigger,
		g.sThumbLX, g.sThumbLY,
		g.sThumbRX, g.sThumbRY
	);
	for(j=0;j<16;j++)
	{
		if(g.wButtons & (1<<j))
		{
			printf("	%s\n", xname[j]);
		}
	}
}
void* joystickthread(void* win)
{
	XINPUT_STATE s;

	while(1)
	{
		if(ERROR_SUCCESS == XInputGetState(0, &s))
		{
			joyprint(0, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(1, &s))
		{
			joyprint(1, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(2, &s))
		{
			joyprint(2, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(3, &s))
		{
			joyprint(3, s.Gamepad);
		}
	}
}
void* terminalthread(void* win)
{
	u64 why, what;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		eventwrite(why, what, 0, 0);
	}
}




void windowread()
{
}
void windowwrite(struct arena* win)
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstop()
{
}
void windowstart(struct arena* this)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex32('c','l','i',0);
	this->buf = 0;
	this->len = 0;

	this->width = this->stride = 80;
	this->height = 25;

	if(termcount == 0)
	{
		u64 thread = threadcreate(terminalthread, this);
		termcount++;
	}
	else
	{
		CreateProcess(
			"c:\\windows\\system32\\cmd.exe",
			0, 0, 0, 0,
			CREATE_NEW_CONSOLE, 0, 0,
			&si, &pi
		);
		printf("GetLastError=%d\n",GetLastError());
		termcount++;
	}
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
{
}
void freewindow()
{
}
