#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include "arena.h"
int lowlevel_input();




//
static int mode = 0;
static u64 thread;




void* terminalthread(void* win)
{
	u64 why, what, where;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		where = (u64)win;
		eventwrite(why, what, where, 0);
	}
}




void windowchange(int what)
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	mode = what;

	if(mode == 0)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
		t.c_lflag |= ICANON|ECHO;
	}
	else if(mode == 1)
	{
		fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
		t.c_lflag &= ~(ICANON|ECHO);
		t.c_cc[VTIME] = 0;
		t.c_cc[VMIN] = 1;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void windowlist()
{
}
void windowread(char* where)
{
}
void windowwrite()
{
}
void windowstop()
{
}
void windowstart(struct arena* this)
{
	this->type = hex32('w','i','n',0);
	this->fmt = hex32('c','l','i',0);

	this->buf = 0;
	this->len = 0;

	this->width = this->stride = 80;
	this->height = 25;

	thread = startthread(terminalthread, this);
}
void windowcreate()
{
}
void windowdelete()
{
}




void initwindow()
{
	windowchange(1);
}
void freewindow()
{
	windowchange(0);
}
