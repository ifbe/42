#include "libuser.h"
#define STRBUF buf0
#define DATBUF buf1
void printhtmlbody(u8* buf, int len);




static void browser_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
	struct str* str = act->STRBUF;
	struct str* dat = act->DATBUF;
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
static void browser_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	vec3 tc,tr,tf,tu;
	struct str* str = act->STRBUF;
	struct str* dat = act->DATBUF;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	gl41solid_rect(win, 0xffffff, vc, vr, vf);

	//address
	tc[0] = vc[0] + vf[0]*0.95 + vu[0]*0.001;
	tc[1] = vc[1] + vf[1]*0.95 + vu[1]*0.001;
	tc[2] = vc[2] + vf[2]*0.95 + vu[2]*0.001;
	tf[0] = vf[0]*0.02;
	tf[1] = vf[1]*0.02;
	tf[2] = vf[2]*0.02;
	gl41solid_rect(win, 0xc0c0c0, tc, vr, tf);

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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void browser_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void browser_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void browser_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}




static void browser_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev)
{
	int len;
	u8* buf;
	void* www;
	struct str* str;
	struct str* dat;
	if(_char_ != ev->what)return;

	str = act->STRBUF;
	len = str->len;
	buf = str->buf;
	if(0xd == ev->why)
	{
		str->len = 0;
/*
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
			relationcreate(act, 0, _ent_, 0, www, 0, _art_, 0);
		}*/
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
	struct entity* act, struct style* pin,
	u8* buf, int len)
{
	int j,cnt;
	u8* tmp;
	struct str* dat;

	dat = act->buf0;
	cnt = dat->len;
	tmp = dat->buf;

	for(j=0;j<len;j++)tmp[cnt+j] = buf[j];
	dat->len += len;

	printhtmlbody(dat->buf, dat->len);
}




static void browser_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void browser_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void browser_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void browser_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void browser_search(struct entity* act)
{
}
static void browser_modify(struct entity* act)
{
}
static void browser_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->STRBUF){
		memorydelete(act->STRBUF);
		act->STRBUF = 0;
	}
	if(act->DATBUF){
		memorydelete(act->DATBUF);
		act->DATBUF = 0;
	}
}
static void browser_create(struct entity* act)
{
	int j;
	u8* buf;
	if(0 == act)return;

	buf = act->STRBUF = memorycreate(0x1000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;

	buf = act->DATBUF = memorycreate(0x100000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;
}




void browser_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onsearch = (void*)browser_search;
	p->onmodify = (void*)browser_modify;

	p->onlinkup = (void*)browser_linkup;
	p->ondiscon = (void*)browser_discon;
	p->onread  = (void*)browser_read;
	p->onwrite = (void*)browser_write;
}
