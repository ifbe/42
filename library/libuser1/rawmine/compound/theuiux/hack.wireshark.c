#include "libuser.h"
#define _UDP_ hex32('U','D','P',0)
#define _fd_ hex32('f','d',0,0)
void* systemcreate(u64, void*);




void queuepacket(u8* dst, int* idx, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)dst[j] = buf[j];
}




static void rawdump_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int j;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
static void rawdump_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rawdump_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rawdump_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"rawdump\" style=\"width:50%%;height:100px;float:left;background-color:#39c8a7;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void rawdump_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rawdump_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rawdump_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rawdump_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)rawdump_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)rawdump_draw_html(act, pin, win, sty);
	else if(fmt == _json_)rawdump_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)rawdump_draw_vbo(act, pin, win, sty);
	else rawdump_draw_pixel(act, pin, win, sty);
}




static void rawdump_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//rawdump_draw(act, pin, win, sty);
}
static void rawdump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//if(len)queuepacket(act->buf, act->idx, buf, len);
}
static void rawdump_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rawdump_start(struct halfrel* self, struct halfrel* peer)
{
}




static void rawdump_search(struct actor* act, u8* buf)
{
}
static void rawdump_modify(struct actor* act, u8* buf)
{
}
static void rawdump_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)memorydelete(act->buf);
	act->buf = 0;
}
static void rawdump_create(struct actor* act, u8* buf)
{
	void* addr;
	if(0 == act)return;
	act->idx = memorycreate(0x10000, 0);
	act->buf = memorycreate(0x100000, 0);

	addr = systemcreate(_UDP_, "127.0.0.1:2222");
	if(0 == addr)return;

	relationcreate(act, 0, _act_, 0, addr, 0, _fd_, 0);
}




void rawdump_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','a','w','d','u','m','p',0);

	p->oncreate = (void*)rawdump_create;
	p->ondelete = (void*)rawdump_delete;
	p->onsearch = (void*)rawdump_search;
	p->onmodify = (void*)rawdump_modify;

	p->onstart = (void*)rawdump_start;
	p->onstop  = (void*)rawdump_stop;
	p->onread  = (void*)rawdump_read;
	p->onwrite = (void*)rawdump_write;
}
