#include "actor.h"
#define _mic_ hex32('m','i','c',0)
#define cccc 444.0
#define c__d 470.40161389552713
#define dddd 498.3731494493616
#define d__e 528.0079590612081
#define eeee 559.4049461533237
#define ffff 592.6688952514953
#define f__g 627.9108216936543
#define gggg 665.2483421332466
#define g__a 704.8060670738805
#define aaaa 746.7160167452985
#define a__b 791.1180617086213
#define bbbb 838.1603896613437
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);
int soundread(int dev, int time, void* buf, int len);
int soundwrite(int dev, int time, void* buf, int len);




struct perframe
{
	float real[4096];
	float imag[4096];
	float amp[4096];
	float phase[4096];
};
static struct perframe* frame;




static void piano_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 color;
	int x,y;
	int x1,y1,x2,y2;
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;

	int j;
	for(j=0;j<7;j++)
	{
		drawsolid_rect(
			win, 0xffffff,
			(cx-ww+4)+(2*j+0)*ww/7, (cy-hh+4),
			(cx-ww-4)+(2*j+2)*ww/7, (cy+hh-4)
		);
	}
	for(j=0;j<6;j++)
	{
		if(2 == j)continue;
		drawsolid_rect(
			win, 0x111111,
			(cx-ww+4)+(2*j+1)*ww/7, (cy-hh+4),
			(cx-ww-4)+(2*j+3)*ww/7, (cy-4)
		);
	}
}
static void piano_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void piano_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void piano_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void piano_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("piano(%x,%x,%x)\n",win,act,sty);
}
static void piano_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)piano_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)piano_read_tui(win, sty, act, pin);
	else if(fmt == _vbo_)piano_read_vbo(win, sty, act, pin);
	else if(fmt == _html_)piano_read_html(win, sty, act, pin);
	else piano_read_pixel(win, sty, act, pin);
}
static void piano_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,k;
	float f;
	float* real;
	float* imag;
	float* amp;
	u16* pcmout;
	if(0 != win)return;

	if(_char_ == ev->what)
	{
		k = ev->why;
		if('c' == k)f = cccc;
		else if('d' == k)f = dddd;
		else if('e' == k)f = eeee;
		else if('f' == k)f = ffff;
		else if('g' == k)f = gggg;
		else if('a' == k)f = aaaa;
		else if('b' == k)f = bbbb;
		else if('1' == k)f = cccc;
		else if('2' == k)f = c__d;
		else if('3' == k)f = dddd;
		else if('4' == k)f = d__e;
		else if('5' == k)f = eeee;
		else if('6' == k)f = ffff;
		else if('7' == k)f = f__g;
		else if('8' == k)f = gggg;
		else if('9' == k)f = g__a;
		else if('0' == k)f = aaaa;
		else if('-' == k)f = a__b;
		else if('=' == k)f = bbbb;

		real = frame[0].real;
		imag = frame[0].imag;
		for(j=0;j<4096;j++)
		{
			real[j] = 0.0;
			imag[j] = 0.0;
		}

		j = (f*4096)/44100;
		real[j] = 4096.0;
		real[4096-j] = 4096.0;

		ifft(real, imag, 12);
		pcmout = (act->buf)+0x80000;
		for(j=0;j<4096;j++)
		{
			pcmout[j] = (int)(real[j]*65535.0);
		}
		soundwrite(0, 0, pcmout, 4096*2);
	}
}
static void piano_list()
{
}
static void piano_into()
{
}
static void piano_stop(struct actor* act, struct pinid* pin)
{
}
static void piano_start(struct actor* act, struct pinid* pin)
{
}
static void piano_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void piano_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(0x100000);
	frame = act->buf;
}




void piano_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'i', 'a', 'n', 'o', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)piano_create;
	p->ondelete = (void*)piano_delete;
	p->onstart  = (void*)piano_start;
	p->onstop   = (void*)piano_stop;
	p->onlist   = (void*)piano_list;
	p->onchoose = (void*)piano_into;
	p->onread   = (void*)piano_read;
	p->onwrite  = (void*)piano_write;
}
