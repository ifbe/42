#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
u64 startthread(void*, void*);
void stopthread();
//
u64 gettime();
void say(char*,...);




//
static int alive = 1;
static u64 thread;
//
static int fd = -1;




void* systemuart_read(void* p)
{
	int ret;
	char buf[256];
	while(alive == 1)
	{
		ret = read(fd, buf, 256);
		if(ret < 0)break;

		buf[ret] = 0;
		printf("%s", buf);
		fflush(stdout);
	}
	return 0;
}
int systemuart_write(char* buf, int len)
{
	int ret;
	ret = write(fd, buf, len);
	return ret;
}




int systemuart_list()
{
	return system("ls /dev/tty*");
}
int systemuart_choose(char* p, int speed)
{
	struct termios option;

	if(p == 0)
	{
		if(fd>0)
		{
			alive = 0;

			close(fd);
			fd = -1;
		}
		return 0;
	}

	fd=open(p , O_RDWR|O_NOCTTY|O_NDELAY);
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

	//thread
	alive = 1;
	thread = startthread(systemuart_read, 0);

	//success
	return 1;
}




int systemuart_stop()
{
	return 0;
}
int systemuart_start(char* p)
{
	return 0;
}
void systemuart_create()
{
}
void systemuart_delete()
{
}
