#include "libuser.h"
void quaternion_rotatefrom(float* o, float* v, float* q);
void gl41solid_bodypart(_obj*, u32, vec3, vec3);
void quaternion_multiplyfrom(float* o, float* l, float* r);



/*
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

static void human_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j,k;
	vec3 t0, t1;
	float w,h;
	float x,y,z,n;
	vec3* bonevert;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_circle(ctx, 0xff00ff, vc, vr, vf);

	j = (timeread_us()%1000000);
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
*/



static void human_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
	struct fstyle* obb = 0;
	//logtoall("human_search\n");

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
static void human_modify(_obj* act)
{
}
static void human_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void human_create(_obj* act)
{
	if(0 == act)return;
	act->whdf.fz0 = 0;
}




static void human_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	vec3 tc,tt;
	int j;

	for(j=0;j<3;j++){
		tt[j] = vt[j]/2;
		tc[j] = vc[j]+tt[j];
	}
	gl41solid_prism4(ctx, 0xc000c0, tc, vr, vf, tt);

	for(j=0;j<3;j++){
		tt[j] = vt[j]/2;
		tc[j] = vc[j]-tt[j];
	}
	gl41solid_prism4(ctx, 0xff00ff, tc, vr, vf, tt);
}
static void human_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void human_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void human_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void human_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void human_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}



/*
static int human_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	//logtoall("act=%llx\n", act);

	human_search(act, 0, &self, &peer);
	obb = peer->pfoot;
	//logtoall("@human_event:%llx\n", obb);

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
	short* t;
	float x,y,z,n;
	float sec,a,c,s;
	struct relation* rel;
	vec4 tmp;

	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		logtoall("@human_swrite:%x\n", t[2]);
		if(t[3] & joyl_left   )sty->fs.vc[0] -= 10;
		if(t[3] & joyl_right  )sty->fs.vc[0] += 10;
		if(t[3] & joyl_down   )sty->fs.vc[1] -= 10;
		if(t[3] & joyl_up     )sty->fs.vc[1] += 10;
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
			tmp[0] = sty->fs.vc[0];
			tmp[1] = sty->fs.vc[1];
			tmp[2] = 0.0;

			entityread((void*)&rel->srcchip, (void*)&rel->dstchip, tmp, 0);

			sty->fs.vc[2] = tmp[2] + act->z0;
			break;
		}
		rel = samedstnextsrc(rel);
	}
	return 1;
*/
/*	sec = timeread_us() / 1000000.0;

	x = boneverta[0][0] - boneverta[1][0];
	y = boneverta[0][1] - boneverta[1][1];
	z = boneverta[0][2] - boneverta[1][2];
	n = squareroot(x*x + y*y + z*z);

	a = PI/90*getsin(2.0*PI*sec);
	c = getcos(a);
	s = getsin(a);
	boneverta[0][0] = boneverta[1][0] + n*s;
	boneverta[0][1] = 0.0;
	boneverta[0][2] = boneverta[1][2] + n*c;

	//arm
	a = PI/3*getsin(2.0*PI*sec);
	c = getcos(a);
	s = getsin(a);
	boneverta[5][1] = -s * 0.3;
	boneverta[5][2] = 0.6 - c * 0.3;
	boneverta[6][1] = s * 0.3;
	boneverta[6][2] = 0.6 - c * 0.3;

	a = PI/3*getsin(2.0*PI*sec);
	c = getcos(a);
	s = getsin(a);
	boneverta[7][1] = boneverta[5][1] - s * 0.3;
	boneverta[7][2] = boneverta[5][2] - c * 0.3;
	boneverta[8][1] = boneverta[6][1] + s * 0.3;
	boneverta[8][2] = boneverta[6][2] - c * 0.3;

	//leg
	a = PI/3*getsin(2.0*PI*sec);
	c = getcos(a);
	s = getsin(a);
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

	return 1;
}
*/



