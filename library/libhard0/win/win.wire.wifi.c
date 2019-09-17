#include<stdio.h>
#include<stdlib.h>

int wifi_list(int stage)
{
	if(stage == 0)
	{
		system("netsh wlan show interfaces");
	}
	else if(stage == 1)
	{
		system("netsh wlan show networks");
	}
}
int wifi_choose()
{
}
int wifi_read()
{
}
int wifi_write()
{
}
int wifi_start()
{
}
int wifi_stop()
{
}
int wifi_create()
{
}
int wifi_delete()
{
}

