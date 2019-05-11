#include "libuser.h"




void windowread(struct arena* win)
{
	//draw frame
	actorread_all(win);
}
void windowwrite()
{
}
void windowchange()
{
}
void windowlist()
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
}




void initwindow()
{
}
void freewindow()
{
}
