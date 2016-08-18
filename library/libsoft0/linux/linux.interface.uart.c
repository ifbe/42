#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
void say(char*,...);




//
static int fd=-1;




int systemuart_list()
{
	return system("ls /dev/tty*");
}
int systemuart_choose()
{
}




int systemuart_read()
{
}
int systemuart_write(char* p,int count)
{
	int ret;
	ret=write(fd,p,strlen(p));
	say("ret=%d\n",ret);
	return ret;
}




int systemuart_start(char* p)
{
	struct termios option;

	fd=open(p , O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd<=0)
	{
		say("error@systemuart_start.open\n");
		return -1;
	}

	//get
	tcgetattr(fd,&option);
	fcntl(fd,F_SETFL,0);

	//8n1
	option.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	option.c_iflag = IGNPAR;
	option.c_oflag = 0;
	option.c_lflag=0;

	//set
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&option);
}
int systemuart_stop()
{
	if(fd>0)
	{
		close(fd);
		fd=-1;
	}
}




void systemuart_create()
{
}
void systemuart_delete()
{
}
