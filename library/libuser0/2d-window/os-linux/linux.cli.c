#include "libuser.h"




void window_take(struct supply* win)
{
}
void window_give()
{
}




void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
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
