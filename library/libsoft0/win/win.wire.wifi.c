#include<stdio.h>
#include<stdlib.h>

int systemwifi_list(int stage)
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
int systemwifi_choose()
{
}
int systemwifi_read()
{
}
int systemwifi_write()
{
}
int systemwifi_start()
{
}
int systemwifi_stop()
{
}
int systemwifi_create()
{
}
int systemwifi_delete()
{
}

