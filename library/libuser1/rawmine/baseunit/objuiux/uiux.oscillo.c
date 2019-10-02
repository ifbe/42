#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define _pcm_ hex32('p','c','m',0)
#define slice 16
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
/*
static void oscillo_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	short* pcmbuf = (act->buf)+0x80000;
	for(x=0;x<1024*16;x++)
	{
		tc[0] = vc[0] + vr[0] * (x/8192.0 - 1.0);
		tc[1] = vc[1] + vr[1] * (x/8192.0 - 1.0);
		tc[2] = vc[2] + vr[2] * (x/8192.0 - 1.0);
		tr[0] = tc[0] + vf[0]*pcmbuf[x]/16384.0;
		tr[1] = tc[1] + vf[1]*pcmbuf[x]/16384.0;
		tr[2] = tc[2] + vf[2]*pcmbuf[x]/16384.0;
		carveline2d(win, 0xffffff, tc, tr);
	}

	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;
	for(x=0;x<512;x++)
	{
		a = x*tau/512;
		c = cosine(a);
		s = sine(a);

		tc[0] = vc[0] + vr[0] * (x-256) / 256;
		tc[1] = vc[1] + vr[1] * (x-256) / 256;
		tc[2] = vc[2] + vr[2] * (x-256) / 256;
		tr[0] = tc[0] + vf[0]*amp[x]*10;
		tr[1] = tc[1] + vf[1]*amp[x]*10;
		tr[2] = tc[2] + vf[2]*amp[x]*10;
		carveline2d(win, 0xff0000, tc, tr);
	}
}*/
static void oscillo_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,t;
	float tmp,val;
	vec3 ta,tb;
	void** tab;
	short* buf;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carveline_rect(win, 0xffff00, vc, vr, vf);

	tab = act->buf;
	if(0 == tab)return;
	//printmemory(tab, 8*4);

	for(t=0;t<slice;t++){
		buf = tab[t];
		if(0 == buf)break;

		for(x=0;x<1024;x++){
			tmp = (t*1024 + x)/(slice*512.0) - 1.0;
			ta[0] = vc[0] + vr[0] * tmp;
			ta[1] = vc[1] + vr[1] * tmp;
			ta[2] = vc[2] + vr[2] * tmp;

			val = buf[x] / 32768.0;
			tb[0] = ta[0] + vf[0] * val;
			tb[1] = ta[1] + vf[1] * val;
			tb[2] = ta[2] + vf[2] * val;

			carveline(win, 0xffffff, ta, tb);
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
		carveline(win, 0xffffff, tc, tr);
	}*/
}
static void oscillo_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void oscillo_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("oscillo(%x,%x,%x)\n",win,act,sty);
}
static void oscillo_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(0 == act->buf)return;

	if(fmt == _cli_)oscillo_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)oscillo_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)oscillo_draw_html(act, pin, win, sty);
	else if(fmt == _json_)oscillo_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)oscillo_draw_vbo2d(act, pin, win, sty);
		//else oscillo_draw_vbo3d(act, pin, win, sty);
	}
	else oscillo_draw_pixel(act, pin, win, sty);
}



/*
static void oscillo_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev)
{
	int j,k;
	if(_char_ == ev->what)
	{
		k = ev->why;
		if((k>='0')&&(k<='9'))haha=k;
	}
}
static void oscillo_update(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	u8* buf, int len)
{
	int j,k;
	float f;
	float* real;
	float* imag;
	float* amp;
	struct perframe* frame = act->buf;
	short* pcmbuf = (act->buf)+0x80000+(cur*1024*2);
	short* pcmin = (void*)buf;

	soundwrite(0,0,buf,1024*2);
say("%llx, %x\n", buf, len);

	cur = (cur+1) % 16;
	real = frame[cur].real;
	imag = frame[cur].imag;
	amp = frame[cur].amp;

	for(j=0;j<1024;j++)
	{
		pcmbuf[j] = pcmin[j];
		real[j] = (float)pcmbuf[j] / 32768.0;
		imag[j] = 0.0;
	}
	fft(real, imag, 10);
	//say("%f,%f\n",real[0],imag[0]);

	k = 0;
	f = 0.0;
	for(j=0;j<512;j++)
	{
		amp[j] = squareroot(real[j]*real[j] + imag[j]*imag[j]) / 1024;
		if(j < 1)continue;

		if(amp[j] > f)
		{
			k = j;
			f = amp[j];
			//say("%f,%f\n",real[j],imag[j]);
		}
	}
	//say("k=%d\n",k);
	that = k*44100/1024;
}*/
void oscillo_data(struct actor* act, int type, void* buf, int len)
{
	int idx;
	void** tab;
	say("@oscillo_write.pcm: %d\n", len);

	idx = act->len;
	tab = act->buf;
	tab[idx] = buf;
	act->len = (idx+1) % slice;
}




static void oscillo_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	if(ctx){
		if(_gl41data_ == ctx->type)oscillo_draw_vbo3d(act,pin,ctx,sty);
	}
}
static void oscillo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	//struct actor* win = (void*)(peer->chip);
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
static void oscillo_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void oscillo_start(struct halfrel* self, struct halfrel* peer)
{
	say("@oscillo_start\n");
}




static void oscillo_search(struct actor* act)
{
}
static void oscillo_modify(struct actor* act)
{
}
static void oscillo_delete(struct actor* act)
{
}
static void oscillo_create(struct actor* act)
{
	act->buf = memorycreate(0x1000, 0);
	act->len = 0;
}




void oscillo_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 's', 'c', 'i', 'l', 'l', 'o', 0);

	p->oncreate = (void*)oscillo_create;
	p->ondelete = (void*)oscillo_delete;
	p->onsearch = (void*)oscillo_search;
	p->onmodify = (void*)oscillo_modify;

	p->onstart = (void*)oscillo_start;
	p->onstop  = (void*)oscillo_stop;
	p->onread  = (void*)oscillo_read;
	p->onwrite = (void*)oscillo_write;
}
