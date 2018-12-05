#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include "libuser.h"




void joystick_event(struct arena* win, void* p)
{
	struct event ev;
	if(0 == win->orel0)
	{
		eventwrite(*(u64*)(p+0), joy_left, 0, 0);
		eventwrite(*(u64*)(p+8), joy_right, 0, 0);
	}
	else
	{
		ev.where = (u64)win;

		ev.why = *(u64*)(p+0);
		ev.what = joy_left;
		nodetree_rootwrite(win, 0, &ev, 0);

		ev.why = *(u64*)(p+8);
		ev.what = joy_right;
		nodetree_rootwrite(win, 0, &ev, 0);
	}
}
void joystickthread(struct arena* win)
{
	u8 af,gf;
	int av[3];
	int gv[3];
	int fd;
	int ret;
	char* str;
	struct js_event ev;
	struct xyzwpair pair;

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
			joystick_event(struct arena* win, &pair);
			usleep(1000*10);
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
					//printf("ac=(%d,%d,%d)\n",av[0],av[1],av[2]);
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
					//printf("gy=(%d,%d,%d)\n",gv[0],gv[1],gv[2]);
					gf = 0;
				}
			}
			else if(0 == ev.number)
			{
				printf("lx=%d\n", ev.value);
				pair.x0 = ev.value;
			}
			else if(1 == ev.number)
			{
				printf("ly=%d\n", -ev.value);
				pair.y0 = ev.value;
			}
			else if(2 == ev.number)
			{
				printf("rx=%d\n", ev.value);
				pair.x1 = ev.value;
			}
			else if(3 == ev.number)
			{
				printf("lt=%d\n", ev.value+32767);
				pair.z0 = ev.value+32767;
			}
			else if(4 == ev.number)
			{
				printf("rt=%d\n", ev.value+32767);
				pair.z1 = ev.value+32767;
			}
			else if(5 == ev.number)
			{
				printf("ry=%d\n", -ev.value);
				pair.y1 = -ev.value;
			}
			else if(9 == ev.number)
			{
				printf("lk=%d\n", ev.value);
			}
			else if(10== ev.number)
			{
				printf("rk=%d\n", -ev.value);
			}
			else printf("axis%d=%d\n", ev.number, ev.value);
		}
		else if(ev.type & 1)
		{
			if(0 == ev.number)
			{
				str = "x";
				pair.nn |= joyr_left;
			}
			else if(1 == ev.number)
			{
				str = "a";
				pair.nn |= joyr_down;
			}
			else if(2 == ev.number)
			{
				str = "b";
				pair.nn |= joyr_right;
			}
			else if(3 == ev.number)
			{
				str = "y";
				pair.nn |= joyr_up;
			}
			else if(4 == ev.number)
			{
				str = "lb";
				pair.id |= joyl_bumper;
			}
			else if(5 == ev.number)
			{
				str = "rb";
				pair.nn |= joyr_bumper;
			}
			else if(6 == ev.number)
			{
				str = "lt";
				pair.id |= joyl_trigger;
			}
			else if(7 == ev.number)
			{
				str = "rt";
				pair.nn |= joyr_trigger;
			}
			else if(8 == ev.number)
			{
				str = "share";
				pair.id |= joyl_select;
			}
			else if(9 == ev.number)
			{
				str = "option";
				pair.nn |= joyr_start;
			}
			else if(10== ev.number)
			{
				str = "ls";
				pair.id |= joyl_stick;
			}
			else if(11== ev.number)
			{
				str = "rs";
				pair.nn |= joyr_stick;
			}
			else if(12== ev.number)str = "!";
			else str = "?";
			printf("%s=%x\n", str, ev.value);
		}
		else printf("init\n");
	}

	close(fd);
}
void joycreate(struct arena* win)
{
	threadcreate(joystickthread, win);
}




void initjoy()
{
}
void freejoy()
{
}
