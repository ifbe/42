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
	u8 af,gf;
	int av[3];
	int gv[3];
	int fd;
	int ret;
	char* str;
	struct js_event ev;

	//wait until joystick up
	while(1)
	{
		fd = open("/dev/input/js0", O_RDONLY);
		if(fd <= 0)
		{
			usleep(1000*1000);
			continue;
		}
		break;
	}

	//keep reading
	af = 0;
	gf = 0;
	while(1)
	{
		ret = read(fd, &ev, sizeof(ev));
		if(ret <= 0)
		{
			usleep(1000*100);
			continue;
		}

		if(ev.type & 2)
		{
			if(     (6 == ev.number) |
				(7 == ev.number) |
				(8 == ev.number) )	//accel
			{
				ret = ev.number-6;
				av[ret] = ev.value;

				af |= (1 << ret);
				if(7 == af)
				{
					printf("ac=(%d,%d,%d)\n",av[0],av[1],av[2]);
					af = 0;
				}
			}
			else if((11== ev.number) |
				(12== ev.number) |
				(13== ev.number) )	//gyro
			{
				ret = ev.number-11;
				gv[ret] = ev.value;

				gf |= (1 << ret);
				if(7 == gf)
				{
					printf("gy=(%d,%d,%d)\n",gv[0],gv[1],gv[2]);
					gf = 0;
				}
			}
			else if(0 == ev.number)printf("lx=%d\n", ev.value);
			else if(1 == ev.number)printf("ly=%d\n", -ev.value);
			else if(2 == ev.number)printf("rx=%d\n", ev.value);
			else if(3 == ev.number)printf("lt=%d\n", ev.value+32767);
			else if(4 == ev.number)printf("rt=%d\n", ev.value+32767);
			else if(5 == ev.number)printf("ry=%d\n", -ev.value);
			else if(9 == ev.number)printf("lk=%d\n", ev.value);
			else if(10== ev.number)printf("rk=%d\n", -ev.value);
			else printf("axis%d=%d\n", ev.number, ev.value);
		}
		else if(ev.type & 1)
		{
			if(0 == ev.number)str = "x";
			else if(1 == ev.number)str = "a";
			else if(2 == ev.number)str = "b";
			else if(3 == ev.number)str = "y";
			else if(4 == ev.number)str = "lb";
			else if(5 == ev.number)str = "rb";
			else if(6 == ev.number)str = "lt";
			else if(7 == ev.number)str = "rt";
			else if(8 == ev.number)str = "share";
			else if(9 == ev.number)str = "option";
			else if(10== ev.number)str = "ls";
			else if(11== ev.number)str = "rs";
			else if(12== ev.number)str = "!";
			else str = "?";
			printf("%s=%x\n", str, ev.value);
		}
		else printf("init\n");
	}

	close(fd);
}
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
void termcreate(struct arena* win)
{
	threadcreate(joystickthread, win);
	threadcreate(terminalthread, win);
}




void initterm()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void freetterm()
{
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & (~O_NONBLOCK));
	t.c_lflag |= ICANON|ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
