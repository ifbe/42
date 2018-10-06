#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <linux/joystick.h>
#include "libuser.h"
int lowlevel_input();




void joystickthread()
{
	int fd;
	int ret;
	struct js_event ev;

	fd = open("/dev/input/js0", O_RDONLY);
	if(fd <= 0){printf("error@open\n");return;}

	while(1)
	{
		ret = read(fd, &ev, sizeof(ev));
		if(ret <= 0)continue;

		printf("time=%-8x,value=%-8x,type=%-8x,number=%-8x\n",
			ev.time, ev.value, ev.type, ev.number);
	}

	close(fd);
}
void terminalthread(void* win)
{
	while(1)
	{
		eventwrite(lowlevel_input(), _char_, 0, 0);
	}
}
void traycreate(struct arena* win)
{
	threadcreate(terminalthread, 0);
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
