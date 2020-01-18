#include "libuser.h"
#define _UDP_ hex32('U','D','P',0)




void queuepacket(u8* dst, int* idx, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)dst[j] = buf[j];
}




static void rawdump_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
static void rawdump_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rawdump_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rawdump_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rawdump_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void rawdump_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void rawdump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void rawdump_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void rawdump_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void rawdump_search(struct entity* act, u8* buf)
{
}
static void rawdump_modify(struct entity* act, u8* buf)
{
}
static void rawdump_delete(struct entity* act, u8* buf)
{
	if(0 == act)return;
	if(0 != act->buf)memorydelete(act->buf);
	act->buf = 0;
}
static void rawdump_create(struct entity* act, u8* buf)
{
	if(0 == act)return;
	act->idx = memorycreate(0x10000, 0);
	act->buf = memorycreate(0x100000, 0);
}




void rawdump_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','a','w','d','u','m','p',0);

	p->oncreate = (void*)rawdump_create;
	p->ondelete = (void*)rawdump_delete;
	p->onsearch = (void*)rawdump_search;
	p->onmodify = (void*)rawdump_modify;

	p->onlinkup = (void*)rawdump_linkup;
	p->ondiscon = (void*)rawdump_discon;
	p->onread  = (void*)rawdump_read;
	p->onwrite = (void*)rawdump_write;
}
