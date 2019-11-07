#include "libuser.h"
#define _tar_ hex32('t','a','r',0)
void fixmatrix(float* m, struct fstyle* sty);
void freecam_shape2frustum(struct fstyle* d, struct fstyle* s);




static void thirdperson_search(struct actor* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct actor* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	if(_tar_ == foot){
		while(1){
			if(0 == rel)return;
			if(_tar_ == rel->dstflag){
				self[0] = (void*)&rel->dstchip;
				peer[0] = (void*)&rel->srcchip;
				return;
			}
			rel = samedstnextsrc(rel);
		}
		return;
	}

	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
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
	act->buf = memorycreate(64, 0);
}




static int thirdperson_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	return 0;
}




static void thirdperson_fixcam(struct fstyle* obb, struct fstyle* tar, int dx, int dy, int roll)
{
	float a,c,s;
	float q[4];		//quaternion
	float t[4];		//target
	float v[4];		//tar to eye
	t[0] = tar->vc[0]+tar->vt[0];
	t[1] = tar->vc[1]+tar->vt[1];
	t[2] = tar->vc[2]+tar->vt[2];
	v[0] = obb->vc[0] - t[0];
	v[1] = obb->vc[1] - t[1];
	v[2] = obb->vc[2] - t[2];

	if(roll != 0){
		a = 1.0 + roll/20.0;
		v[0] *= a;
		v[1] *= a;
		v[2] *= a;
		obb->vc[0] = t[0]+v[0];
		obb->vc[1] = t[1]+v[1];
		obb->vc[2] = t[2]+v[2];
	}
	if(dy != 0){
		q[0] =-v[1];
		q[1] = v[0];
		q[2] = 0.0;
		quaternion_operation(v, q, -dy/100.0);
		obb->vc[0] = t[0]+v[0];
		obb->vc[1] = t[1]+v[1];
		obb->vc[2] = t[2]+v[2];
	}
	if(dx != 0){
		a = -dx/100.0;
		s = sine(a);
		c = cosine(a);
		obb->vc[0] = t[0] + v[0]*c - v[1]*s;
		obb->vc[1] = t[1] + v[0]*s + v[1]*c;
	}

	obb->vf[0] = t[0] - obb->vc[0];
	obb->vf[1] = t[1] - obb->vc[1];
	obb->vf[2] = t[2] - obb->vc[2];
	vec3_normalize(obb->vf);

	//a × b= [aybz-azby,azbx-axbz, axby-aybx]
	obb->vr[0] = obb->vf[1];
	obb->vr[1] =-obb->vf[0];
	obb->vr[2] = 0.0;

	//
	obb->vt[0] = obb->vr[1]*obb->vf[2] - obb->vr[2]*obb->vf[1];
	obb->vt[1] = obb->vr[2]*obb->vf[0] - obb->vr[0]*obb->vf[2];
	obb->vt[2] = obb->vr[0]*obb->vf[1] - obb->vr[1]*obb->vf[0];
}
static int thirdperson_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int dx,dy,roll;
	short* t;
	struct actor* tmp;

	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	struct fstyle* tar;

	thirdperson_search(act, 0, &self, &peer);
	obb = peer->pfoot;

	thirdperson_search(act, _tar_, &self, &peer);
	tmp = peer->pchip;
	if(_char_ == ev->what){
		tmp->onwrite(peer, self, 0, 0, ev, 0);
	}

	tmp->onsearch(tmp, 0, &self, &peer);
	tar = peer->pfoot;

	if(_char_ == ev->what){
		thirdperson_fixcam(obb, tar, 0, 0, 0);
		return 0;
	}
	if(0x4070 == ev->what){
		t = (void*)ev;
		if(act->w0 != 0){
			thirdperson_fixcam(obb, tar, t[0] - act->xn, t[1] - act->yn, 0);
		}
		act->xn = t[0];
		act->yn = t[1];
	}
	if(0x2b70 == ev->what){
		t = (void*)ev;
		say("%c\n",t[3]);
		switch(t[3]){
			case 'l':
			case 'r':{
				act->w0 = 1;
				act->x0 = act->xn = t[0];
				act->y0 = act->yn = t[1];
				break;
			}
			case 'f':{
				thirdperson_fixcam(obb, tar, 0, 0, -1);
				break;
			}
			case 'b':{
				thirdperson_fixcam(obb, tar, 0, 0, 1);
				break;
			}
		}
	}
	if(0x2d70 == ev->what){
		act->w0 = 0;
	}

	return 0;
}




static void thirdperson_matrix(
	struct actor* act, struct fstyle* frus,
	struct actor* ctx, struct fstyle* area)
{
	//say("@freecam_matrix:%llx,%llx,%llx,%llx\n", ctx->gl_camera, ctx->gl_light, ctx->gl_solid, ctx->gl_opaque);
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	float dx,dy;

	dx = area->vq[0] * ctx->fbwidth;
	dy = area->vq[1] * ctx->fbheight;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
	thirdperson_search(act, 0, &self, &peer);

	obb = peer->pfoot;
	freecam_shape2frustum(obb, frus);

	float* mat = act->buf;
	fixmatrix((void*)mat, frus);
	mat4_transpose((void*)mat);
	//printmat4(m);

	struct glsrc* src = ctx->gl_camera;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = mat;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = obb->vc;
}




static void thirdperson_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@freecam_read:\n");

	if(ctx){
		switch(ctx->type){
			case _gl41data_:thirdperson_draw(act,pin,ctx,sty);
		}
	}
	else{
		switch(win->type){
			case _gl41wnd0_:thirdperson_matrix(act, &pin->fs, win, &sty->fs);
		}
	}
}
static int thirdperson_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;

	thirdperson_event(act, pin, win, sty, ev, 0);
	return 0;
}
static void thirdperson_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void thirdperson_start(struct halfrel* self, struct halfrel* peer)
{
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
