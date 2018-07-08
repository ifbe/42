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
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('c','l','i',0);

	w->width = w->stride = 80;
	w->height = 25;

	w->buf = 0;
	w->len = 0;
}




void initwindow()
{
}
void freewindow()
{
}
