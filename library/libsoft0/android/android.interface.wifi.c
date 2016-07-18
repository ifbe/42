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




int systemwifi_read()
{
	return 1;
}
int systemwifi_write()
{
	return 1;
}




int systemwifi_list(char* towhere)
{
	int ret=system("ifconfig|grep wlan -A 1");
	return ret;
}
int systemwifi_choose()
{
}




void systemwifi_start(char* p)
{
}
void systemwifi_stop()
{
}




void systemwifi_init()
{
}
void systemwifi_kill()
{
}