static void human_read_traverse(_obj* ent,_obj* node, _syn* stack,int sp, float* v0, vec4 quat)
{
	logtoall("ent=%p,node=%p\n", ent, node);
	int j;
	struct style tmp;
	//
	vec4 q1, q2, rq;
	vec3 v1, v2, rv;
	//
	struct style* part;
	struct style* bias;

	struct relation* rel = node->orel0;
	_obj* child;
	while(rel){
		if(rel->pdstchip){
			bias = rel->pdstfoot;
			part = rel->psrcfoot;

			//local space vector
			tmp.fs.vr[0] = 1.0;tmp.fs.vr[1] = 0.0;tmp.fs.vr[2] = 0.0;
			tmp.fs.vf[0] = 0.0;tmp.fs.vf[1] = 1.0;tmp.fs.vf[2] = 0.0;
			tmp.fs.vt[0] = 0.0;tmp.fs.vt[1] = 0.0;tmp.fs.vt[2] = 1.0;
			vec3_setlen(tmp.fs.vr, 0.1);
			vec3_setlen(tmp.fs.vf, 0.1);
			vec3_setlen(tmp.fs.vt, 0.02);

			//rotation
			quaternion_multiplyfrom(q1, quat, part->fshape.vq);
			for(j=0;j<4;j++)rq[j] = bias->fshape.vq[j];
			rq[3] = -rq[3];
			quaternion_multiplyfrom(q2, q1, rq);

			//translation
			quaternion_rotatefrom(v1, part->fs.vc, quat);
			for(j=0;j<3;j++)rv[j] = - bias->fs.vc[j];
			quaternion_rotatefrom(v2, rv, q2);
			/*
			logtoall("%f,%f,%f    %f,%f,%f\n",
				v1[0], v1[1], v1[2],
				v2[0], v2[1], v2[2]
			);
			*/

			//world space vector
			for(j=0;j<3;j++)tmp.fs.vc[j] = v0[j] + v1[j] + v2[j];
			quaternion_rotate(tmp.fs.vr, q2);
			quaternion_rotate(tmp.fs.vf, q2);
			quaternion_rotate(tmp.fs.vt, q2);

			logtoall("%f,%f,%f    %f,%f,%f    %f,%f,%f,    %f,%f,%f\n",
				tmp.fs.vr[0], tmp.fs.vr[1], tmp.fs.vr[2],
				tmp.fs.vf[0], tmp.fs.vf[1], tmp.fs.vf[2],
				tmp.fs.vt[0], tmp.fs.vt[1], tmp.fs.vt[2],
				tmp.fs.vc[0], tmp.fs.vc[1], tmp.fs.vc[2]
			);
			

			//call child
			stack[sp+0].pchip = ent;		//node=human
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			child = rel->pdstchip;
			//logtoall("%p %p %p %d    %p\n", child, rel->dst, stack, sp+2, child->ontaking);
			//printmemory(&stack[sp], 0x80);
			child->ontaking(child, rel->dst, stack, sp+2, 0, 0, &tmp, 0);
			//logtoall("-----%p %p %p %d    %p\n", child, rel->dst, stack, sp+2, child->ontaking);

			//call child's child
			if(rel->pdstchip)human_read_traverse(ent,rel->pdstchip, stack,sp, tmp.fs.vc, q2);
		}
		rel = samesrcnextdst(rel);
	}
}
static void human_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	//self
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	human_draw_gl41(ent,slot, wor,geom, wnd,area);

	//child
	vec4 q = {0, 0, 0, 1};
	human_read_traverse(ent,ent, stack,sp, geom->fs.vc, q);
	logtoall("\n\n\n\n", ent);
}




static void human_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		human_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static int human_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
static void human_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void human_attach(struct halfrel* self, struct halfrel* peer)
{
}




void human_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('h', 'u', 'm', 'a', 'n', 0, 0, 0);

	p->oncreate = (void*)human_create;
	p->ondelete = (void*)human_delete;
	p->onreader = (void*)human_search;
	p->onwriter = (void*)human_modify;

	p->onattach = (void*)human_attach;
	p->ondetach = (void*)human_detach;
	p->ontaking = (void*)human_taking;
	p->ongiving = (void*)human_giving;
}
