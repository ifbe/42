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




int bt_read()
{
	return 1;
}
int bt_write()
{
	return 1;
}




int bt_list(char* p)
{
	char buf[0x80];
	if(p == 0)snprintf(buf, 0x80, "hcitool scan");
	else snprintf(buf, 0x80, "sdptool browse %s", p);

	printf("%s\n",buf);
	return system(buf);
}
int bt_choose()
{
	return 0;
}




int bt_linkup(char* p)
{
	return 0;
}
int bt_discon()
{
	return 0;
}




void bt_create()
{
}
void bt_delete()
{
}
