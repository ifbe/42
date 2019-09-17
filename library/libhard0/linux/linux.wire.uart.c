#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libhard.h"
int epoll_add(int);
int listfolder(char*, int, char*, char*);




static struct device* dev;




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
	int ret = listfolder(buf, len, "/dev", "tty");
	if(ret < 0)ret = 0;
	return ret;
}
int uart_change()
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
	int fd=open(p , O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd<=0)
	{
		say("error:%d@open:%s\n",errno,p);
		return -1;
	}

	//get
	tcgetattr(fd,&option);
	fcntl(fd,F_SETFL,0);

	if(speed == 9600)speed = B9600;
	else speed = B115200;

	//8n1
	option.c_cflag = speed | CS8 | CLOCAL | CREAD;
	option.c_iflag = IGNPAR;
	option.c_oflag = 0;
	option.c_lflag=0;

	//set
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&option);

	dev[fd].type = _uart_;
	dev[fd].buf = (void*)malloc(0x100000);
	epoll_add(fd);

	return fd;
}




void freeuart()
{
}
void inituart(void* addr)
{
	dev = addr;
}
