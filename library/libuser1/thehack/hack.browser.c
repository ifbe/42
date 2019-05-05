#include "libuser.h"
void printhtmlbody(u8* buf, int len);




static void browser_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
	struct str* str = act->idx;
	struct str* dat = act->buf;
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
static void browser_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc,tr,tf,tu;
	struct str* str = act->idx;
	struct str* dat = act->buf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
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
static void browser_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void browser_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}
static void browser_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)browser_read_cli(win, sty, act, pin);
	else if(_tui_ == fmt)browser_read_tui(win, sty, act, pin);
	else if(_html_ == fmt)browser_read_html(win, sty, act, pin);
	else if(_json_ == fmt)browser_read_json(win, sty, act, pin);
	else if(_vbo_ == fmt)browser_read_vbo(win, sty, act, pin);
	else browser_read_pixel(win, sty, act, pin);
}




static void browser_write_event(
	struct actor* act, struct pinid* pin,
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
static void browser_write_data(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
static void browser_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	void* buf, int len)
{
	if(0 == win)browser_write_event(act, pin, win, sty, buf);
	else browser_write_data(act, pin, win, sty, buf, len);
}
static void browser_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void browser_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void browser_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void browser_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
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
	p->onstart  = (void*)browser_start;
	p->onstop   = (void*)browser_stop;
	p->oncread  = (void*)browser_cread;
	p->oncwrite = (void*)browser_cwrite;
	p->onsread  = (void*)browser_sread;
	p->onswrite = (void*)browser_swrite;
}
