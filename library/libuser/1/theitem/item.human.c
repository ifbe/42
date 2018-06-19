#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
static vec3 bonenode[15] = {
	{  0.0, 0.0, 0.8},	//00.head
	{  0.0, 0.0, 0.6},	//01.neck
	{  0.0, 0.0, 0.0},	//02.center
	{-0.25, 0.0, 0.6},	//03.scapula l
	{ 0.25, 0.0, 0.6},	//04.scapula r
	{-0.25, 0.0, 0.0},	//05.hipbone l
	{ 0.25, 0.0, 0.0},	//06.hipbone r
	{-0.25, 0.1, 0.3},	//07.elbow l
	{ 0.25,-0.1, 0.3},	//08.elbow r
	{-0.25, 0.3, 0.0},	//09.hand l
	{ 0.25,-0.3, 0.0},	//10.hand r
	{-0.25,-0.1, -0.4},	//11.knee l
	{ 0.25, 0.1, -0.4},	//12.knee r
	{-0.25,-0.3, -0.8},	//13.foot l
	{ 0.25, 0.3, -0.8}	//14.foot r
};
static u8 bonepair[14][2] = {
	{ 0,  1},	//00.neck
	{ 1,  2},	//01.body
	{ 1,  3},	//02.l shoulder
	{ 1,  4},	//03.r shoulder
	{ 2,  5},	//04.l butt
	{ 2,  6},	//05.r butt
	{ 3,  7},	//06.l upper arm
	{ 7,  9},	//07.l fore arm
	{ 4,  8},	//08.r upper arm
	{ 8, 10},	//09.r fore arm
	{ 5, 11},	//10.l thigh
	{11, 13},	//11.l shank
	{ 6, 12},	//12.r thigh
	{12, 14}	//13.r shank
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
	float x,y,z;
	vec3 tc, tr, tf, tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_prism4(win, 0x6a4b23, vc, vr, vf, vu);

	for(j=0;j<14;j++)
	{
		k = bonepair[j][0];
		x = bonenode[k][0];
		y = bonenode[k][1];
		z = bonenode[k][2];
		tc[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		tc[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		tc[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;
		k = bonepair[j][1];
		x = bonenode[k][0];
		y = bonenode[k][1];
		z = bonenode[k][2];
		tr[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		tr[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		tr[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;
		carveline(win, 0xffffff, tc, tr);
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
}
static void human_list()
{
}
static void human_change()
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
	p->onlist   = (void*)human_list;
	p->onchoose = (void*)human_change;
	p->onread   = (void*)human_read;
	p->onwrite  = (void*)human_write;
}
