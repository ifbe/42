#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include"libuser.h"




static int alive = 1;
void windowsignal(int arg)
{
	alive = arg;
}
void windowthread()
{
	u64 why,what;
	what = _char_;
	while(alive)
	{
		why = lowlevel_input();
		eventwrite(why, what, 0, 0);
	}
}




void windowchange(int what)
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
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
