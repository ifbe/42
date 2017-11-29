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
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
u64 startthread(void*, void*);
void stopthread();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




struct uartinfo
{
        char* buf;
        int len;
        int enq;
        int deq;
};
static struct uartinfo info;
static int fd = -1;
static u64 thread;
static int alive = 1;




void* systemuart_thread(void* p)
{
	int ret,max;
	while(alive == 1)
	{
		max = 0x100000 - (info.enq);
		if(max > 256)max = 256;

		ret = read(
			fd,
			(info.buf)+(info.enq),
			256
		);
		if(ret < 0)break;
		if(ret == 0)continue;

		info.enq = (info.enq+ret)%0x100000;
		eventwrite(
			(u64)&info,
			hex32('u','a','r','t'),
			0,
			0
		);
	}
	return 0;
}




int systemuart_read(char* buf, int len)
{
	return 0;
}
int systemuart_write(char* buf, int len)
{
	int ret;
	ret = write(fd, buf, len);
	return ret;
}
int systemuart_list(char* p)
{
	system("ls /dev/tty*");
	return 0;
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

	if(info.buf == 0)
	{
		info.enq = 0;
		info.deq = 0;
		info.len = 0x100000;
		info.buf = (char*)malloc(info.len);
	}

	//thread
	alive = 1;
	thread = startthread(systemuart_thread, 0);

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
