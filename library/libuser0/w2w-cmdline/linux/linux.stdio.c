#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libuser.h"
int lowlevel_input();
int termwrite(void* buf, int len);




void terminalthread(struct arena* win)
{
	int ret;
	while(1)
	{
		ret = lowlevel_input();
		if( (0 == win->orel0) && (0 == win->irel0) ){
			eventwrite(ret, _char_, 0, 0);
		}
		else {
			//say("%x\n", ret);
			relationwrite(win, _dst_, 0, 0, &ret, 1);
		}
	}
}




void stddelete(struct arena* win)
{
}
void stdcreate(struct arena* win)
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
