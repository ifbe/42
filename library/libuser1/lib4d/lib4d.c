#include "libuser.h"
void content_create(void*);
void bgfg_create(void*);
void helpout_create(void*);
void overview_create(void*);
//
void* getsuffix(void* p);
void initutf8(void*);
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);
//
void actorcreatefromobj(struct actor* act, u8* buf, int len);
void actorcreatefromstl(struct actor* act, u8* buf, int len);
void actorcreatefromjpg(struct actor* act, u8* buf, int len);
void actorcreatefrompng(struct actor* act, u8* buf, int len);
void actorcreatefrommp3(struct actor* act, u8* buf, int len);
void actorcreatefromogg(struct actor* act, u8* buf, int len);
void actorcreatefromwav(struct actor* act, u8* buf, int len);




static u8* utf8data = 0;
static u16* bgmdata = 0;




void asset_create(void* addr)
{
	int j,k;
	if(0 == utf8data)
	{
		utf8data = memorycreate(0x200000);
		if(0 != utf8data)
		{
			j = openreadclose("datafile/unicode.raw", 0, utf8data, 0x200000);
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
	u8 str[256];
	if(0 == act)return;

	tmp = getsuffix(name);
	if(0 == tmp)return;

	mysnprintf(str, 256, "%s/%s", "datafile/", name);

	buf = memorycreate(0x800000);
	len = openreadclose(str, 0, buf, 0x800000);
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
	else if(0 == ncmp(tmp, "mp3", 3))actorcreatefrommp3(act, buf, len);
	else if(0 == ncmp(tmp, "ogg", 3))actorcreatefromogg(act, buf, len);
	else if(0 == ncmp(tmp, "wav", 3))actorcreatefromwav(act, buf, len);
}
void* alloc_winobj(struct arena* win)
{
	int j;
	struct datapair* mod = win->mod;
	for(j=16;j<128;j++)
	{
		if((0 == mod[j].src.vs)&&(0 == mod[j].src.vbuf))return &mod[j].src;
	}
	return 0;
}




void lib4d_create(void* addr)
{
	asset_create(addr);
	bgfg_create(addr);
	content_create(addr);

	overview_create(addr);
}
void lib4d_delete()
{
}
