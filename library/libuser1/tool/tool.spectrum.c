#include "actor.h"
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define tau PI*2
#define _mic_ hex32('m','i','c',0)
//libsoft1
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);
//libsoft0
int soundread(int dev, int time, void* buf, int len);
int soundwrite(int dev, int time, void* buf, int len);
//
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* relation_read(u64);
void* relation_write(void*, void*, u64, void*, void*, u64);
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
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	u16* pcm;
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;

	if(0x30 == haha)
	{
		pcm = (act->buf)+0x80000;
		for(x=0;x<0x2000;x++)
		{
			drawline(win, 0xffffff,
				cx-ww + (x*2+1)*ww/0x2000, cy+hh,
				cx-ww + (x*2+1)*ww/0x2000, cy+hh-(pcm[x]*hh/65536)
			);
		}
	}
	else if(haha&1)
	{
		for(x=0;x<512;x++)
		{
			t = (float)hh*amp[x]*2;
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
				cx + (int)(cc * (1.0 - 2*amp[x])),
				cy + (int)(ss * (1.0 - 2*amp[x])),
				cx + (int)cc,
				cy + (int)ss
			);
		}
	}
	drawdecimal(win, 0xffffff, cx, cy, that);
}
static void spectrum_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void spectrum_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x;
	float cc,ss;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	struct perframe* frame = act->buf;
	float* real = frame[cur].real;
	float* imag = frame[cur].imag;
	float* amp = frame[cur].amp;

	for(x=0;x<512;x++)
	{
		cc = cosine(x*tau/512);
		ss = sine(x*tau/512);
		carveline(
			win, (255-x/2)+(x<<15),
			cx + cc, cy + ss, 0.0,
			cx + cc, cy + ss, amp[x]
		);
	}
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

	if(fmt == _cli_)spectrum_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)spectrum_read_tui(win, sty, act, pin);
	else if(fmt == _vbo_)spectrum_read_vbo(win, sty, act, pin);
	else if(fmt == _html_)spectrum_read_html(win, sty, act, pin);
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
	u16* pcmbuf = (act->buf)+0x80000+((cur/4)*1024*2);
	u16* pcmin = (void*)buf;

	if(0 != win)
	{
		cur = (cur+1)%32;
		real = frame[cur].real;
		imag = frame[cur].imag;
		amp = frame[cur].amp;

		for(j=0;j<1024;j++)
		{
			pcmbuf[j] = pcmin[j];
			real[j] = (float)pcmbuf[j] / 65536.0;
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
static void spectrum_list()
{
}
static void spectrum_into()
{
}
static void spectrum_stop(struct actor* act, struct pinid* pin)
{
}
static void spectrum_start(struct actor* act, struct pinid* pin)
{
}
static void spectrum_delete(struct actor* act)
{
	if(0 == act)return;
	stopmemory(act->buf);
}
static void spectrum_create(struct actor* act)
{
	struct arena* win;
	if(0 == act)return;

	act->buf = startmemory(0x100000);
	if(0 == act->buf)return;

	win = arenacreate(_mic_, "0");
	say("win=%llx\n",win);
	if(0 == win)return;

	relation_write(act, 0, _act_, win, 0, _win_);
	say("%llx,%llx\n", win->irel, win->orel);
	say("%llx,%llx\n", act->irel, act->orel);
}




void spectrum_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)spectrum_create;
	p->ondelete = (void*)spectrum_delete;
	p->onstart  = (void*)spectrum_start;
	p->onstop   = (void*)spectrum_stop;
	p->onlist   = (void*)spectrum_list;
	p->onchoose = (void*)spectrum_into;
	p->onread   = (void*)spectrum_read;
	p->onwrite  = (void*)spectrum_write;
}
