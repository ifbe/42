#include "actor.h"
#define _UDP_ hex32('U','D','P',0)
#define _udp_ hex32('u','d','p',0)
#define _tcp_ hex32('t','c','p',0)
#define _cam_ hex32('c','a','m',0)
#define _fd_ hex32('f','d',0,0)
int systemcreate(u64, void*);
void* systemread(int fd);
void* arenacreate(int, void*);
void systemwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
//
void* samesrcnextdst(void*);
void* relation_write(void*,void*,u64,void*,void*,u64);




static void switch_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
}
static void switch_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	int len = act->len;
	u8* buf = act->buf;
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1, buf, len);
}
static void switch_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void switch_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)switch_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)switch_read_tui(win, sty, act, pin);
	else if(fmt == _html_)switch_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)switch_read_vbo(win, sty, act, pin);
	else switch_read_pixel(win, sty, act, pin);
}




static void switch_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;
	if(0 != win)
	{
		dst = act->buf;
		for(j=0;j<len;j++)dst[j] = buf[j];
		dst[j] = 0;

		struct relation* orel = act->orel;
		while(1)
		{
			if(0 == orel)break;
			if(_fd_ == orel->dsttype)
			{
				systemwrite(
					(void*)(orel->dstchip), (void*)(orel->dstfoot),
					(void*)(orel->srcchip), (void*)(orel->srcfoot),
					act->buf, len
				);
			}
			orel = samesrcnextdst(orel);
		}
	}
}
static void switch_stop(struct actor* act, struct pinid* pin)
{
}
static void switch_start(struct actor* act, struct pinid* pin)
{
}
static void switch_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)
	{
		stopmemory(act->buf);
	}
	act->buf = 0;
}
static void switch_create(struct actor* act, u8* buf)
{
	int fd;
	void* addr;
	if(0 == act)return;
	else if(_orig_ == act->type)act->buf = startmemory(0x100000);

	//act => fd
	fd = systemcreate(_udp_, "127.0.0.1:2222");
	if(fd <= 0)return;

	addr = systemread(fd);
	relation_write(addr, 0, _fd_, act, 0, _act_);
}
static void switch_list(u8* buf)
{
}
static void switch_choose(u8* buf)
{
}




void switch_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s','w','i','t','c','h',0,0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)switch_create;
	p->ondelete = (void*)switch_delete;
	p->onstart  = (void*)switch_start;
	p->onstop   = (void*)switch_stop;
	p->onlist   = (void*)switch_list;
	p->onchoose = (void*)switch_choose;
	p->onread   = (void*)switch_read;
	p->onwrite  = (void*)switch_write;
}
