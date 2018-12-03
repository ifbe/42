#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libuser.h"
int lowlevel_input();




void terminalthread(struct arena* win)
{
	int ret;
	while(1)
	{
		ret = lowlevel_input();
		if(0 == win->orel0){
			eventwrite(ret, _char_, 0, 0);
		}
		else {
			say("%x\n", ret);
		}
	}
}
void traycreate(struct arena* win)
{
	//threadcreate(joystickthread, win);
	threadcreate(terminalthread, win);
}




void inittray()
{
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);

        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
        t.c_lflag &= ~(ICANON|ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void freetray()
{
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);

        fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
        t.c_lflag |= ICANON|ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
}