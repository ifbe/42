#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include "libuser.h"
int lowlevel_input();
int lowlevel_output(void*,int);
int termwrite(void* buf, int len);




void terminalthread(struct supply* win)
{
	int ret;
	struct event ev;
	struct halfrel stack[0x80];
	while(1)
	{
		ret = lowlevel_input();
		if( (0 == win->orel0) && (0 == win->irel0) ){
			eventwrite(ret, _char_, 0, 0);
		}
		else {
			//say("%x\n", ret);
			ev.why = ret;
			ev.what = _char_;
			give_data_into_peer(win,_dst_, stack,0, 0,0, &ev,0);
		}
	}
}




void stdio_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void stdio_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	lowlevel_output(buf, len);
}
void stdio_delete(struct supply* win)
{
}
void stdio_create(struct supply* win)
{
	termwrite("\n", 1);
	threadcreate(terminalthread, win);
}




void freestd()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
	t.c_lflag |= ICANON|ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void initstd()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
