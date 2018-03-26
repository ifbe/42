#include "arena.h"
#define _TCP_ hex32('T','C','P',0)
#define _WS_ hex32('W','S',0,0)
#define _fd_ hex32('f','d',0,0)
void* relation_write(void*,void*,u64,void*,void*,u64);
void* arenacreate(u64, void*);
void* systemcreate(u64, void*);
int http_help(void*,void*,void*,int);




int httpclient_write(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin,
	u8* buf, int len)
{
	if(0 == win)return 0;
	if(0 == act)return 0;
	return 0;
}
int httpclient_read()
{
	return 0;
}
int httpclient_delete(struct arena* win)
{
	return 0;
}
int httpclient_create(struct arena* win)
{
	return 0;
}




int httpserver_write(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin,
	u8* buf, int len)
{
	int ret;
	void* addr;
	struct relation* rel;
	if(0 == win)return 0;
	if(0 == act)return 0;

	rel = win->orel;
	while(1)
	{
		if(0 == rel)break;
		if(_act_ == rel->dsttype)
		{
			actorwrite(
				(void*)(rel->dstchip), (void*)(rel->dstfoot),
				(void*)(rel->srcchip), (void*)(rel->srcfoot),
				buf, len
			);
		}
		rel = samesrcnextdst(rel);
	}

	ret = http_help(act,pin,buf,len);
	if(_WS_ == ret)
	{
		addr = arenacreate(_WS_, act);
		if(0 == addr)return 0;

		relation_write(addr, 0, _win_, act, 0, _fd_);
		//wsserver_write(addr, 0, act, 0, buf, len);
	}
	return 0;
}
int httpserver_read()
{
	return 0;
}
int httpserver_delete(struct arena* win)
{
	return 0;
}
int httpserver_create(struct arena* win, void* str)
{
	void* tmp;
	if(0 == str)str = "127.0.0.1:2222";

	tmp = systemcreate(_TCP_, str);
	if(tmp == 0)return 0;

	relation_write(win, 0, _win_, tmp, 0, _fd_);
	return 0;
}