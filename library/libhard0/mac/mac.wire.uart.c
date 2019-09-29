#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libsoft.h"
int kqueue_add(int);




static struct object* obj;




int uart_read(int fd, int off, void* buf, int len)
{
	int ret;
	ret = read(fd, buf, len);
	return ret;
}
int uart_write(int fd, int off, void* buf, int len)
{
	int ret;
	ret = write(fd, buf, len);
	return ret;
}
int uart_list(char* buf, int len)
{
	return 0;
}
int uart_choose()
{
	return 0;
}
int uart_stop()
{
	return 0;
}
int uart_start(char* p, int speed)
{
	struct termios option;
	int fd = open(p , O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd <= 0)
	{
		say("error:%d@open:%s\n",errno,p);
		return -1;
	}

	//get
	tcgetattr(fd, &option);
	fcntl(fd, F_SETFL, 0);

	switch(speed){
		case 9600:  speed = B9600;  break;
		case 19200: speed = B19200; break;
		case 38400: speed = B38400; break;
		case 57600: speed = B57600; break;
		case 115200:speed = B115200;break;
		case 230400:speed = B230400;break;
		case 460800:speed = B460800;break;
		case 921600:speed = B921600;break;
		//case 250000:speed = B250000;break;
		default:return 0;
	}

	//8n1
	option.c_cflag = speed | CS8 | CLOCAL | CREAD;
	option.c_iflag = IGNPAR;
	option.c_oflag = 0;
	option.c_lflag = 0;

	//set
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &option);

	//obj[fd].type = hex32('u','a','r','t');
	//obj[fd].buf = (void*)malloc(0x100000);
	kqueue_add(fd);

	return fd;
}




void freeuart()
{
}
void inituart(void* addr)
{
	obj = addr;
}
