#include "libuser.h"
void gl41solid_bodypart(struct entity*, u32, vec3, vec3);




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




static void human_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("human_search\n");

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void human_modify(struct entity* act)
{
}
static void human_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void human_create(struct entity* act)
{
	if(0 == act)return;
	act->fz0 = 0;
}




static void human_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
}
static void human_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,k;
	vec3 t0, t1;
	float w,h;
	float x,y,z,n;
	vec3* bonevert;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_circle(ctx, 0xff00ff, vc, vr, vf);

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

		gl41line(ctx, 0x008080, t0, t1);
	}
}
static void human_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void human_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void human_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void human_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static int human_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	//say("act=%llx\n", act);

	human_search(act, 0, &self, &peer);
	obb = peer->pfoot;
	//say("@human_event:%llx\n", obb);

	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'w':obb->vc[1] += 100;break;
			case 's':obb->vc[1] -= 100;break;
			case 'a':obb->vc[0] -= 100;break;
			case 'd':obb->vc[0] += 100;break;
		}
	}
/*	short* t;
	float x,y,z,n;
	float sec,a,c,s;
	struct relation* rel;
	vec4 tmp;

	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		say("@human_swrite:%x\n", t[2]);
		if(t[3] & joyl_left   )sty->f.vc[0] -= 10;
		if(t[3] & joyl_right  )sty->f.vc[0] += 10;
		if(t[3] & joyl_down   )sty->f.vc[1] -= 10;
		if(t[3] & joyl_up     )sty->f.vc[1] += 10;
		if(t[3] & joyl_trigger)act->z0 = 0;
		if(t[3] & joyl_bumper )act->z0 += 10;
	}
	else return 0;

	//read terrain, fix z
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)break;
		if(_ent_ == rel->srctype)
		{
			tmp[0] = sty->f.vc[0];
			tmp[1] = sty->f.vc[1];
			tmp[2] = 0.0;

			entityread((void*)&rel->srcchip, (void*)&rel->dstchip, tmp, 0);

			sty->f.vc[2] = tmp[2] + act->z0;
			break;
		}
		rel = samedstnextsrc(rel);
	}
	return 1;
*/
/*	sec = timeread() / 1000000.0;

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
	return 1;
}




static int human_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		human_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int human_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
static void human_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void human_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void human_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0);

	p->oncreate = (void*)human_create;
	p->ondelete = (void*)human_delete;
	p->onsearch = (void*)human_search;
	p->onmodify = (void*)human_modify;

	p->onlinkup = (void*)human_linkup;
	p->ondiscon = (void*)human_discon;
	p->onread  = (void*)human_read;
	p->onwrite = (void*)human_write;
}
