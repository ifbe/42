#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define SLICECOUNT (0xe0/8)
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);




struct peroscillo{
	void* datbuf;
	int datlen;

	void* tabbuf[SLICECOUNT];
	int tablen;
};
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
	for(t=0;t<SLICECOUNT;t++){
		buf = per->tabbuf[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			m = cx-ww + (t*1024+x)*(2*ww)/(SLICECOUNT*1024);
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

	for(t=0;t<SLICECOUNT;t++){
		buf = tab[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			tmp = (t*1024 + x)/(SLICECOUNT*512.0) - 1.0;
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
#if 0
#define SHOWCOUNT SLICECOUNT
	for(t=0;t<SHOWCOUNT;t++){
		buf = per->tabbuf[t];

		if(0 == buf)continue;

		for(x=0;x<1024;x++){
			tmp = (t*1024 + x)/(SHOWCOUNT*512.0) - 1.0;
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
#else
#define SHOWCOUNT 24
	for(t=0;t<SHOWCOUNT;t++){
		buf = per->tabbuf[(SLICECOUNT+per->tablen-SHOWCOUNT+t)%SLICECOUNT];

		if(0 == buf)continue;

		for(x=0;x<1024;x++){
			tmp = (t*1024 + x)/(SHOWCOUNT*512.0) - 1.0;
			ta[0] = vc[0] + vr[0] * tmp;
			ta[1] = vc[1] + vr[1] * tmp;
			ta[2] = vc[2] + vr[2] * tmp;

			val = buf[x] / 32768.0;
			tb[0] = ta[0] + vf[0] * val;
			tb[1] = ta[1] + vf[1] * val;
			tb[2] = ta[2] + vf[2] * val;

			gl41line(ctx, 0xffffff, ta,tb);
		}
	}
#endif
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
static void oscillo_draw_tui256(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8* tuibuf = win->tuitext.buf;
	int tuiwidth = win->whdf.width;
	int tuiheight= win->whdf.height;

	struct peroscillo* per = (void*)act->priv_256b;

	short* buf = per->tabbuf[(SLICECOUNT+per->tablen-1)%SLICECOUNT];
	if(0 == buf)return;

	int x,y,k;
	int sum;
	for(x=0;x<64;x++){
		sum = 0;
		for(k=0;k<16;k++)sum += buf[x*16+k];

		y = 15-2*sum*30/(32768*16);
		if(y<0)y=0;
		if(y+1>tuiheight)y = tuiheight-1;

		tuibuf[4*(tuiwidth*y+x)+0] = 'a';
		tuibuf[4*(tuiwidth*y+x)+2] = 7;
	}
}
static void oscillo_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("oscillo(%x,%x,%x)\n",win,act,sty);
}
void oscillo_getpcm(_obj* ent, _obj* sup)
{
	struct peroscillo* per = (void*)ent->priv_256b;
	if(0 == per->datbuf)return;

	struct pcmdata* pcm;
//logtoall("@oscillo_getpcm\n");
	pcm = per->datbuf + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;

	sup->pcmeasy.data = pcm;
}




void oscillo_data(_obj* act, int type, void* buf, int len)
{
	logtoall("@oscillo_write.pcm: %d\n", len);
	struct peroscillo* per = (void*)act->priv_256b;

	int j;
	u8* dst = per->datbuf + per->datlen;
	u8* src = buf;
	for(j=0;j<len;j++)dst[j] = src[j];

	per->datlen += len;
	if(per->datlen > 0xf0000)per->datlen = 0x10000;

	per->tabbuf[per->tablen] = dst;
	per->tablen = (per->tablen+1) % SLICECOUNT;
}




static void oscillo_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	switch(wnd->vfmt){
	case _cli_:
		break;
	case _tui_:
		oscillo_draw_tui(ent,0, wnd, area);
		break;
	case _tui256_:
		oscillo_draw_tui256(ent,0, wnd, area);
		break;
	case _rgba8888_:
		oscillo_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		oscillo_read_bywnd(ent,slot, wnd,area);
		break;
	}
}

static void oscillo_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		oscillo_gl41draw(ent,slot, wor,geom, wnd,area);
		break;
	}
}



static void oscillo_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//logtoall("fmt=%.8s\n", &sup->type);

	switch(wnd->type){
	case _pcm_:
		oscillo_getpcm(ent, wnd);
		break;
	case _wnd_:
	case _render_:
		oscillo_read_bywnd(ent,slot, wnd,area);
		break;
	default:
		oscillo_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void oscillo_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
	logtoall("@oscillo_attach\n");
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
	struct peroscillo* per = (void*)act->priv_256b;

	per->datlen = 0x10000;
	per->datbuf = memoryalloc(0x100000, 0);
	//logtoall("size=%x\n",sizeof(struct peroscillo));

	int j;
	per->tablen = 0;
	for(j=0;j<SLICECOUNT;j++)per->tabbuf[j] = 0;

	if(arg){	//wav file
		openreadclose(arg, 0, per->datbuf, 0x100000);

		for(j=0;j<SLICECOUNT;j++)per->tabbuf[j] = per->datbuf+44 + j*1024;
		per->tablen = SLICECOUNT;
	}
}




void oscillo_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('o', 's', 'c', 'i', 'l', 'l', 'o', 0);

	p->oncreate = (void*)oscillo_create;
	p->ondelete = (void*)oscillo_delete;
	p->onreader = (void*)oscillo_search;
	p->onwriter = (void*)oscillo_modify;

	p->onattach = (void*)oscillo_attach;
	p->ondetach = (void*)oscillo_detach;
	p->ontaking = (void*)oscillo_taking;
	p->ongiving = (void*)oscillo_giving;
}
