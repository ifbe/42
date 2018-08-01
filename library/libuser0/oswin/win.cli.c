#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "libuser.h"
int lowlevel_input();




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




void windowread(struct arena* win)
{
	eventwrite(lowlevel_input(), _char_, 0, 0);
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
void windowstart()
{
}
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('c','l','i',0);
	w->buf = 0;
	w->len = 0;

	w->width = w->stride = 80;
	w->height = 25;

	if(termcount != 0)
	{
		CreateProcess(
			"c:\\windows\\system32\\cmd.exe",
			0, 0, 0, 0,
			CREATE_NEW_CONSOLE, 0, 0,
			&si, &pi
		);
		printf("GetLastError=%d\n",GetLastError());
	}
	termcount++;
}




void initwindow()
{
}
void freewindow()
{
}
