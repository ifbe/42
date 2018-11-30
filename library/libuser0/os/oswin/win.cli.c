#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "libuser.h"




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




void windowread(struct arena* win)
{
	//draw frame
	actorread_all(win);

	//cleanup events(window event)
	sleep_us(1000*1000);
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
	w->fmt = _cli_;
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
