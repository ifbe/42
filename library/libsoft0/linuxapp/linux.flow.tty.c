#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//#include <termios.h>
#include <sys/ioctl.h>
#include <asm-generic/termbits.h>
#include "libhard.h"
int epoll_add(int);
int listfolder(char*, int, char*, char*);




static struct item* obj;
void inituart(void* addr)
{
	obj = addr;
}
void freeuart()
{
}
void uartalloc()
{
}




int uart_insert()
{
	return 0;
}
int uart_remove()
{
	return 0;
}
int uart_search(char* p)
{
	int ret = system("ls /dev/tty*");
	return 0;
}
int uart_modify(char* p, int speed)
{
	return 0;
}




_obj* uart_create(char* p, int speed)
{
	int fd = open(p , O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd <= 0)
	{
		logtoall("error:%d@open:%s\n",errno,p);
		return 0;
	}

	//obj
	struct item* oo = &obj[fd];
	oo->sockinfo.fd = fd;
	logtoall("fd=%d,obj=%p\n", fd, oo);

	struct termios2 options;
	ioctl(fd, TCGETS2, &options);
	options.c_cflag &= ~CBAUD;	//Remove current BAUD rate
	options.c_cflag |= BOTHER;	//Allow custom BAUD rate using int input
	options.c_ispeed = speed;	//Set the input BAUD rate
	options.c_ospeed = speed;	//Set the output BAUD rate
	ioctl(fd, TCSETS2, &options);
/*
	//get
	struct termios option;
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
*/
	//dev[fd].type = _uart_;
	//dev[fd].buf = (void*)malloc(0x100000);
	epoll_add(fd);

	return oo;
}
int uart_delete(_obj* oo)
{
	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	close(fd);
	return 0;
}
int uart_reader(_obj* oo,int xx, p64 arg,int off, void* buf,int len)
{
	int fd = oo->sockinfo.fd;
	//logtoall("fd=%d,obj=%p\n", fd, oo);

	int ret;
	ret = read(fd, buf, len);
	return ret;
}
int uart_writer(_obj* oo,int xx, p64 arg,int off, void* buf,int len)
{
	int fd = oo->sockinfo.fd;
	//logtoall("fd=%d,obj=%p\n", fd, oo);

	int ret = write(fd, buf, len);
	if(ret != len){
		printf("err@write:%d,%d\n", ret, errno);
	}
	return ret;
}




int uart_attach()
{
	return 0;
}
int uart_detach()
{
	return 0;
}
int uart_takeby(_obj* oo,int xx, p64 arg,int off, void* buf,int len)
{
	return 0;
}
int uart_giveby(_obj* oo,int xx, p64 arg,int off, void* buf,int len)
{
	return 0;
}