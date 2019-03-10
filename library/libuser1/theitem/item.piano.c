#include "libuser.h"
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




static void piano_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
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

	int j;
	for(j=0;j<7;j++)
	{
		drawsolid_rect(
			win, 0xffffff,
			(cx-ww+4)+(2*j+0)*ww/7, (cy+4-hh),
			(cx-ww-4)+(2*j+2)*ww/7, (cy-4+hh)
		);
	}
	for(j=0;j<6;j++)
	{
		if(2 == j)continue;
		drawsolid_rect(
			win, 0x111111,
			(cx-ww)+(4*j+3)*ww/14, (cy+4-hh),
			(cx-ww)+(4*j+5)*ww/14, (cy-4)
		);
	}
}
static void piano_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x;
	vec3 tc,tr,tf,tu;
	if(0 == sty)sty = defaultstyle_vbo2d();
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	tr[0] = vr[0] / 7 / 11;
	tr[1] = vr[1] / 7 / 11;
	tr[2] = vr[2] / 7 / 11;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;
	for(x=0;x<7*10;x++)
	{
		tc[0] = vc[0] + vr[0]*(x-34.5)/35 - vf[0]/4;
		tc[1] = vc[1] + vr[1]*(x-34.5)/35 - vf[1]/4;
		tc[2] = vc[2] + vr[2]*(x-34.5)/35 - vf[2]/4;
		carvesolid2d_rect(win, 0xfffffff, tc, tr, tf);
	}

	tr[0] = vr[0] / 7 / 20;
	tr[1] = vr[1] / 7 / 20;
	tr[2] = vr[2] / 7 / 20;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(x=0;x<7*10;x++)
	{
		if((x%7) == 0)continue;
		if((x%7) == 3)continue;
		tc[0] = vc[0] + vr[0]*(x-35)/35 - vf[0]/8;
		tc[1] = vc[1] + vr[1]*(x-35)/35 - vf[1]/8;
		tc[2] = vc[2] + vr[2]*(x-35)/35 - vf[2]/8 - 0.1;
		carvesolid2d_rect(win, 0x202020, tc, tr, tf);
	}

	tc[0] = vc[0] + vr[0]*3/35 + vf[0]/4;
	tc[1] = vc[1] + vr[1]*3/35 + vf[1]/4;
	tc[2] = vc[2] + vr[2]*3/35 + vf[2]/4;
	tr[0] = vr[0] * 2*52 / 7 / 20;
	tr[1] = vr[1] * 2*52 / 7 / 20;
	tr[2] = vr[2] * 2*52 / 7 / 20;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);

	tr[0] = vr[0] / 64;
	tr[1] = vr[1] / 64;
	tr[2] = vr[2] / 64;
	tf[0] = vf[0] / 64;
	tf[1] = vf[1] / 64;
	tf[2] = vf[2] / 64;
	tc[0] = vc[0] - vr[0]*16/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*16/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*16/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"27.5", 4);
	tc[0] = vc[0] - vr[0]*12/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*12/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*12/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"55", 2);
	tc[0] = vc[0] - vr[0]*8/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*8/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*8/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"110", 3);
	tc[0] = vc[0] - vr[0]*4/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*4/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*4/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"220", 3);
	tc[0] = vc[0] + vf[0]/64;
	tc[1] = vc[1] + vf[1]/64;
	tc[2] = vc[2] + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"440", 3);
	tc[0] = vc[0] + vr[0]*4/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*4/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*4/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"880", 3);
	tc[0] = vc[0] + vr[0]*8/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*8/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*8/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"1760", 4);
	tc[0] = vc[0] + vr[0]*12/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*12/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*12/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"3520", 4);
	tc[0] = vc[0] + vr[0]*16/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*16/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*16/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"7040", 4);
}
static void piano_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	tc[0] = vc[0] + vf[0]/2 + vu[0]/2;
	tc[1] = vc[1] + vf[1]/2 + vu[1]/2;
	tc[2] = vc[2] + vf[2]/2 + vu[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
    carvesolid_prism4(win, 0x202020, tc, vr, tf, tu);

	tr[0] = vr[0] / 7 / 11;
	tr[1] = vr[1] / 7 / 11;
	tr[2] = vr[2] / 7 / 11;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;
	for(x=0;x<7*10;x++)
	{
		tc[0] = vc[0] + vr[0]*(x-34.5)/35 - vf[0]/4 + vu[0]/2;
		tc[1] = vc[1] + vr[1]*(x-34.5)/35 - vf[1]/4 + vu[1]/2;
		tc[2] = vc[2] + vr[2]*(x-34.5)/35 - vf[2]/4 + vu[2]/2;
		carvesolid_rect(win, 0xfffffff, tc, tr, tf);
	}

	tr[0] = vr[0] / 7 / 20;
	tr[1] = vr[1] / 7 / 20;
	tr[2] = vr[2] / 7 / 20;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(x=0;x<7*10;x++)
	{
		if((x%7) == 0)continue;
		if((x%7) == 3)continue;
		tc[0] = vc[0] + vr[0]*(x-35)/35 - vf[0]/8 + vu[0]*0.51;
		tc[1] = vc[1] + vr[1]*(x-35)/35 - vf[1]/8 + vu[1]*0.51;
		tc[2] = vc[2] + vr[2]*(x-35)/35 - vf[2]/8 + vu[2]*0.51;
		carvesolid_rect(win, 0x202020, tc, tr, tf);
	}
}
static void piano_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void piano_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"piano\" style=\"width:50%%;height:100px;float:left;background-color:#48e1a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
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
static void piano_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)piano_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)piano_read_tui(win, sty, act, pin);
	else if(fmt == _html_)piano_read_html(win, sty, act, pin);
	else if(fmt == _json_)piano_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)piano_read_vbo2d(win, sty, act, pin);
		else piano_read_vbo3d(win, sty, act, pin);
	}
	else piano_read_pixel(win, sty, act, pin);
}
static void piano_swrite(
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
	struct perframe* frame;
//say("@piano_write\n");

	if(0 == len){
	if(_char_ == ev->what)
	{
		k = ev->why;
		say("%d\n", k);

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

		frame = act->buf;
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
	}//if(_char_)
	}//if(0==len)
}
static void piano_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void piano_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void piano_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void piano_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void piano_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void piano_create(struct actor* act)
{
	struct arena* win;
	if(0 == act)return;

	act->buf = memorycreate(0x100000);
	if(0 == act->buf)return;

	win = arenacreate(_mic_, "0");
	if(0 == win)return;

	relationcreate(win, 0, _win_, act, 0, _act_);
}




void piano_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'i', 'a', 'n', 'o', 0, 0, 0);

	p->oncreate = (void*)piano_create;
	p->ondelete = (void*)piano_delete;
	p->onstart  = (void*)piano_start;
	p->onstop   = (void*)piano_stop;
	p->onget    = (void*)piano_cread;
	p->onpost   = (void*)piano_cwrite;
	p->onread   = (void*)piano_sread;
	p->onwrite  = (void*)piano_swrite;
}
