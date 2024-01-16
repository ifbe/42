#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
void logtoall(char*,...);




//
static int fp=-1;




int usb_read()
{
	return 1;
}
int usb_write()
{
	return 1;
}




int usb_list(char* towhere)
{
	int ret = system("lsusb");
	return ret;
}
int usb_choose()
{
	return 0;
}




void usb_linkup(char* p)
{
}
void usb_discon()
{
}




void usb_create()
{
}
void usb_delete()
{
}
