#include "libuser.h"
void printhtmlbody(u8* buf, int len);




static void browser_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
	struct str* str = act->idx;
	struct str* dat = act->buf;
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
	drawsolid_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh);

	//address
	x0 = cx-ww+32;
	y0 = cy-hh+8;
	x1 = cx+ww-32;
	y1 = cy-hh+24;
	drawsolid_rect(win, 0xc0c0c0, x0, y0-4, x1, y1+4);
	drawstring(win, 0x000000, x0, y0, str->buf, str->len);

	//context
	x0 = cx-ww;
	y0 = cy-hh+32;
	x1 = cx+ww;
	y1 = cy+hh;
	drawline_rect(win, 0x404040, x0, y0, x1, y1);
	drawtext(win, 0x000000, x0, y0, x1, y1, dat->buf, dat->len);
}
static void browser_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf,tu;
	struct str* str = act->idx;
	struct str* dat = act->buf;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid_rect(win, 0xffffff, vc, vr, vf);

	//address
	tc[0] = vc[0] + vf[0]*0.95 + vu[0]*0.001;
	tc[1] = vc[1] + vf[1]*0.95 + vu[1]*0.001;
	tc[2] = vc[2] + vf[2]*0.95 + vu[2]*0.001;
	tf[0] = vf[0]*0.02;
	tf[1] = vf[1]*0.02;
	tf[2] = vf[2]*0.02;
	carvesolid_rect(win, 0xc0c0c0, tc, vr, tf);

	tc[0] = vc[0] - vr[0] + vf[0]*0.95 + vu[0]*0.002;
	tc[1] = vc[1] - vr[1] + vf[1]*0.95 + vu[1]*0.002;
	tc[2] = vc[2] - vr[2] + vf[2]*0.95 + vu[2]*0.002;
	tr[0] = vr[0]/32;
	tr[1] = vr[1]/32;
	tr[2] = vr[2]/32;
	tf[0] = vf[0]*0.02;
	tf[1] = vf[1]*0.02;
	tf[2] = vf[2]*0.02;
	carvestring(win, 0x000000, tc, tr, tf, str->buf, str->len);
}
static void browser_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void browser_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void browser_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void browser_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}
static void browser_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)browser_draw_cli(act, pin, win, sty);
	else if(_tui_ == fmt)browser_draw_tui(act, pin, win, sty);
	else if(_html_ == fmt)browser_draw_html(act, pin, win, sty);
	else if(_json_ == fmt)browser_draw_json(act, pin, win, sty);
	else if(_vbo_ == fmt)browser_draw_vbo(act, pin, win, sty);
	else browser_draw_pixel(act, pin, win, sty);
}




static void browser_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev)
{
	int len;
	u8* buf;
	void* www;
	struct str* str;
	struct str* dat;
	if(_char_ != ev->what)return;

	str = act->idx;
	len = str->len;
	buf = str->buf;
	if(0xd == ev->why)
	{
		str->len = 0;

		if(0 == ncmp(buf, "file://", 7))
		{
			dat = act->buf;
			dat->len = openreadclose(str->buf+7, 0, dat->buf, 0xf0000);
			printhtmlbody(dat->buf, dat->len);
			return;
		}
		else
		{
			www = arterycreate(0, buf);
			relationcreate(act, 0, _act_, 0, www, 0, _art_, 0);
		}
	}
	else if(0x8 == ev->why)
	{
		if(len <= 0)
		{
			str->len = 0;
		}
		else
		{
			len--;
			buf[len] = 0;
			str->len = len;
		}
	}
	else
	{
		if(len < 0xfc)
		{
			buf[len] = ev->why;
			buf[len+1] = 0;
			str->len = len+1;
		}
	}
}
static void browser_data(
	struct actor* act, struct style* pin,
	u8* buf, int len)
{
	int j,cnt;
	u8* tmp;
	struct str* dat;

	dat = act->buf;
	cnt = dat->len;
	tmp = dat->buf;

	for(j=0;j<len;j++)tmp[cnt+j] = buf[j];
	dat->len += len;

	printhtmlbody(dat->buf, dat->len);
}




static void browser_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	browser_draw(act, pin, win, sty);
}
static void browser_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	if(len)browser_data(act, pin, buf, len);
	else browser_event(act, pin, win, sty, ev);
}
static void browser_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void browser_start(struct halfrel* self, struct halfrel* peer)
{
}




static void browser_search(struct actor* act)
{
}
static void browser_modify(struct actor* act)
{
}
static void browser_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void browser_create(struct actor* act)
{
	int j;
	u8* buf;
	if(0 == act)return;

	buf = memorycreate(0x1000);
	for(j=0;j<0x1000;j++)buf[j] = 0;
	act->idx = buf;

	buf = memorycreate(0x100000);
	for(j=0;j<0x1000;j++)buf[j] = 0;
	act->buf = buf;
}




void browser_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onsearch = (void*)browser_search;
	p->onmodify = (void*)browser_modify;

	p->onstart = (void*)browser_start;
	p->onstop  = (void*)browser_stop;
	p->onread  = (void*)browser_read;
	p->onwrite = (void*)browser_write;
}
