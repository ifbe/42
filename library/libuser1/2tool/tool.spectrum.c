#include "actor.h"
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define tau PI*2
//libsoft1
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
int piano_freq(int);
//libsoft0
int readsound(void* buf, int len);
int writesound(void* buf, int len);
int startsound(int rate, int chan);
int stopsound();




struct waveinfo
{
	void* buf;
	int len;
	int enq;
	int deq;
};
static struct waveinfo* info = 0;
static int area=0;
static int maxamp;
//
static double* real;
static double* imag;
static double* amp;
static double* phase;




static void spectrum_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	double t,cc,ss;
	int x,y;
	int w = win->w;
	int h = win->h;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
/*
	if(?????)
	{
		for(x=0;x<1024;x++)
		{
			if(pcmin[x]>32768)continue;
			y = pcmin[x] *height /maxamp /4;
			line(win,
				x*w/1024, (height/4) - y,
				x*h/1024, (height/4) + y,
				0xffffff
			);
		}
		return;
	}
*/
	for(x=0;x<ww;x++)
	{
		t = tau / ww * x;
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
static void spectrum_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void spectrum_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x;
	float cc,ss;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
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
static void spectrum_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	u8* p = (u8*)(win->buf);

	for(x=0;x<w*h*4;x++)p[x]=0;
	for(x=0;x<w;x++)
	{
		y = h - (int)(real[x] * (double)h / (double)maxamp);
		for(;y<h;y++)
		{
			p[((y*w + x)<<2) + 3] =  0x2;
		}
	}
}
static void spectrum_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("spectrum(%x,%x,%x)\n",win,act,sty);
}
static void spectrum_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)spectrum_read_cli(win, act, sty);
	else if(fmt == _tui_)spectrum_read_tui(win, act, sty);
	else if(fmt == _vbo_)spectrum_read_vbo(win, act, sty);
	else if(fmt == _html_)spectrum_read_html(win, act, sty);
	else spectrum_read_pixel(win, act, sty);
}




void spectrum_fft()
{
	int j;
	u16* pcmin;
	if(info == 0)return;

	pcmin = (info->buf)+(info->deq);
	info->deq = info->enq;

	for(j=0;j<1024;j++)
	{
		real[j] = (double)pcmin[j] / 65536.0;
		imag[j] = 0.0;
	}

	fft(real, imag, 10);
	for(j=0;j<1024;j++)
	{
		//say("%lf	%lf\n", real[j], imag[j]);
		amp[j]=squareroot(real[j]*real[j] + imag[j]*imag[j]) / 1024;
	}
	//say("%lf,%lf,%lf,%lf\n",amp[0],amp[1],amp[2],amp[3]);
}
static void spectrum_write(struct event* ev)
{
	int j,k;
	float max;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _char_)
	{
		if( (key>=0x31) && (key<=0x39) )
		{
			key -= 0x31;
		}
		else if(key == '0')key = 9;
		else if(key == '-')key = 10;
		else if(key == '=')key = 11;
		else
		{
			if( (key>='a') && (key<='z') )
			{
				area = key - 'a';
			}
			return;
		}

		for(j=0;j<1024;j++)real[j] = imag[j] = 0.0;

		j = piano_freq(area*12+key);
		say("%d->",j);
		j = (j*1024)/44100;
		say("%d\n",j);

		real[j] = 1.0;
		writesound(real, 1024*2);
		//real[j]=real[1023-j]=65535;
		//sound_output(real, imag, pcmout);
	}
	else if(type=='s')
	{
		info = (void*)(ev->why);
		spectrum_fft();

		k = 0;
		max = 0.0;
		for(j=20;j<512;j++)
		{
			if(amp[j] > max)
			{
				k = j;
				max = amp[j];
			}
		}
		say("%d,%f,%f\n", k, max, k*22000.0/512.0);
	}
}




static void spectrum_list()
{
}
static void spectrum_into()
{
}
static void spectrum_start()
{
	maxamp = 65536;
	startsound(44100, 2);
}
static void spectrum_stop()
{
}
void spectrum_create(void* uibuf,void* addr)
{
	struct actor* p = addr;
	real = (double*)(uibuf+0x300000);
	imag = (double*)(uibuf+0x340000);
	amp = (double*)(uibuf+0x380000);
	phase = (double*)(uibuf+0x3c0000);

	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');
	p->irel = 0;
	p->orel = 0;

	p->onstart = (void*)spectrum_start;
	p->onstop = (void*)spectrum_stop;
	p->onlist = (void*)spectrum_list;
	p->onchoose = (void*)spectrum_into;
	p->onread = (void*)spectrum_read;
	p->onwrite = (void*)spectrum_write;
}
void spectrum_delete()
{
}
