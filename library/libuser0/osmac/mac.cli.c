#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include"libuser.h"




void windowread(struct arena* win)
{
	//draw frame
	actorread_all(win);

	//cleanup events(window event)
	sleep_us(1000*1000);
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
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = _win_;
	w->fmt = _cli_;

	w->len = 0;
	w->buf = 0;

	w->width = w->stride = 80;
	w->height = 25;
}




void initwindow()
{
}
void freewindow()
{
}
