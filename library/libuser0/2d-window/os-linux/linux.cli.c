#include "libuser.h"




void window_take(_obj* wnd)
{
}
void window_give(_obj* wnd)
{
}




void windowdelete(_obj* wnd)
{
}
void windowcreate(_obj* wnd)
{
	wnd->hfmt = _cli_;
	wnd->vfmt = 0;

	wnd->whdf.width = 80;
	wnd->whdf.height = 25;

	wnd->whdf.fbwidth = 80;
	//wnd->whdf.fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
