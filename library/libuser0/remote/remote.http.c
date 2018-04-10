#include "arena.h"
#define _TCP_ hex32('T','C','P',0)
#define _WS_ hex32('W','S',0,0)
#define _fd_ hex32('f','d',0,0)
void* arenacreate(u64, void*);
int arenadelete(void*);
void* systemcreate(u64, void*);
int systemdelete(void*);
//
int websocket_write_handshake(u8* buf, int len);
int websocket_read_handshake(u8* buf, int len, u8* dst, int max);




static u8* buffer = 0;




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
	int j,k,ret;
	void* addr;
	struct relation* rel;
	u8* GET = 0;
	u8* POST = 0;
	u8* Upgrade = 0;
	u8* Connection = 0;
	if(0 == win)return 0;
	if(0 == act)return 0;

	//parse
	k = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		say("%.*s\n", j-k, buf+k);
		if(ncmp(buf+k, "Connection: ", 12) == 0)Connection = buf+k+12;
		else if(ncmp(buf+k, "Upgrade: ", 9) == 0)Upgrade = buf+k+9;
		else if(ncmp(buf+k, "POST ", 5) == 0)POST = buf+k+5;
		else if(ncmp(buf+k, "GET ", 4) == 0)GET = buf+k+4;

		if(0xa == buf[j+1])j++;
		k = j+1;
	}

	//ws request
	if((0 != Connection)&&(0 != Upgrade))
	{
		ret = websocket_read_handshake(buf, len, buffer, 0x1000);
		ret = systemwrite(act, pin, win, sty, buffer, ret);

		addr = arenacreate(_WS_, act);
		if(0 == addr)return 0;

		relationcreate(addr, 0, _win_, act, 0, _fd_);
		relationcreate(act, 0, _fd_, addr, 0, _win_);
		return 0;
	}
	else if(0 != GET)
	{
		win->buf = buffer;
		win->fmt = hex32('h','t','m','l');

		actorread(win,0,0,0);
		if(0 == win->len)win->len = mysnprintf(win->buf, 9, "empty");
		ret = mysnprintf(
			(win->buf)+(win->len),0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			win->len
		);

		systemwrite(act, pin, win, sty, (win->buf)+(win->len), ret);
		systemwrite(act, pin, win, sty, win->buf, win->len);
		if(0 != Connection)
		{
			if(0 == ncmp(Connection, "keep-alive", 10))return 0;
		}
	}
	else if(0 != POST)
	{
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
	if(0 == buffer)buffer = memorycreate(0x100000);
	if(0 == str)str = "127.0.0.1:2222";

	tmp = systemcreate(_TCP_, str);
	if(tmp == 0)return 0;

	relationcreate(win, 0, _win_, tmp, 0, _fd_);
	return 0;
}