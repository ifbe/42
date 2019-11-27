#include "libuser.h"
void ortho_mvp(mat4 m, struct fstyle* s);




static void orthcam_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
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
static void orthcam_modify(struct entity* act)
{
}
static void orthcam_delete(struct entity* act)
{
}
static void orthcam_create(struct entity* act, void* arg)
{
    say("@orthcam_create\n");
	act->buf = memorycreate(64, 0);
}




static int orthcam_draw_vbo(
	struct entity* act, struct style* pin,
	struct entity* ctx, struct style* sty)
{/*
	vec3 tc,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	carvesolid_prism4(ctx, 0x800000, tc, vr, vu, tf);

	sty->vn[0] = sty->vf[0];
	sty->vn[1] = sty->vf[1];
	sty->vn[2] = sty->vf[2];
	sty->vl[0] = -sty->vr[0];
	sty->vl[1] = -sty->vr[1];
	sty->vl[2] = -sty->vr[2];
	sty->vb[0] = -sty->vu[0];
	sty->vb[1] = -sty->vu[1];
	sty->vb[2] = -sty->vu[2];
	carvefrustum(ctx, sty);
*/
	return 0;
}
static int orthcam_event(
	struct entity* act, struct style* pin,
	struct entity* wld, struct style* sty,
	struct event* ev, int len)
{
	//say("orthcam_event@%llx:%x,%x\n", act, ev->why, ev->what);
	if(_char_ == ev->what){
		switch(ev->why){
		case 'a':sty->fs.vc[0] -= 10;break;
		case 'd':sty->fs.vc[0] += 10;break;
		}
	}
	return 0;
}




void orthocam_shape2frustum(struct fstyle* s, struct fstyle* d)
{
	float a,b,c;
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	//left, right
	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	d->vr[3] = n;
	d->vl[0] = - d->vr[0];
	d->vl[1] = - d->vr[1];
	d->vl[2] = - d->vr[2];
	d->vl[3] = -n;


	//bottom, top
	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	d->vt[3] = n;
	d->vb[0] = - d->vt[0];
	d->vb[1] = - d->vt[1];
	d->vb[2] = - d->vt[2];
	d->vb[3] = -n;


	//near, front
	a = s->vf[0] - s->vn[0];
	b = s->vf[1] - s->vn[1];
	c = s->vf[2] - s->vn[2];

	x = s->vn[0];
	y = s->vn[1];
	z = s->vn[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	if(a*x + b*y + c*z < 0)n = -n;
	d->vn[3] = n;

	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	if(a*x + b*y + c*z < 0)n = -n;
	d->vf[3] = n;
}
static void orthcam_matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	void* mat = act->buf;
	struct fstyle* frus = &geom->frus;

	ortho_mvp(mat, frus);
	mat4_transpose(mat);

	struct glsrc* src = wnd->gl_camera;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = mat;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
}
static void orthcam_ratio(
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float dx,dy;
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;

	dx = rect->vq[0] * wnd->fbwidth;
	dy = rect->vq[1] * wnd->fbheight;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
}




static void orthcam_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> tree
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len){
			orthcam_ratio(win, geom, wnd, area);
			orthocam_shape2frustum(&geom->fshape, &geom->frustum);
			orthcam_matrix(act,part, win,geom, wnd,area);
		}
	}
}
static int orthcam_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* wld;struct style* geom;
	struct entity* act;struct style* part;
	struct event* ev;

	wld = peer->pchip;geom = peer->pfoot;
	act = self->pchip;part = self->pfoot;
	ev = (void*)buf;

	//say("%llx@freecam_write:%llx,%llx,%llx,%llx\n", act, ev->why, ev->what, ev->where, ev->when);
	orthcam_event(act,part, wld,geom, ev, 0);
	return 0;
}
static void orthcam_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void orthcam_start(struct halfrel* self, struct halfrel* peer)
{
    say("@orthcam_start\n");
}




void orthcam_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'r', 't', 'h', 'c', 'a', 'm', 0);

	p->oncreate = (void*)orthcam_create;
	p->ondelete = (void*)orthcam_delete;
	p->onsearch = (void*)orthcam_search;
	p->onmodify = (void*)orthcam_modify;

	p->onstart = (void*)orthcam_start;
	p->onstop  = (void*)orthcam_stop;
	p->onread  = (void*)orthcam_read;
	p->onwrite = (void*)orthcam_write;
}
