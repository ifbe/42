#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include"libuser.h"
int lowlevel_input();




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
	eventwrite(lowlevel_input(), _char_, 0, 0);
}
void windowwrite()
{
	//actorread_all(win);
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
