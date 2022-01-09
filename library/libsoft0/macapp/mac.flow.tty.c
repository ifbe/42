#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "libsoft.h"
int kqueue_add(int);




static struct item* obj;




void inituart(void* addr)
{
	obj = addr;
}
void freeuart()
{
}




void* uart_create(char* path, int speed)
{
	//fd
	int fd = open(path , O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd <= 0){
		say("error:%d@open:%s\n", errno, path);
		return 0;
	}
	//else say("open: %s, %d\n", path, speed);

	//obj
	struct item* oo = &obj[fd];
	oo->sockinfo.fd = fd;
	say("fd=%d,obj=%p\n", fd, oo);

	//speed
	switch(speed){
		case 9600:  speed = B9600;  break;
		case 19200: speed = B19200; break;
		case 38400: speed = B38400; break;
		case 57600: speed = B57600; break;
		case 115200:speed = B115200;break;
		case 230400:speed = B230400;break;
		//case 460800:speed = B460800;break;
		//case 921600:speed = B921600;break;
		//case 250000:speed = B250000;break;
		default:return 0;
	}

	struct termios options;
	tcgetattr(fd, &options);
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
/*
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
		//case 460800:speed = B460800;break;
		//case 921600:speed = B921600;break;
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
	//obj[fd].type = hex32('u','a','r','t');
	//obj[fd].buf = (void*)malloc(0x100000);
	kqueue_add(fd);

	return oo;
}
int uart_delete(struct item* oo)
{
	int fd = oo->sockinfo.fd;
	close(fd);
	return 0;
}
int uart_search(char* buf, int len)
{
	return 0;
}
int uart_modify()
{
	return 0;
}




int uart_take(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	int fd = oo->sockinfo.fd;
	//say("fd=%d,obj=%p\n", fd, oo);

	int ret = read(fd, buf, len);
	if(ret != len){
		printf("err@read:%d,%d\n", ret, errno);
	}
	return ret;
}
int uart_give(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	int fd = oo->sockinfo.fd;
	//say("fd=%d,obj=%p\n", fd, oo);

	int ret = write(fd, buf, len);
	if(ret != len){
		printf("err@write:%d,%d\n", ret, errno);
	}
	return ret;
}