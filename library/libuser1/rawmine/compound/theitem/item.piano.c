#include "libuser.h"
#define _mic_ hex32('m','i','c',0)
#define cccc 440.000
#define c__d 466.164
#define dddd 493.883
#define d__e 523.251
#define eeee 554.365
#define ffff 587.330
#define f__g 622.254
#define gggg 659.255
#define g__a 698.456
#define aaaa 739.989
#define a__b 783.991
#define bbbb 830.609
int soundread(int dev, int time, void* buf, int len);
int soundwrite(int dev, int time, void* buf, int len);



void piano_gen(short* pcm, float f)
{
	int j;
	for(j=0;j<16384;j++)
	{
		pcm[j] = 16384*sine(j*2*PI*f*1/44100)
				+ 8192*sine(j*2*PI*f*2/44100)
				+ 1024*sine(j*2*PI*f*3/44100)
				+ 4096*sine(j*2*PI*f*4/44100)
				+ 1024*sine(j*2*PI*f*5/44100);

		pcm[j] *= 1.0 - j/16384.0;
	}
}




static void piano_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx, cy, ww, hh;
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
}/*
static void piano_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x;
	vec3 tc,tr,tf,tu;
	if(0 == sty)sty = defaultstyle_vbo2d();
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

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
}*/
static void piano_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
static void piano_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void piano_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void piano_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void piano_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("piano(%x,%x,%x)\n",win,act,sty);
}
static void piano_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)piano_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)piano_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)piano_draw_html(act, pin, win, sty);
	else if(fmt == _json_)piano_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)piano_draw_vbo2d(act, pin, win, sty);
		//else piano_draw_vbo3d(act, pin, win, sty);
	}
	else piano_draw_pixel(act, pin, win, sty);
}
static void piano_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int j,k;
	float f;

	if(0 == len){
	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'c':f = cccc;break;
			case 'd':f = dddd;break;
			case 'e':f = eeee;break;
			case 'f':f = ffff;break;
			case 'g':f = gggg;break;
			case 'a':f = aaaa;break;
			case 'b':f = bbbb;break;
			case '1':f = cccc;break;
			case '2':f = c__d;break;
			case '3':f = dddd;break;
			case '4':f = d__e;break;
			case '5':f = eeee;break;
			case '6':f = ffff;break;
			case '7':f = f__g;break;
			case '8':f = gggg;break;
			case '9':f = g__a;break;
			case '0':f = aaaa;break;
			case '-':f = a__b;break;
			case '=':f = bbbb;break;
		}

		piano_gen(act->buf, f);
		soundwrite(0, 0, act->buf, 16384*2);
	}//if(_char_)

	if(0x2d70 == ev->what){
		j = (ev->why)&0xffff;
		j = j*70/(win->width);
		say("j=%d\n",j);

		k = j%7;
		j = j/7;
		switch(k)
		{
			case 0:f = cccc;break;
			case 1:f = dddd;break;
			case 2:f = eeee;break;
			case 3:f = ffff;break;
			case 4:f = gggg;break;
			case 5:f = aaaa;break;
			case 6:f = bbbb;break;
		}
		switch(j)
		{
			case 0:f /= 32;break;
			case 1:f /= 16;break;
			case 2:f /= 8;break;
			case 3:f /= 4;break;
			case 4:f /= 2;break;
			case 5:break;
			case 6:f *= 2;break;
			case 7:f *= 4;break;
			case 8:f *= 8;break;
			case 9:f *= 16;break;
		}
		say("f=%f\n",f);

		piano_gen(act->buf, f);
		soundwrite(0, 0, act->buf, 16384*2);
	}//if(mouseup)

	}//if(0==len)
}




static void piano_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//piano_draw(act, pin, win, sty);
}
static void piano_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//piano_event(act, pin, win, sty, ev, 0);
}
static void piano_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void piano_start(struct halfrel* self, struct halfrel* peer)
{
}




static void piano_search(struct actor* act)
{
}
static void piano_modify(struct actor* act)
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

	act->buf = memorycreate(0x100000, 0);
	if(0 == act->buf)return;
}




void piano_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'i', 'a', 'n', 'o', 0, 0, 0);

	p->oncreate = (void*)piano_create;
	p->ondelete = (void*)piano_delete;
	p->onsearch = (void*)piano_search;
	p->onmodify = (void*)piano_modify;

	p->onstart = (void*)piano_start;
	p->onstop  = (void*)piano_stop;
	p->onread  = (void*)piano_read;
	p->onwrite = (void*)piano_write;
}
