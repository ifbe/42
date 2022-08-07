#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);




static STARTUPINFO si;
static PROCESS_INFORMATION pi;
static int termcount = 0;




void window_attach()
{
}
void window_detach()
{
}
void window_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg, int idx, short* buf, int len)
{
}
void window_give(_sup* wnd,void* foot, _syn* stack,int sp, void* arg, int idx, short* buf, int len)
{
}




void window_create(_obj* w)
{
	w->fmt = _cli_;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;
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
void window_delete(_obj* w)
{
}
void window_read()
{
}
void window_write()
{
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
