#include "libuser.h"




void windowread()
{
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
void windowdelete(_obj* w)
{
}
void windowcreate(_obj* w)
{
	w->fmt = _cli_;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
