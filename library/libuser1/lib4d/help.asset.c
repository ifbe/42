#include "actor.h"
void initutf8(void*);
//
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);




static u8* utf8data = 0;
static u16* bgmdata = 0;




void asset_create()
{
	int j,k;
	if(0 == utf8data)
	{
		utf8data = memorycreate(0x200000);
		if(0 != utf8data)
		{
			j = openreadclose("unicode.raw", 0, utf8data, 0x200000);
			if(j < 0x200000)say("error@unicode\n");

			for(j=0;j<0x200000;j++)
			{
				k = utf8data[j];
				k = ((k&0x0f)<<4) | ((k&0xf0)>>4);
				k = ((k&0x33)<<2) | ((k&0xcc)>>2);
				k = ((k&0x55)<<1) | ((k&0xaa)>>1);
				utf8data[j] = k;
			}
			initutf8(utf8data);
		}
	}
}
void asset_delete()
{
}
