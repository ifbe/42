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




int wifi_read()
{
	return 1;
}
int wifi_write()
{
	return 1;
}




int wifi_list(char* towhere)
{
	int ret = system("ifconfig|grep wlan -A 1");
	return ret;
}
int wifi_choose()
{
	return 0;
}




void wifi_linkup(char* p)
{
}
void wifi_discon()
{
}




void wifi_create()
{
}
void wifi_delete()
{
}
