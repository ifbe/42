#include "libuser.h"
#define _UDP_ hex32('U','D','P',0)
#define _fd_ hex32('f','d',0,0)
void* systemcreate(u64, void*);




static void rawdump_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
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
static void rawdump_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rawdump_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rawdump_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	else if(fmt == _json_)rawdump_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)rawdump_read_vbo(win, sty, act, pin);
	else rawdump_read_pixel(win, sty, act, pin);
}




void queuepacket(u8* dst, int* idx, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)dst[j] = buf[j];
}
static void rawdump_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j;
	u8* dst;
	struct actor* tmp;
	if(0 == win)return;

	//writefile();
	queuepacket(act->buf, act->idx, buf, len);
}
static void rawdump_get(u8* buf)
{
}
static void rawdump_post(u8* buf)
{
}
static void rawdump_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void rawdump_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	act->idx = memorycreate(0x10000);
	act->buf = memorycreate(0x100000);

	addr = systemcreate(_UDP_, "127.0.0.1:2222");
	if(0 == addr)return;

	relationcreate(act, 0, _act_, addr, 0, _fd_);
}




void rawdump_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r','a','w','d','u','m','p',0);

	p->oncreate = (void*)rawdump_create;
	p->ondelete = (void*)rawdump_delete;
	p->onstart  = (void*)rawdump_start;
	p->onstop   = (void*)rawdump_stop;
	p->onget    = (void*)rawdump_get;
	p->onpost   = (void*)rawdump_post;
	p->onread   = (void*)rawdump_read;
	p->onwrite  = (void*)rawdump_write;
}
