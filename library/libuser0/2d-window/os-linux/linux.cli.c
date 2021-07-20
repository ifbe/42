#include "libuser.h"




void window_take(_obj* win)
{
}
void window_give()
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
