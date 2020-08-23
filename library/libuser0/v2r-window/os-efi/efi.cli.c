#include "libuser.h"
int lowlevel_input();




void windowread()
{
	int ret = lowlevel_input();
	say("ret=%d\n",ret);
}
void windowwrite()
{
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd)
{
	wnd->fmt = _cli_;
	wnd->vfmt = 0;

	wnd->width = 80;
	wnd->height = 25;

	wnd->fbwidth = 80;
	wnd->fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
