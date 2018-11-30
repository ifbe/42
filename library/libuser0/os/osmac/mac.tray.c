#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libuser.h"
int lowlevel_input();




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




void terminalthread(void* win)
{
	inittray();
	while(1)
	{
		eventwrite(lowlevel_input(), _char_, 0, 0);
	}
	freetray();
}
void traycreate(struct arena* win)
{
	threadcreate(terminalthread, 0);
}
