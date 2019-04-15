#include "libuser.h"
void carvesolid_bodypart(struct arena*, u32, vec3, vec3);




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
static vec3 boneverta[15] = {
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
static vec3 bonevertb[15] = {
	{ 0.0, 0.0, 1.0},	//00.head
	{ 0.0, 0.0, 0.8},	//01.neck
	{ 0.0, 0.0, 0.5},	//02.center
	{-0.2, 0.0, 0.8},	//03.scapula l
	{ 0.2, 0.0, 0.8},	//04.scapula r
	{-0.3,-0.1, 0.6},	//05.elbow l
	{ 0.3, 0.1, 0.6},	//06.elbow r
	{-0.3,-0.3, 0.5},	//07.hand l
	{ 0.3, 0.3, 0.5},	//08.hand r
	{-0.2, 0.0, 0.5},	//09.hipbone l
	{ 0.2, 0.0, 0.5},	//10.hipbone r
	{-0.2, 0.1, 0.3},	//11.knee l
	{ 0.2,-0.1, 0.3},	//12.knee r
	{-0.2, 0.3, 0.0},	//13.foot l
	{ 0.2,-0.3, 0.0}	//14.foot r
};
static vec3 bonevertc[15] = {
	{ 0.0, 0.0, 1.0},	//00.head
	{ 0.0, 0.0, 0.8},	//01.neck
	{ 0.0, 0.0, 0.5},	//02.center
	{-0.2, 0.0, 0.8},	//03.scapula l
	{ 0.2, 0.0, 0.8},	//04.scapula r
	{-0.3, 0.0, 0.6},	//05.elbow l
	{ 0.3, 0.0, 0.6},	//06.elbow r
	{-0.3, 0.0, 0.5},	//07.hand l
	{ 0.3, 0.0, 0.5},	//08.hand r
	{-0.2, 0.0, 0.5},	//09.hipbone l
	{ 0.2, 0.0, 0.5},	//10.hipbone r
	{-0.2, 0.0, 0.3},	//11.knee l
	{ 0.2, 0.0, 0.3},	//12.knee r
	{-0.2, 0.0, 0.0},	//13.foot l
	{ 0.2, 0.0, 0.0}	//14.foot r
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
	float w,h;
	float x,y,z,n;
	vec3* bonevert;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carveline_circle(win, 0xff00ff, vc, vr, vf);

	j = (timeread()%1000000);
	if(j < 250*1000){
		bonevert = boneverta;
	}
	else if(j < 500*1000){
		bonevert = bonevertc;
	}
	else if(j < 750*1000){
		bonevert = bonevertb;
	}
	else{
		bonevert = bonevertc;
	}

	for(j=0;j<16;j++)
	{
		k = bonepair[j][0];
		x = bonevert[k][0];
		y = bonevert[k][1];
		z = bonevert[k][2];
		t0[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		t0[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		t0[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;
		k = bonepair[j][1];
		x = bonevert[k][0];
		y = bonevert[k][1];
		z = bonevert[k][2];
		t1[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
		t1[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
		t1[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;

		carvesolid_bodypart(win, 0x008080, t0, t1);
	}

	x = bonevert[0][0];
	y = bonevert[0][1];
	z = bonevert[0][2];
	act->camera.vc[0] = vc[0] + vr[0]*x + vf[0]*y + vu[0]*z;
	act->camera.vc[1] = vc[1] + vr[1]*x + vf[1]*y + vu[1]*z;
	act->camera.vc[2] = vc[2] + vr[2]*x + vf[2]*y + vu[2]*z;

	x = vf[0];
	y = vf[1];
	z = vf[2];
	n = squareroot(x*x + y*y + z*z);
	act->camera.vn[0] = 100*x/n;
	act->camera.vn[1] = 100*y/n;
	act->camera.vn[2] = 100*z/n;

	//
	w = win->width;
	h = win->height;
	x = bonevert[0][0] - bonevert[1][0];
	y = bonevert[0][1] - bonevert[1][1];
	z = bonevert[0][2] - bonevert[1][2];
	n = h/w*173.0 / squareroot(x*x + y*y + z*z);
	act->camera.vu[0] = x*n;
	act->camera.vu[1] = y*n;
	act->camera.vu[2] = z*n;
	act->camera.vb[0] =-x*n;
	act->camera.vb[1] =-y*n;
	act->camera.vb[2] =-z*n;

	//right = cross(near, up)
	vf = act->camera.vn;
	vu = act->camera.vu;
	x = vf[1] * vu[2] - vf[2] * vu[1];
	y = vf[2] * vu[0] - vf[0] * vu[2];
	z = vf[0] * vu[1] - vf[1] * vu[0];
	n = 173.0 / squareroot(x*x + y*y + z*z);
	act->camera.vr[0] = x*n;
	act->camera.vr[1] = y*n;
	act->camera.vr[2] = z*n;
	act->camera.vl[0] =-x*n;
	act->camera.vl[1] =-y*n;
	act->camera.vl[2] =-z*n;
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
static void human_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)human_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)human_read_tui(win, sty, act, pin);
	else if(fmt == _html_)human_read_html(win, sty, act, pin);
	else if(fmt == _json_)human_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)human_read_vbo(win, sty, act, pin);
	else human_read_pixel(win, sty, act, pin);
}
static void human_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	float x,y,z,n;
	float sec,a,c,s;
	struct relation* rel;
	struct actor* map;
	vec4 tmp;

	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'w':sty->vc[1] += 1000;break;
			case 's':sty->vc[1] -= 1000;break;
			case 'a':sty->vc[0] -= 1000;break;
			case 'd':sty->vc[0] += 1000;break;
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		say("@human_swrite:%x\n", t[2]);
		if(t[3] & joyl_left   )sty->vc[0] -= 10;
		if(t[3] & joyl_right  )sty->vc[0] += 10;
		if(t[3] & joyl_down   )sty->vc[1] -= 10;
		if(t[3] & joyl_up     )sty->vc[1] += 10;
		if(t[3] & joyl_trigger)act->z0 = 0;
		if(t[3] & joyl_bumper )act->z0 += 10;
	}
	else return;

	//read terrain, fix z
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)break;
		if(_act_ == rel->srctype)
		{
			tmp[0] = sty->vc[0];
			tmp[1] = sty->vc[1];
			tmp[2] = 0.0;

			map = (void*)(rel->srcchip);
			actor_leafread(map, 0, act, 0, tmp, 0);

			sty->vc[2] = tmp[2] + act->z0;
			break;
		}
		rel = samedstnextsrc(rel);
	}

/*		sec = timeread() / 1000000.0;

		x = boneverta[0][0] - boneverta[1][0];
		y = boneverta[0][1] - boneverta[1][1];
		z = boneverta[0][2] - boneverta[1][2];
		n = squareroot(x*x + y*y + z*z);

		a = PI/90*sine(2.0*PI*sec);
		c = cosine(a);
		s = sine(a);
		boneverta[0][0] = boneverta[1][0] + n*s;
		boneverta[0][1] = 0.0;
		boneverta[0][2] = boneverta[1][2] + n*c;

		//arm
		a = PI/3*sine(2.0*PI*sec);
		c = cosine(a);
		s = sine(a);
		boneverta[5][1] = -s * 0.3;
		boneverta[5][2] = 0.6 - c * 0.3;
		boneverta[6][1] = s * 0.3;
		boneverta[6][2] = 0.6 - c * 0.3;

		a = PI/3*sine(2.0*PI*sec);
		c = cosine(a);
		s = sine(a);
		boneverta[7][1] = boneverta[5][1] - s * 0.3;
		boneverta[7][2] = boneverta[5][2] - c * 0.3;
		boneverta[8][1] = boneverta[6][1] + s * 0.3;
		boneverta[8][2] = boneverta[6][2] - c * 0.3;

		//leg
		a = PI/3*sine(2.0*PI*sec);
		c = cosine(a);
		s = sine(a);
		if(a > 0.0)
		{
			boneverta[11][1] =  s * 0.5;
			boneverta[11][2] = -c * 0.5;
			boneverta[12][1] = 0.0;
			boneverta[12][2] = -0.5;
		}
		else
		{
			boneverta[11][1] = 0.0;
			boneverta[11][2] = -0.5;
			boneverta[12][1] = -s * 0.5;
			boneverta[12][2] = -c * 0.5;
		}
		boneverta[13][1] = boneverta[11][1];
		boneverta[13][2] = boneverta[11][2] - 0.5;
		boneverta[14][1] = boneverta[12][1];
		boneverta[14][2] = boneverta[12][2] - 0.5;
*/
}
static void human_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void human_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void human_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void human_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
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
	act->z0 = 0;
}




void human_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0);

	p->oncreate = (void*)human_create;
	p->ondelete = (void*)human_delete;
	p->onstart  = (void*)human_start;
	p->onstop   = (void*)human_stop;
	p->oncread  = (void*)human_cread;
	p->oncwrite = (void*)human_cwrite;
	p->onsread  = (void*)human_sread;
	p->onswrite = (void*)human_swrite;
}
