#include "libuser.h"
void carvesolid_bodypart(struct arena*, u32, vec3, vec3);




static vec3 bonenode[15] = {
	{ 0.0, 0.0, 1.0},	//00.head
	{ 0.0, 0.0, 0.8},	//01.neck
	{ 0.0, 0.0, 0.5},	//02.center
	{-0.2, 0.0, 0.8},	//03.scapula l
	{ 0.2, 0.0, 0.8},	//04.scapula r
	{-0.3, 0.1, 0.6},	//05.elbow l
	{ 0.3,-0.1, 0.6},	//06.elbow r
	{-0.3, 0.3, 0.5},	//07.hand l
	{ 0.3,-0.3, 0.5},	//08.hand r
	{-0.2, 0.0, 0.5},	//09.hipbone l
	{ 0.2, 0.0, 0.5},	//10.hipbone r
	{-0.2,-0.1, 0.3},	//11.knee l
	{ 0.2, 0.1, 0.3},	//12.knee r
	{-0.2,-0.3, 0.0},	//13.foot l
	{ 0.2, 0.3, 0.0}	//14.foot r
};
static u8 bonepair[16][2] = {
	{ 0,  1},	//00.neck
	{ 1,  2},	//01.body
	{ 3,  4},	//02.shoulder
	{ 9, 10},	//03.hipbone
	{ 1,  3},	//04.l shoulder
	{ 1,  4},	//05.r shoulder
	{ 3,  5},	//06.l upper arm
	{ 5,  7},	//07.l fore arm
	{ 4,  6},	//08.r upper arm
	{ 6,  8},	//09.r fore arm
	{ 2,  9},	//10.l butt
	{ 2, 10},	//11.r butt
	{ 9, 11},	//12.l thigh
	{11, 13},	//13.l shank
	{10, 12},	//14.r thigh
	{12, 14}	//15.r shank
};




static void human_read_pixel(
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
}
static void human_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j,k;
	vec3 t0, t1;
	float x,y,z;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x808080, vc, vr, vf);

	for(j=0;j<16;j++)
	{
		k = bonepair[j][0];
		x = bonenode[k][0];
		y = bonenode[k][1];
		z = bonenode[k][2];
		t0[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		t0[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		t0[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;
		k = bonepair[j][1];
		x = bonenode[k][0];
		y = bonenode[k][1];
		z = bonenode[k][2];
		t1[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		t1[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		t1[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;

		carvesolid_bodypart(win, 0xffffff, t0, t1);
	}
}
static void human_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)human_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)human_read_tui(win, sty, act, pin);
	else if(fmt == _html_)human_read_html(win, sty, act, pin);
	else if(fmt == _json_)human_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)human_read_vbo(win, sty, act, pin);
	else human_read_pixel(win, sty, act, pin);
}
static void human_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	float a,c,s;
	float sec = (timeread() % 2000000) / 2000000.0;
/*
	//arm
	a = PI/3*sine(2.0*PI*sec);
	c = cosine(a);
	s = sine(a);
	bonenode[5][1] = -s * 0.3;
	bonenode[5][2] = 0.6 - c * 0.3;
	bonenode[6][1] = s * 0.3;
	bonenode[6][2] = 0.6 - c * 0.3;

	a = PI/3*sine(2.0*PI*sec);
	c = cosine(a);
	s = sine(a);
	bonenode[7][1] = bonenode[5][1] - s * 0.3;
	bonenode[7][2] = bonenode[5][2] - c * 0.3;
	bonenode[8][1] = bonenode[6][1] + s * 0.3;
	bonenode[8][2] = bonenode[6][2] - c * 0.3;

	//leg
	a = PI/3*sine(2.0*PI*sec);
	c = cosine(a);
	s = sine(a);
	if(a > 0.0)
	{
		bonenode[11][1] =  s * 0.5;
		bonenode[11][2] = -c * 0.5;
		bonenode[12][1] = 0.0;
		bonenode[12][2] = -0.5;
	}
	else
	{
		bonenode[11][1] = 0.0;
		bonenode[11][2] = -0.5;
		bonenode[12][1] = -s * 0.5;
		bonenode[12][2] = -c * 0.5;
	}
	bonenode[13][1] = bonenode[11][1];
	bonenode[13][2] = bonenode[11][2] - 0.5;
	bonenode[14][1] = bonenode[12][1];
	bonenode[14][2] = bonenode[12][2] - 0.5;
*/
}
static void human_get()
{
}
static void human_post()
{
}
static void human_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void human_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void human_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void human_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0);

	p->oncreate = (void*)human_create;
	p->ondelete = (void*)human_delete;
	p->onstart  = (void*)human_start;
	p->onstop   = (void*)human_stop;
	p->onget    = (void*)human_get;
	p->onpost   = (void*)human_post;
	p->onread   = (void*)human_read;
	p->onwrite  = (void*)human_write;
}
