#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include"libuser.h"




void windowread(struct supply* win)
{
}
void windowwrite()
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstart()
{
}
void windowstop()
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
