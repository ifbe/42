#include "actor.h"
#define _UDP_ hex32('U','D','P',0)
#define _fd_ hex32('f','d',0,0)
void* systemcreate(u64, void*);
void* relation_write(void*,void*,u64,void*,void*,u64);




static void rawdump_read_vbo(
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
static void rawdump_read_pixel(
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
static void rawdump_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rawdump_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rawdump_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rawdump_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rawdump_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)rawdump_read_tui(win, sty, act, pin);
	else if(fmt == _html_)rawdump_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)rawdump_read_vbo(win, sty, act, pin);
	else rawdump_read_pixel(win, sty, act, pin);
}




void queuepacket(u8* dst, int max, u8* buf, int len)
{
	int j,k;
	int* pp = (int*)dst;
	u8* bb = dst+0x1000;

	j = pp[0];
	if(j > 0)
	{
		
	}
	else
	{
		pp[0] = 1;
		pp[1] = len;
		k = 0;
	}

	for(j=0;j<len;j++)
	{
		bb[k+j] = buf[j];
	}
}
static void rawdump_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;
	struct actor* tmp;

	if(0 != win)
	{
		say("@rawdump_write\n");
/*
		queuepacket(
			act->buf, 0x400,
			act->buf+0x1000, 0xff000,
			tmp->buf, tmp->len
		);
*/
		if((act->len)+len > 0x100000)
		{
			dst = act->buf;
			act->len = len;
		}
		else
		{
			dst = (act->buf)+(act->len);
			act->len += len;
		}

		for(j=0;j<len;j++)dst[j] = buf[j];
		dst[j] = 0;
	}
}
static void rawdump_stop(struct actor* act, struct pinid* pin)
{
}
static void rawdump_start(struct actor* act, struct pinid* pin)
{
}
static void rawdump_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)
	{
		stopmemory(act->buf);
	}
	act->buf = 0;
}
static void rawdump_create(struct actor* act, u8* buf)
{
	void* addr;
	if(0 == act)return;
	else if(_orig_ == act->type)act->buf = startmemory(0x100000);

	addr = systemcreate(_UDP_, "127.0.0.1:2222");
	if(0 == addr)return;

	relation_write(act, 0, _act_, addr, 0, _fd_);
}
static void rawdump_list(u8* buf)
{
}
static void rawdump_choose(u8* buf)
{
}




void rawdump_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r','a','w','d','u','m','p',0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)rawdump_create;
	p->ondelete = (void*)rawdump_delete;
	p->onstart  = (void*)rawdump_start;
	p->onstop   = (void*)rawdump_stop;
	p->onlist   = (void*)rawdump_list;
	p->onchoose = (void*)rawdump_choose;
	p->onread   = (void*)rawdump_read;
	p->onwrite  = (void*)rawdump_write;
}
