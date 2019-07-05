#include "libuser.h"
void fixmatrix(float* m, struct fstyle* sty);




/*
void thridperson_fixcam(struct arena* win, float* v)
{
	float w,h;
	float x,y,z,n;

	//near = -v
	n = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	win->camera.vn[0] = -v[0] / n;
	win->camera.vn[1] = -v[1] / n;
	win->camera.vn[2] = -v[2] / n;

	//right = cross(near, (0,0,1))
	x = -v[1]*1 + v[2]*0;
	y = -v[2]*0 + v[0]*1;
	z = -v[0]*0 + v[1]*0;
	n = 1.0 / squareroot(x*x + y*y + z*z);
	win->camera.vr[0] = x * n;
	win->camera.vr[1] = y * n;
	win->camera.vr[2] = z * n;
	win->camera.vl[0] =-x * n;
	win->camera.vl[1] =-y * n;
	win->camera.vl[2] =-z * n;

	//w,h
	w = win->width;
	h = win->height;

	//up = cross(right, near)
	x = win->camera.vr[1]*win->camera.vn[2] - win->camera.vr[2]*win->camera.vn[1];
	y = win->camera.vr[2]*win->camera.vn[0] - win->camera.vr[0]*win->camera.vn[2];
	z = win->camera.vr[0]*win->camera.vn[1] - win->camera.vr[1]*win->camera.vn[0];
	n = h / w / squareroot(x*x + y*y + z*z);
	win->camera.vt[0] = x * n;
	win->camera.vt[1] = y * n;
	win->camera.vt[2] = z * n;
	win->camera.vb[0] =-x * n;
	win->camera.vb[1] =-y * n;
	win->camera.vb[2] =-z * n;
}
static int thirdperson_sread000(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int j;
	struct relation* rel;
	struct actor* tar;
	struct fstyle* tmpsty;

	//3rdcam's target
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_act_ == rel->srctype)
		{
			tar = (void*)(rel->srcchip);
			if(0 == tar)return 0;
			else goto found;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

found:
	//target's world
	rel = tar->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_win_ == rel->srctype)
		{
			tmpsty = (void*)(rel->srcfoot);

			//camera.xyz = target.top + act.vector
			win->camera.vc[0] = tmpsty->vc[0] + tmpsty->vt[0] + act->camera.vc[0];
			win->camera.vc[1] = tmpsty->vc[1] + tmpsty->vt[1] + act->camera.vc[1];
			win->camera.vc[2] = tmpsty->vc[2] + tmpsty->vt[2] + act->camera.vc[2];
			thridperson_fixcam(win, act->camera.vc);
			goto print;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

print:
	carvefrustum(win, &win->camera);
	return 0;
}*/
static int thirdperson_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	carvesolid_prism4(win, 0x008000, tc, vr, vu, tf);

	//
	//thirdperson_sread000(act, pin, win, sty);
	return 0;
}




void thirdperson_fixgeom(struct fstyle* d, struct fstyle* s, struct fstyle* delta)
{
	d->vc[0] = (s->vc[0] + s->vt[0]) - delta->vf[0];
	d->vc[1] = (s->vc[1] + s->vt[1]) - delta->vf[1];
	d->vc[2] = (s->vc[2] + s->vt[2]) - delta->vf[2];

	d->vf[0] = delta->vf[0];
	d->vf[1] = delta->vf[1];
	d->vf[2] = delta->vf[2];

	//right = cross(front, (0,0,1))
	d->vr[0] = d->vf[1];
	d->vr[1] =-d->vf[0];
	d->vr[2] = 0.0;

	//top = cross(right, front)
	d->vt[0] = d->vr[1] * d->vf[2] - d->vr[2] * d->vf[1];
	d->vt[1] = d->vr[2] * d->vf[0] - d->vr[0] * d->vf[2];
	d->vt[2] = d->vr[0] * d->vf[1] - d->vr[1] * d->vf[0];
}
void thirdperson_frustum(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	//d->vr[3] = 0.7;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -0.7;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 0.7;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -0.7;


	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	//d->vn[3] = 1.0;
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 1e20;
}




