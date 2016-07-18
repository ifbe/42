#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
void say(char*,...);




//
static int fp=-1;




int systemusb_read()
{
	return 1;
}
int systemusb_write()
{
	return 1;
}




int systemusb_list(char* towhere)
{
	int ret=system("lsusb");
	return ret;
}
int systemusb_choose()
{
}




void systemusb_start(char* p)
{
}
void systemusb_stop()
{
}




void systemusb_init()
{
}
void systemusb_kill()
{
}
