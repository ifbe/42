#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include "libuser.h"




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
