#include "actor.h"
void initutf8(void*);
void* startmemory(int);




static u16* utf8data = 0;
static u16* bgmdata = 0;




void asset_create()
{
	int j;
	utf8data = startmemory(0x200000);
	if(utf8data != 0)
	{
		j = readfile("datafile/unicode.raw", utf8data, 0, 0x200000);
		if(j <= 0x200000)say("error@readfile\n");

		for(j=0;j<0x100000;j++)
		{
			utf8data[j] = (utf8data[j]>>8) + ((utf8data[j]<<8)&0xff00);
		}
		initutf8(utf8data);
	}
}
void asset_delete()
{
	
}