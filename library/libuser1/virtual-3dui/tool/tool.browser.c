#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_whcam(_obj* wnd, struct style* area);
void printhtmlbody(u8* buf, int len);


struct privdata{
	struct str* url;
	struct str* dat;
};


static void browser_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
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

	struct privdata* priv = (void*)(act->priv_256b);
	struct str* url = priv->url;
	struct str* dat = priv->dat;

	//address
	x0 = cx-ww+32;
	y0 = cy-hh+8;
	x1 = cx+ww-32;
	y1 = cy-hh+24;
	drawsolid_rect(win, 0xc0c0c0, x0, y0-4, x1, y1+4);
	drawstring(win, 0x000000, x0, y0, url->buf, url->len);

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
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	int j;
	vec3 tc,tr,tf,tu;
	struct privdata* priv = (void*)(act->priv_256b);

	//url
	struct str* url = priv->url;
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vf[j]*31/32 + vu[j]*0.001;
		tf[j] = vf[j]/32;
	}
	gl41solid_rect(ctx, 0xcccc00, tc, vr, tf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j] + vf[j]*15/16 + vu[j]*0.002;
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
	}
	gl41string(ctx, 0x000000, tc, tr, tf, url->buf, url->len);

	//data
	struct str* dat = priv->dat;
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]*0.001;
		tf[j] = vf[j]*15/16;
	}
	gl41solid_rect(ctx, 0xc0c0c0, tc, vr, tf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]*0.002;
		tr[j] = vr[j];
		tf[j] = vf[j]*15/16;
	}
	gl41text(ctx, 0x000000, tc, tr, tf, dat->buf, dat->len);

	//status
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vf[j]*31/32 + vu[j]*0.001;
		tf[j] = vf[j]/32;
	}
	gl41solid_rect(ctx, 0x0000cc, tc, vr, tf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j] - vf[j] + vu[j]*0.002;
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
	}
	gl41string(ctx, 0x000000, tc, tr, tf, (void*)"loading", 7);
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
	if(_char_ != ev->what)return;

	struct privdata* priv = (void*)(act->priv_256b);
	struct str* url = priv->url;
	if(0 == url)return;

	int len = url->len;
	u8* buf = url->buf;
	if(0xd == ev->why)
	{
		url->len = 0;

		if(0 == ncmp(buf, "file://", 7))
		{
			struct str* dat = priv->dat;
			if(0 == dat)return;

			dat->len = openreadclose(url->buf+7, 0, dat->buf, 0xf0000);
			printhtmlbody(dat->buf, dat->len);
			return;
		}
		if(0 == ncmp(buf, "http://", 7))
		{
			struct str* dat = priv->dat;
			if(0 == dat)return;
			dat->len = 0;

			_obj* http = artery_create(_http_, buf+7, 0, 0);
			if(0 == http){
				say("err@http\n");
				return;
			}
			struct relation* rel1 = relationcreate(act, 0, _ent_, _http_, http, 0, _art_, _dst_);
			if(0 == rel1){
				say("err@rel1\n");
				return;
			}
			relationattach((void*)rel1->dst, (void*)rel1->src);

			_obj* sock = system_create(_tcp_, buf+7, 0, 0);
			if(0 == sock){
				say("err@sock\n");
				return;
			}
			struct relation* rel2 = relationcreate(http, 0, _art_, _src_, sock, 0, _sys_, _dst_);
			if(0 == rel2){
				say("err@rel2\n");
				return;
			}
			relationattach((void*)rel2->dst, (void*)rel2->src);
			return;
		}
		say("not support: %s\n", url->buf);
	}
	else if(0x8 == ev->why)
	{
		if(len <= 0)
		{
			url->len = 0;
		}
		else
		{
			len--;
			buf[len] = 0;
			url->len = len;
		}
	}
	else
	{
		if(len < 0xfc)
		{
			buf[len] = ev->why;
			buf[len+1] = 0;
			url->len = len+1;
		}
	}
}
static void browser_data(
	_obj* act, struct style* pin,
	u8* buf, int len)
{
	struct privdata* priv = (void*)(act->priv_256b);
	struct str* dat = priv->dat;

	int j;
	for(j=0;j<len;j++)dat->buf[dat->len+j] = buf[j];
	dat->len += len;

	//printhtmlbody(dat->buf, dat->len);
}




static void browser_byworld_bycam_byglwnd_read(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	browser_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void browser_bywnd_read(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vc[0] = (area->fs.vc[0]+area->fs.vq[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vc[1] = (area->fs.vc[1]+area->fs.vq[1]) * wnd->whdf.fbheight / 2.0;
	fs.vr[0] = (area->fs.vq[0]-area->fs.vc[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vf[1] = (area->fs.vq[1]-area->fs.vc[1]) * wnd->whdf.fbheight/ 2.0;
	fs.vt[2] = 1.0;

	gl41data_before(wnd);
	gl41data_whcam(wnd, area);
	browser_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);
}




static void browser_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		browser_bywnd_read(ent,slot, caller,area);
		break;
	default:
		browser_byworld_bycam_byglwnd_read(ent,slot, stack,sp);
		break;
	}
}
static void browser_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		browser_event(ent, foot, caller, area, buf);
		break;
	default:
		if(_http_ == stack[sp-1].foottype)browser_data(ent, foot, buf, len);
	}
}
static void browser_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void browser_attach(struct halfrel* self, struct halfrel* peer)
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
	struct privdata* priv = (void*)(act->priv_256b);
	if(priv->url){
		memoryfree(priv->url);
		priv->url = 0;
	}
	if(priv->dat){
		memoryfree(priv->dat);
		priv->dat = 0;
	}
}
static void browser_create(_obj* act)
{
	if(0 == act)return;

	struct privdata* priv = (void*)(act->priv_256b);

	int j;
	u8* buf;
	buf = memoryalloc(0x1000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;
	priv->url = (void*)buf;

	buf = memoryalloc(0x100000, 0);
	for(j=0;j<0x1000;j++)buf[j] = 0;
	priv->dat = (void*)buf;
}




void browser_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onreader = (void*)browser_search;
	p->onwriter = (void*)browser_modify;

	p->onattach = (void*)browser_attach;
	p->ondetach = (void*)browser_detach;
	p->ontaking = (void*)browser_taking;
	p->ongiving = (void*)browser_giving;
}
