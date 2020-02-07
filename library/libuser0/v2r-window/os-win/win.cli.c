#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "libuser.h"




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




void windowread(struct supply* win)
{
}
void windowwrite(struct supply* win)
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
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	w->type = _wnd_;
	w->fmt = _cli_;

	w->width = w->stride = 80;
	w->height = 25;

	return;

	CreateProcess(
		"c:\\windows\\system32\\cmd.exe",
		0, 0, 0, 0,
		CREATE_NEW_CONSOLE, 0, 0,
		&si, &pi
	);
	printf("GetLastError=%d\n",GetLastError());
	termcount++;
}




void initwindow()
{
}
void freewindow()
{
}
