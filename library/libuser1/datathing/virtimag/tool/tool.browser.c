#include "libuser.h"
#define STRBUF listptr.buf0
#define DATBUF listptr.buf1
void printhtmlbody(u8* buf, int len);




static void browser_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
	struct str* str = act->STRBUF;
	struct str* dat = act->DATBUF;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
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
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	struct str* str = act->STRBUF;
	struct str* dat = act->DATBUF;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
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
	gl41string(win, 0x000000, tc, tr, tf, str->buf, str->len);
}
static void browser_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void browser_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void browser_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void browser_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}




static void browser_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
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
	_obj* act, struct style* pin,
	u8* buf, int len)
{
	int j,cnt;
	u8* tmp;
	struct str* dat = act->DATBUF;
	cnt = dat->len;
	tmp = dat->buf;

	for(j=0;j<len;j++)tmp[cnt+j] = buf[j];
	dat->len += len;

	printhtmlbody(dat->buf, dat->len);
}




static void browser_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	browser_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void browser_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		browser_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void browser_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void browser_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void browser_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void browser_search(_obj* act)
{
}
static void browser_modify(_obj* act)
{
}
static void browser_delete(_obj* act)
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
static void browser_create(_obj* act)
{
	int j;
	u8* buf;
	if(0 == act)return;

	buf = act->STRBUF = memorycreate(0x1000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;

	buf = act->DATBUF = memorycreate(0x100000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;
}




void browser_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onsearch = (void*)browser_search;
	p->onmodify = (void*)browser_modify;

	p->onlinkup = (void*)browser_linkup;
	p->ondiscon = (void*)browser_discon;
	p->ontaking = (void*)browser_taking;
	p->ongiving = (void*)browser_giving;
}
