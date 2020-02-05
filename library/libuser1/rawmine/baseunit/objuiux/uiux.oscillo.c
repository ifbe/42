#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define SLICE 16
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);
//libsoft0
int soundread(int dev, int time, void* buf, int len);
int soundwrite(int dev, int time, void* buf, int len);




struct perframe
{
	float real[1024];
	float imag[1024];
	float amp[1024];
	float phase[1024];
};
static int cur = 0;
static int haha;
static int that;




static void oscillo_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{/*
	float t,cc,ss;
	int x,y;
	int cx, cy, ww, hh;
	short* pcm;
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;
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

	if(0x30 == haha)
	{
		pcm = (act->buf)+0x80000;
		for(x=0;x<0x2000;x++)
		{
			drawline(win, 0xffffff,
				cx-ww + (x*2+1)*ww/0x2000, cy,
				cx-ww + (x*2+1)*ww/0x2000, cy-(pcm[x]*hh/32768)
			);
		}
	}
	else if(haha&1)
	{
		for(x=0;x<512;x++)
		{
			t = (float)hh*amp[x]*16;
			drawline(win, 0xffffff,
				cx-ww + (x*2+1)*ww/512, cy+hh,
				cx-ww + (x*2+1)*ww/512, cy+hh-(int)t
			);
		}
	}
	else
	{
		for(x=0;x<512;x++)
		{
			t = tau / 512 * x;
			cc = cosine(t) * ww;
			ss = -sine(t) * hh;
			drawline(win, 0xffffff,
				cx + (int)(cc * (1.0 - 16*amp[x])),
				cy + (int)(ss * (1.0 - 16*amp[x])),
				cx + (int)cc,
				cy + (int)ss
			);
		}
	}
	drawdecimal(win, 0xffffff, cx, cy, that);
*/
}
static void oscillo_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,t;
	float tmp,val;
	vec3 ta,tb;
	void** tab;
	short* buf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_rect(ctx, 0xffff00, vc, vr, vf);

	tab = act->buf;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	for(t=0;t<SLICE;t++){
		buf = tab[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			tmp = (t*1024 + x)/(SLICE*512.0) - 1.0;
			ta[0] = vc[0] + vr[0] * tmp;
			ta[1] = vc[1] + vr[1] * tmp;
			ta[2] = vc[2] + vr[2] * tmp;

			val = buf[x] / 32768.0;
			tb[0] = ta[0] + vf[0] * val;
			tb[1] = ta[1] + vf[1] * val;
			tb[2] = ta[2] + vf[2] * val;

			gl41line(ctx, 0xffffff, ta, tb);
		}
	}

/*	int x;
	float a,c,s,t;
	vec3 tc, tr, tf, tu;
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	for(x=0;x<512;x++)
	{
		a = x*tau/512;
		c = cosine(a);
		s = sine(a);
		t = 1.0 - amp[x]*10.0;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s;
		c *= t;
		s *= t;

		tr[0] = vc[0] + vr[0]*c + vf[0]*s;
		tr[1] = vc[1] + vr[1]*c + vf[1]*s;
		tr[2] = vc[2] + vr[2]*c + vf[2]*s;
		gl41line(win, 0xffffff, tc, tr);
	}*/
}
static void oscillo_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void oscillo_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"oscillo\" style=\"width:50%%;height:100px;float:left;background-color:#7ae129;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void oscillo_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	int w = win->stride;
	int h = win->height;
	u8* p = (u8*)(win->buf);
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;

	for(x=0;x<w;x++)
	{
		y = h - (int)(real[x] * (float)h / 65536.0);
		for(;y<h;y++)
		{
			p[((y*w + x)<<2) + 3] =  0x2;
		}
	}
}
static void oscillo_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("oscillo(%x,%x,%x)\n",win,act,sty);
}




void oscillo_data(struct entity* act, int type, void* buf, int len)
{
	int idx;
	void** tab;
	say("@oscillo_write.pcm: %d\n", len);

	idx = act->len;
	tab = act->buf;
	tab[idx] = buf;
	act->len = (idx+1) % SLICE;
}
void oscillo_pcm(struct entity* ent, struct supply* sup)
{
	int j;
	struct pcmdata* pcm;
	if(0 == ent->buf0)return;
//say("@oscillo_pcm\n");
	pcm = ent->buf0 + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;
	sup->pcmdata = pcm;
}




static void oscillo_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> oscillo
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)oscillo_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void oscillo_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
}




static void oscillo_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct supply* sup = peer->pchip;
//say("fmt=%.8s\n", &sup->fmt);
	switch(sup->fmt){
	case _pcm_:{
		oscillo_pcm(ent, sup);break;
	}
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != len)break;
		oscillo_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	default:{
		oscillo_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
	}
}
static void oscillo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	//struct entity* win = (void*)(peer->chip);
	//struct style* sty = (void*)(peer->foot);
	if(_pcm_ == self->flag){
		oscillo_data(act, 0, buf, len);
	}
/*
	switch(self->flag){
		case _pcm_:oscillo_update(act, pin, win, sty, buf, len);break;
		default:   oscillo_event( act, pin, win, sty, buf);break;
	}
*/
}
static void oscillo_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void oscillo_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@oscillo_linkup\n");
}




static void oscillo_search(struct entity* act)
{
}
static void oscillo_modify(struct entity* act)
{
}
static void oscillo_delete(struct entity* act)
{
}
static void oscillo_create(struct entity* act, u8* arg)
{
	int j;
	void* buf;
	void** tab;

	act->len = 0;
	tab = act->buf = memorycreate(0x1000, 0);

	if(arg){
		buf = act->buf0 = memorycreate(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);

		for(j=0;j<SLICE;j++)tab[j] = buf+44 + j*1024;
		act->len = SLICE;
	}
}




void oscillo_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 's', 'c', 'i', 'l', 'l', 'o', 0);

	p->oncreate = (void*)oscillo_create;
	p->ondelete = (void*)oscillo_delete;
	p->onsearch = (void*)oscillo_search;
	p->onmodify = (void*)oscillo_modify;

	p->onlinkup = (void*)oscillo_linkup;
	p->ondiscon = (void*)oscillo_discon;
	p->onread  = (void*)oscillo_read;
	p->onwrite = (void*)oscillo_write;
}
