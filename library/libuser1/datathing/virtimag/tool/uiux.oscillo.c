#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define SLICE 16
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);




struct peroscillo{
	int tablen;
	void* tabbuf[SLICE];

	int datlen;
	void* datbuf;
};
/*
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	float t,cc,ss;
	int x,y;
	int cx, cy, ww, hh;
	short* pcm;
	struct perframe* frame = act->buf0;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;
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

	if(0x30 == haha)
	{
		pcm = (act->buf0)+0x80000;
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
			cc = getcos(t) * ww;
			ss = -getsin(t) * hh;
			drawline(win, 0xffffff,
				cx + (int)(cc * (1.0 - 16*amp[x])),
				cy + (int)(ss * (1.0 - 16*amp[x])),
				cx + (int)cc,
				cy + (int)ss
			);
		}
	}
	drawdecimal(win, 0xffffff, cx, cy, that);
}
*/
static void oscillo_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
	if(0 == sty)return;

	int cx,cy,ww,hh;
	cx = sty->fs.vc[0];
	cy = sty->fs.vc[1];
	ww = sty->fs.vr[0];
	hh = sty->fs.vf[1];

	struct peroscillo* per = (void*)act->priv_256b;
	int x,t,m;
	short* buf;
	for(t=0;t<SLICE;t++){
		buf = per->tabbuf[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			m = cx-ww + (t*1024+x)*(2*ww)/(SLICE*1024);
			drawline((void*)wnd, 0xffffff, m, cy, m, cy + buf[x]*hh/32768);
		}
	}
}/*
static void oscillo_dx11draw(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,t;
	float tmp,val;
	vec3 ta,tb;
	void** tab;
	short* buf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	dx11line_rect(ctx, 0xffff00, vc, vr, vf);

	tab = act->TABBUF;
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

			dx11line(ctx, 0xffffff, ta, tb);
		}
	}
}*/
static void oscillo_gl41draw(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0xffff00, vc, vr, vf);

	struct peroscillo* per = (void*)act->priv_256b;
	//printmemory(tab, 8*4);

	int x,t;
	float tmp,val;
	vec3 ta,tb;
	short* buf;
	for(t=0;t<SLICE;t++){
		buf = per->tabbuf[t];
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
}
static void oscillo_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void oscillo_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void oscillo_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void oscillo_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("oscillo(%x,%x,%x)\n",win,act,sty);
}
void oscillo_getpcm(_obj* ent, _obj* sup)
{
	struct peroscillo* per = (void*)ent->priv_256b;
	if(0 == per->datbuf)return;

	struct pcmdata* pcm;
//say("@oscillo_getpcm\n");
	pcm = per->datbuf + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;

	sup->pcmeasy.data = pcm;
}




void oscillo_data(_obj* act, int type, void* buf, int len)
{
	say("@oscillo_write.pcm: %d\n", len);

	struct peroscillo* per = (void*)act->priv_256b;
	per->tabbuf[per->tablen] = buf;
	per->tablen = (per->tablen+1) % SLICE;
}




static void oscillo_read_bycam(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->hfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		oscillo_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void oscillo_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
}




static void oscillo_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//say("fmt=%.8s\n", &sup->hfmt);

	switch(wnd->hfmt){
	case _pcm_:
		oscillo_getpcm(ent, wnd);
		break;
	case _rgba_:
		oscillo_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		oscillo_read_bywnd(ent,slot, wnd,area);
		break;
	default:
		oscillo_read_bycam(ent,slot, stack,sp);
		break;
	}
}
static void oscillo_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_pcm_ == stack[sp-1].foottype){
		oscillo_data(ent, 0, buf, len);
		return;
	}
}
static void oscillo_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void oscillo_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@oscillo_attach\n");
}




static void oscillo_search(_obj* act)
{
}
static void oscillo_modify(_obj* act)
{
}
static void oscillo_delete(_obj* act)
{
}
static void oscillo_create(_obj* act, u8* arg)
{
	int j;
	struct peroscillo* per = (void*)act->priv_256b;
	per->tablen = 0;
	per->datlen = 0;

	if(arg){	//wav file
		per->datbuf = memorycreate(0x100000, 0);
		openreadclose(arg, 0, per->datbuf, 0x100000);

		for(j=0;j<SLICE;j++)per->tabbuf[j] = per->datbuf+44 + j*1024;
		per->tablen = SLICE;
	}
}




void oscillo_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('o', 's', 'c', 'i', 'l', 'l', 'o', 0);

	p->oncreate = (void*)oscillo_create;
	p->ondelete = (void*)oscillo_delete;
	p->onreader = (void*)oscillo_search;
	p->onwriter = (void*)oscillo_modify;

	p->onattach = (void*)oscillo_attach;
	p->ondetach = (void*)oscillo_detach;
	p->ontaking = (void*)oscillo_taking;
	p->ongiving = (void*)oscillo_giving;
}
