#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define SLICE 16
#define DATBUF buf0
#define DATLEN data1
#define TABBUF buf2
#define TABLEN data3
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);



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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
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
	struct entity* act, struct style* pin,
	struct supply* wnd, struct style* sty)
{
	int cx,cy,ww,hh;
	int x,t,m;
	void** tab;
	short* buf;
	if(0 == sty)return;

	cx = sty->fs.vc[0];
	cy = sty->fs.vc[1];
	ww = sty->fs.vr[0];
	hh = sty->fs.vf[1];

	tab = act->TABBUF;
	if(0 == tab)return;

	for(t=0;t<SLICE;t++){
		buf = tab[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			m = cx-ww + (t*1024+x)*(2*ww)/(SLICE*1024);
			drawline((void*)wnd, 0xffffff, m, cy, m, cy + buf[x]*hh/32768);
		}
	}
}/*
static void oscillo_dx11draw(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	gl41line_rect(ctx, 0xffff00, vc, vr, vf);

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

			gl41line(ctx, 0xffffff, ta, tb);
		}
	}
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
}
static void oscillo_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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

	idx = (act->TABLEN+1) % SLICE;
	act->TABLEN = idx;

	tab = act->TABBUF;
	tab[idx] = buf;
}
void oscillo_pcm(struct entity* ent, struct supply* sup)
{
	struct pcmdata* pcm;
	if(0 == ent->DATBUF)return;
//say("@oscillo_pcm\n");
	pcm = ent->DATBUF + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;

	sup->pcmeasy_data = pcm;
}




static void oscillo_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if( 0 == stack)return;
	if('v'!= key)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:
	case _mt20full_:
	case _gl41full_:
	case _vk12full_:
		oscillo_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void oscillo_read_bywnd(_ent* ent,struct style* slot, _sup* wnd,struct style* area)
{
}




static void oscillo_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct supply* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//say("fmt=%.8s\n", &sup->fmt);

	switch(wnd->fmt){
	case _pcm_:{
		oscillo_pcm(ent, wnd);break;
	}
	case _rgba_:{
		oscillo_draw_pixel(ent,slot, wnd,area);break;
	}
	case _gl41full_:{
		if('v' != key)break;
		oscillo_read_bywnd(ent,slot, wnd,area);break;
	}
	default:{
		oscillo_read_bycam(ent,foot, stack,sp, arg,key);break;
	}
	}
}
static void oscillo_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_pcm_ == stack[sp-1].flag){
		oscillo_data(ent, 0, buf, len);
		return;
	}
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

	act->TABLEN = 0;
	tab = act->TABBUF = memorycreate(0x1000, 0);

	if(arg){
		buf = act->DATBUF = memorycreate(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);

		for(j=0;j<SLICE;j++)tab[j] = buf+44 + j*1024;
		act->TABLEN = SLICE;
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
	p->ontaking = (void*)oscillo_taking;
	p->ongiving = (void*)oscillo_giving;
}
