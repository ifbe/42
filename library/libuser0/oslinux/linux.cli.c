#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include "libuser.h"
int lowlevel_input();




void windowread(struct arena* win)
{
	eventwrite(lowlevel_input(), _char_, 0, 0);
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
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void freewindow()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
	t.c_lflag |= ICANON|ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
