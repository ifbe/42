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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, fsbuf, 0x1000);
}
static void fs_draw_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void fs_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void fs_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void fs_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void fs_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)fs_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)fs_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)fs_draw_html(act, pin, win, sty);
	else if(fmt == _json_)fs_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)fs_draw_vbo(act, pin, win, sty);
	else fs_draw_pixel(act, pin, win, sty);
}
static void fs_event(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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




static void fs_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	fs_draw(act, pin, win, sty);
}
static void fs_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	fs_event(act, pin, win, sty, ev, 0);
}
static void fs_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void fs_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void fs_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void fs_start(struct halfrel* self, struct halfrel* peer)
{
}
static void fs_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void fs_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = fsbuf;
	if(_copy_ == act->type)act->buf = memorycreate(0x1000);
}




void fs_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('f', 's', 0, 0);

	p->oncreate = (void*)fs_create;
	p->ondelete = (void*)fs_delete;
	p->onstart  = (void*)fs_start;
	p->onstop   = (void*)fs_stop;
	p->oncread  = (void*)fs_cread;
	p->oncwrite = (void*)fs_cwrite;
	p->onsread  = (void*)fs_sread;
	p->onswrite = (void*)fs_swrite;
}
