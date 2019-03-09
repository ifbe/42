#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);
//libsoft0
int soundread(int dev, int time, void* buf, int len);
int soundwrite(int dev, int time, void* buf, int len);
//
void* arenacreate(u64, void*);




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




static void spectrum_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
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
}
static void spectrum_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	short* pcmbuf = (act->buf)+0x80000;
	for(x=0;x<8192;x++)
	{
		tc[0] = vc[0] + vr[0] * (x/4096.0 - 1.0);
		tc[1] = vc[1] + vr[1] * (x/4096.0 - 1.0);
		tc[2] = vc[2] + vr[2] * (x/4096.0 - 1.0);
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
}
static void spectrum_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	for(x=0;x<512;x++)
	{
		a = x*tau/512;
		c = cosine(a);
		s = sine(a);

		tc[0] = vc[0] + vr[0]*c + vf[0]*s;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s;
		tr[0] = tc[0] + vu[0]*amp[x];
		tr[1] = tc[1] + vu[1]*amp[x];
		tr[2] = tc[2] + vu[2]*amp[x];
		carveline(win, 0xffffff, tc, tr);
	}
}
static void spectrum_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spectrum_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"spectrum\" style=\"width:50%%;height:100px;float:left;background-color:#7ae129;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void spectrum_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void spectrum_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("spectrum(%x,%x,%x)\n",win,act,sty);
}
static void spectrum_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(0 == act->buf)return;

	if(fmt == _cli_)spectrum_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)spectrum_read_tui(win, sty, act, pin);
	else if(fmt == _html_)spectrum_read_html(win, sty, act, pin);
	else if(fmt == _json_)spectrum_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)spectrum_read_vbo2d(win, sty, act, pin);
		else spectrum_read_vbo3d(win, sty, act, pin);
	}
	else spectrum_read_pixel(win, sty, act, pin);
}
static void spectrum_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	int j,k;
	float f;
	float* real;
	float* imag;
	float* amp;
	struct perframe* frame = act->buf;
	short* pcmbuf = (act->buf)+0x80000+((cur/4)*1024*2);
	short* pcmin = (void*)buf;

	if(0 == len)return;
	if(0 != win)
	{
		cur = (cur+1)%32;
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
		return;
	}

	struct event* ev = (void*)buf;
	if(_char_ == ev->what)
	{
		k = ev->why;
		if((k>='0')&&(k<='9'))haha=k;
	}
}
static void spectrum_get()
{
}
static void spectrum_post()
{
}
static void spectrum_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void spectrum_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void spectrum_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void spectrum_create(struct actor* act)
{
	struct arena* win;
	if(0 == act)return;

	act->buf = memorycreate(0x100000);
	if(0 == act->buf)return;

	win = arenacreate(_mic_, "0");
	say("win=%llx\n",win);
	if(0 == win)return;

	relationcreate(act, 0, _act_, win, 0, _win_);
}




void spectrum_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onstart  = (void*)spectrum_start;
	p->onstop   = (void*)spectrum_stop;
	p->onget    = (void*)spectrum_get;
	p->onpost   = (void*)spectrum_post;
	p->onread   = (void*)spectrum_read;
	p->onwrite  = (void*)spectrum_write;
}
