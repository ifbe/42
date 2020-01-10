#include "libuser.h"
int windowread(int type, void* buf);
u64 file_check(void*, int);




static u8 fsbuf[0x1000];
static u8* buffer;
void fs_prep(void* name)
{
	int ret;
	u64 type;

	ret = openreadclose(name, 0, fsbuf, 0x1000);
	if(ret <= 0)return;
	fsbuf[ret] = 0;

	type = file_check(fsbuf, ret);
	say("type=%.8s\n", &type);
}




static void fs_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, fsbuf, 0x1000);
}
static void fs_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fs_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fs_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"fs\" style=\"width:50%%;height:100px;float:left;background-color:#465097;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void fs_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fs_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void fs_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	if(_drag_ == type)
	{
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		fs_prep(buffer);
	}
}




static void fs_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void fs_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void fs_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void fs_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void fs_search(struct entity* act)
{
}
static void fs_modify(struct entity* act)
{
}
static void fs_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void fs_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = fsbuf;
	if(_copy_ == act->type)act->buf = memorycreate(0x1000, 0);
}




void fs_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('f', 's', 0, 0);

	p->oncreate = (void*)fs_create;
	p->ondelete = (void*)fs_delete;
	p->onsearch = (void*)fs_search;
	p->onmodify = (void*)fs_modify;

	p->onlinkup = (void*)fs_linkup;
	p->ondiscon = (void*)fs_discon;
	p->onread  = (void*)fs_read;
	p->onwrite = (void*)fs_write;
}
