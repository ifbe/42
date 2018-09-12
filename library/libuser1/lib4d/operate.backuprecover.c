#include "libuser.h"




void scene_backup(u8* buf, int len)
{
	if(0 == len)
	{
		while(1)
		{
			if(buf[len] < 0x20)break;
			len++;
		}
	}
	say("%.*s\n", len, buf);
}
void scene_recover(u8* buf, int len)
{
	if(0 == len)
	{
		while(1)
		{
			if(buf[len] < 0x20)break;
			len++;
		}
	}
	say("%.*s\n", len, buf);
}