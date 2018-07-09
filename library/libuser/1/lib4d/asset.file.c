#include "libuser.h"
void* getsuffix(void* p);
int ncmp(void*, void*, int);
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);
void initutf8(void*);
//
void actorcreatefromobj(struct actor* act, u8* buf, int len);
void actorcreatefromstl(struct actor* act, u8* buf, int len);
void actorcreatefromjpg(struct actor* act, u8* buf, int len);
void actorcreatefrompng(struct actor* act, u8* buf, int len);




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








void actorcreatefromfile(struct actor* act, char* name)
{
	int j,len;
	u8* tmp;
	u8* buf;
	if(0 == act)return;

	tmp = getsuffix(name);
	if(0 == tmp)return;

	buf = memorycreate(0x800000);
	len = openreadclose(name, 0, buf, 0x800000);
	if(len <= 0)
	{
		say("len=%d\n", len);
		return;
	}

	if(0 == tmp[0])return;
	else if(0 == ncmp(tmp, "jpg", 3))actorcreatefromjpg(act, buf, len);
	else if(0 == ncmp(tmp, "png", 3))actorcreatefrompng(act, buf, len);
	else if(0 == ncmp(tmp, "obj", 3))actorcreatefromobj(act, buf, len);
	else if(0 == ncmp(tmp, "stl", 3))actorcreatefromstl(act, buf, len);
}