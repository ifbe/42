#include "libuser.h"
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
//
int websocket_read(void*, int, void*, int);
int websocket_write(void*, int, void*, int);




static void* buffer = 0;




/*
int wsserver_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	return 0;
}
int wsserver_write(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin,
	u8* buf, int len)
{
	int readlen, headlen, ret;
	u8 headbuf[16];
	if(0 == win)return 0;
	if(0 == act)return 0;
	say("%.4s,%.4s\n",win,act);

	//received message
	readlen = websocket_read(buf, len, buffer, 0x100000);
	say("@wsserver_write:%.*s\n", readlen, buffer);

	//get current
	actorread_all(win);

	//send message
	headlen = websocket_write(win->buf, win->len, headbuf, 16);
	ret = systemwrite(act, pin, win, sty, headbuf, headlen);
	ret = systemwrite(act, pin, win, sty, win->buf, win->len);
	return 0;
}
int wsserver_delete(struct arena* win)
{
	//unlink
	//delete
	return 0;
}
int wsserver_create(struct arena* win)
{
	if(0 == buffer)buffer = memorycreate(0x100000);

	win->buf = buffer;
	return 0;
}
*/




int wsnode_leafwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int wsnode_leafread(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int wsnode_rootwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	void* dc;
	void* df;
	struct relation* orel = win->orel0;
	if(0 == orel)
	{
		say("@wsnode_write: %.*s\n", len, buf);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_act_ == orel->dsttype)
		{
			actor_rootwrite(dc, df, win, 0, buf, len);
		}

		orel = samesrcnextdst(orel);
	}
	return 0;
}
int wsnode_rootread(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int wsnode_delete(struct arena* win)
{
	return 0;
}
int wsnode_create(struct arena* win, void* str)
{
	void* art;
	if(str)
	{
		art = arterycreate(0, str);
		if(art)relationcreate(win, 0, _win_, art, 0, _art_);
	}
	return 0;
}