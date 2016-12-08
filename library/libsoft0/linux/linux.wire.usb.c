#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
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
	return 0;
}




void systemusb_start(char* p)
{
}
void systemusb_stop()
{
}




void systemusb_create()
{
}
void systemusb_delete()
{
}
