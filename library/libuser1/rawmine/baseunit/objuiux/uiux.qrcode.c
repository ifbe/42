#include "libuser.h"
int qrcode_generate(void* src, void* dst, int len);




static int slen;
static u8 databuf[49*49];




static void qrcode_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 color;
	int x,y;
	int x1,y1,x2,y2;
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

	for(y=0;y<slen;y++)
	{
		for(x=0;x<slen;x++)
		{
			if( databuf[(y*slen)+x] == 0 )color=0;
			else color=0xffffffff;

			x1 = cx + (x*ww*2/slen) - ww;
			y1 = cy + (y*hh*2/slen) - hh;
			x2 = cx + ((x+1)*ww*2/slen) - ww;
			y2 = cy + ((y+1)*hh*2/slen) - hh;
			drawsolid_rect(
				win, color,
				x1, y1, x2, y2
			);
		}
//say("\n");
	}
}
static void qrcode_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u32 rgb;
	int x,y,w,h;
	u8 (*tab)[4];
	vec3 tc, tr, tf, tu, f;

	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	//gl41opaque_rect(ctx, 0x444444, vc, vr, vf);

	tr[0] = vr[0] / 49;
	tr[1] = vr[1] / 49;
	tr[2] = vr[2] / 49;
	tf[0] = vf[0] / 49;
	tf[1] = vf[1] / 49;
	tf[2] = vf[2] / 49;
	for(y=0;y<49;y++)
	{
		for(x=0;x<49;x++)
		{
			if( databuf[(y*slen)+x] == 0 )rgb = 0;
			else rgb = 0xffffffff;

			tc[0] = vc[0] + (x-24)*vr[0]*2/49 + (y-24)*vf[0]*2/49;
			tc[1] = vc[1] + (x-24)*vr[1]*2/49 + (y-24)*vf[1]*2/49;
			tc[2] = vc[2] + (x-24)*vr[2]*2/49 + (y-24)*vf[2]*2/49 - 0.1;
			carvesolid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void qrcode_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void qrcode_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"qrcode\" style=\"width:50%%;height:100px;float:left;background-color:#e127a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void qrcode_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	int width = win->stride;
	int height = win->height;
	u8* p = (u8*)(win->buf);

	for(y=0;y<100;y++)
	{
		if(y >= slen)break;
		if(y >= height)break;
		for(x=0;x<100;x++)
		{
			if(x >= slen)break;
			if(x >= width/2)break;
			if( databuf[(y*slen)+x] != 0 )continue;

			p[( (y*width+x*2)<<2 ) + 3] = 7;
			p[( (y*width+x*2)<<2 ) + 7] = 7;
		}
	}
}
static void qrcode_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}
static void qrcode_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)qrcode_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)qrcode_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)qrcode_draw_html(act, pin, win, sty);
	else if(fmt == _json_)qrcode_draw_json(act, pin, win, sty);
}




static void qrcode_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

//world -> button
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)qrcode_draw_vbo(act,slot, scn,geom, wnd,area);
	}
}
static void qrcode_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = self->pchip;
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
		case _rgba_:qrcode_draw_pixel(ent, self->pfoot, sup, peer->pfoot);break;
		default:    qrcode_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
}
static void qrcode_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void qrcode_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void qrcode_start(struct halfrel* self, struct halfrel* peer)
{
}




static void qrcode_search(struct entity* act)
{
}
static void qrcode_modify(struct entity* act)
{
}
static void qrcode_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void qrcode_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = databuf;
	if(_copy_ == act->type)act->buf = memorycreate(49*49, 0);

	slen=49;
	qrcode_generate("haha",databuf,slen);
}




void qrcode_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);

	p->oncreate = (void*)qrcode_create;
	p->ondelete = (void*)qrcode_delete;
	p->onsearch = (void*)qrcode_search;
	p->onmodify = (void*)qrcode_modify;

	p->onstart  = (void*)qrcode_start;
	p->onstop   = (void*)qrcode_stop;
	p->onread  = (void*)qrcode_read;
	p->onwrite = (void*)qrcode_write;
}
