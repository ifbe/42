#include "libuser.h"
int qrcode_generate(void* src, void* dst, int len);




static int slen;
static u8 databuf[49*49];




static void qrcode_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u32 color;
	int x,y;
	int x1,y1,x2,y2;
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
static void qrcode_draw_vbo2d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u32 rgb;
	int x,y,w,h;
	u8 (*tab)[4];
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0x444444, vc, vr, vf);

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
			carvesolid2d_rect(win, rgb, tc, tr, tf);
		}
	}
}
static void qrcode_draw_vbo3d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x444444, vc, vr, vf);
}
static void qrcode_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void qrcode_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}
static void qrcode_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)qrcode_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)qrcode_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)qrcode_draw_html(act, pin, win, sty);
	else if(fmt == _json_)qrcode_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)qrcode_draw_vbo2d(act, pin, win, sty);
		else qrcode_draw_vbo3d(act, pin, win, sty);
	}
	else qrcode_draw_pixel(act, pin, win, sty);
}




static void qrcode_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	qrcode_draw(act, pin, win, sty);
}
static void qrcode_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void qrcode_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void qrcode_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void qrcode_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void qrcode_start(struct halfrel* self, struct halfrel* peer)
{
	slen=49;
	qrcode_generate("haha",databuf,slen);
}
static void qrcode_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void qrcode_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = databuf;
	if(_copy_ == act->type)act->buf = memorycreate(49*49);
}




void qrcode_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);

	p->oncreate = (void*)qrcode_create;
	p->ondelete = (void*)qrcode_delete;
	p->onstart  = (void*)qrcode_start;
	p->onstop   = (void*)qrcode_stop;
	p->oncread  = (void*)qrcode_cread;
	p->oncwrite = (void*)qrcode_cwrite;
	p->onsread  = (void*)qrcode_sread;
	p->onswrite = (void*)qrcode_swrite;
}