void thirdperson_findroot(struct actor* act, struct arena** r, struct fstyle** s)
{
#define _geom_ hex32('g','e','o','m')
	struct relation* rel = act->irel0;
	while(1){
		if(0 == rel)return;
		if(_geom_ == rel->dstflag){
			*s = (void*)(rel->srcfoot);
			*r = (void*)(rel->srcchip);
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
void thirdperson_findtarget(struct actor* act, struct fstyle** delta, struct fstyle** s)
{
#define _tar_ hex32('t','a','r',0)
	struct relation* rel;
	struct actor* tar;

	rel = act->orel0;
	while(1){
		if(0 == rel)return;
		if(_tar_ == rel->srcflag){
			tar = (void*)(rel->dstchip);
			*delta = (void*)(rel->srcfoot);
			break;
		}
		rel = samesrcnextdst(rel);
	}
	if(0 == tar)return;


	rel = tar->irel0;
	while(1){
		if(0 == rel)return;
		if(_geom_ == rel->dstflag){
			*s = (void*)(rel->srcfoot);
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void thirdperson_matrix(
	struct actor* act, struct style* frustum,
	struct arena* win, struct style* wingeom,
	u8* buf, int len)
{
	struct arena* root;
	struct fstyle* geom;
	struct fstyle* target;
	struct fstyle* delta;

	thirdperson_findroot(act, &root, &geom);
	thirdperson_findtarget(act, &delta, &target);
	say("%llx,%llx,%llx,%llx\n", root, geom, delta, target);

	float* m = act->buf;
	thirdperson_fixgeom(geom, target, delta);
	thirdperson_frustum(&frustum->f, geom);
	fixmatrix(m, &frustum->f);
	mat4_transpose((void*)m);
	//printmat4(m);


	u64* p = (void*)buf;
	struct glsrc* src = (void*)(buf+0x20);

	p[0] = (u64)src;
	p[1] = (u64)root->gl_light;
	p[2] = (u64)root->gl_solid;
	p[3] = (u64)root->gl_opaque;


	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = m;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = frustum->f.vc;
}




static int thirdperson_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	vec4 v,q;
	float a,c,s;
	int x0,y0,x1,y1,id;
	if(0 == act)return 0;

	struct fstyle* target;
	struct fstyle* delta;
	thirdperson_findtarget(act, &delta, &target);

	if(_char_ == ev->what){
		switch(ev->why)
		{
			case 'a':a = 0.01;break;
			case 'd':a =-0.01;break;
			default:return 0;
		}

		s = sine(a);
		c = cosine(a);
		v[0] = delta->vf[0];
		v[1] = delta->vf[1];
		delta->vf[0] = v[0]*c - v[1]*s;
		delta->vf[1] = v[0]*s + v[1]*c;
	}
	if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)
		{
			delta->vf[0] *= 0.99;
			delta->vf[1] *= 0.99;
			delta->vf[2] *= 0.99;
			return 1;
		}
		if('b' == id)
		{
			delta->vf[0] *= 1.01;
			delta->vf[1] *= 1.01;
			delta->vf[2] *= 1.01;
			return 1;
		}
		if('r' == id)
		{
			return 0;
		}
	}
/*
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		x0 = win->input[id].xn;
		y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;

		//rotate y
		v[0] = act->camera.vc[0];
		v[1] = act->camera.vc[1];
		v[2] = act->camera.vc[2];
		q[0] =-v[1];
		q[1] = v[0];
		q[2] = 0.0;
		quaternion_operation(v, q, (y0-y1)/100.0);

		//dot(cross(va,n), cross(vb,n))
		if(v[0]*act->camera.vc[0] + v[1]*act->camera.vc[1] > 0){
			act->camera.vc[0] = v[0];
			act->camera.vc[1] = v[1];
			act->camera.vc[2] = v[2];
		}

		//rotate x
		a = (x0-x1)/100.0;
		s = sine(a);
		c = cosine(a);
		v[0] = act->camera.vc[0];
		v[1] = act->camera.vc[1];
		act->camera.vc[0] = v[0]*c - v[1]*s;
		act->camera.vc[1] = v[0]*s + v[1]*c;
		return 1;
	}
*/
	return 0;
}




static void thirdperson_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	switch(self->flag){
		case _cam_:thirdperson_matrix(act, pin, win, sty, buf, len);break;
	}
}
static int thirdperson_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return thirdperson_event(act, pin, win, sty, ev, 0);
}
static void thirdperson_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void thirdperson_start(struct halfrel* self, struct halfrel* peer)
{
}




static void thirdperson_search(struct actor* act)
{
}
static void thirdperson_modify(struct actor* act)
{
}
static void thirdperson_delete(struct actor* act)
{
}
static void thirdperson_create(struct actor* act, void* str)
{
	if(0 == act)return;
	act->buf = memorycreate(64);
}




void thirdperson_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('3', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)thirdperson_create;
	p->ondelete = (void*)thirdperson_delete;
	p->onsearch = (void*)thirdperson_search;
	p->onmodify = (void*)thirdperson_modify;

	p->onstart = (void*)thirdperson_start;
	p->onstop  = (void*)thirdperson_stop;
	p->onread  = (void*)thirdperson_read;
	p->onwrite = (void*)thirdperson_write;
}
