#include "libuser.h"
int openreadclose(void*, int, void*, int);
void printhtmlbody(u8* buf, int len);




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
	int j,k,ret;
	struct arena* win;
	u8* mem;
	u8 tmp[256];

	ret = 0;
	while(1)
	{
		if(buf[ret] < 0x20)break;
		tmp[ret] = buf[ret];
		ret++;
	}
	tmp[ret] = 0;
	say("name: %.*s\n", ret, tmp);

	win = arenacreate(_html_, 0);
	if(0 == win)return;
	say("win: %llx\n", win);

	mem = memorycreate(0x100000);
	if(0 == mem)return;
	say("mem: %llx\n", mem);

	win->buf = mem;
	ret = openreadclose(tmp, 0, mem, 0x100000);
	if(0 >= ret)return;
	say("read:%d\n", ret);

	printhtmlbody(mem, ret);
}
