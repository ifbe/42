#include "libuser.h"
#define MATBUF buf0
#define CAMBUF buf1
#define EVTYPE iw0
#define EVSEND 666666
int matorth(mat4 m, struct fstyle* s);
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);




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
	act->MATBUF = memorycreate(64, 0);
	act->CAMBUF = memorycreate(0x200, 0);
}




static void orthcam_camera(
	struct entity* act, struct style* slot,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct glsrc* src = act->CAMBUF;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = act->MATBUF;
	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
	wnd->gl_camera[0] = act->CAMBUF;
}
static int orthcam_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
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
	gl41solid_prism4(ctx, 0x800000, tc, vr, vu, tf);

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
		case 's':sty->fs.vc[1] -= 10;break;
		case 'w':sty->fs.vc[1] += 10;break;
		}
	}
	return 0;
}




static void orthcam_ratio(
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;
	float dx = rect->vq[0] * wnd->fbwidth;
	float dy = rect->vq[1] * wnd->fbheight;
	frus->vb[3] = frus->vl[3] * dy / dx;
	frus->vt[3] = frus->vr[3] * dy / dx;
}
void orthocam_shape2frustum(struct fstyle* s, struct fstyle* d)
{
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
	//d->vr[3] = n;
	d->vl[0] = - d->vr[0];
	d->vl[1] = - d->vr[1];
	d->vl[2] = - d->vr[2];
	//d->vl[3] = -n;


	//bottom, top
	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = n;
	d->vb[0] = - d->vt[0];
	d->vb[1] = - d->vt[1];
	d->vb[2] = - d->vt[2];
	//d->vb[3] = -n;


	//near, front
	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = n;
	d->vn[0] = - d->vf[0];
	d->vn[1] = - d->vf[1];
	d->vn[2] = - d->vf[2];
	//d->vn[3] = -n;
}
static void orthcam_matrix(
	struct entity* act, struct style* slot,
	struct entity* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;
	void* mat = act->MATBUF;

	matorth(mat, frus);
	mat4_transpose(mat);
}




static void orthcam_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	struct entity* act;struct style* slot;
	struct entity* wrd;struct style* geom;

	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;
	orthcam_search(cam, 0, &stack[rsp+0], &stack[rsp+1]);
	act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
	wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;

	if('v' == len){
		orthcam_ratio(wrd, geom, wnd, area);
		orthocam_shape2frustum(&geom->fshape, &geom->frustum);
		orthcam_matrix(act,slot, wrd,geom);

		gl41data_read(self, peer, stack, rsp+2, buf, len);
		orthcam_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
	if('?' == len){
		gl41data_read(self, peer, stack, rsp+2, buf, len);
		orthcam_camera(act,slot, wrd,geom, wnd,area);
	}
}
static void orthcam_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> texball
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;slot = self->pfoot;
		orthcam_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
}
static int orthcam_write_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	struct entity* act;struct style* slot;
	struct entity* wrd;struct style* geom;
	struct event* ev = (void*)buf;
	//say("%llx@freecam_write:%llx,%llx,%llx,%llx\n", act, ev->why, ev->what, ev->where, ev->when);

	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;
	orthcam_search(cam, 0, &stack[rsp+0], &stack[rsp+1]);
	act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
	wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;

	if(0x2b70 == ev->what){
		relationwrite(act,_ev_, stack,rsp, buf,len);
	}
	else{
		orthcam_event(act,slot, wrd,geom, ev, 0);
	}
	return 0;
}




static void orthcam_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:orthcam_read_bywnd(self, peer, stack, rsp, buf, len);break;
	default:orthcam_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
}
static int orthcam_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = self->pchip;
	if(EVSEND == ent->EVTYPE){
		relationwrite(ent,_ev_, stack,rsp, buf,len);
	}
	else{
		orthcam_write_bycam(self,peer, stack,rsp, buf,len);
	}
	return 1;
}
static void orthcam_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void orthcam_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@orthcam_linkup\n");
	if(_ev_ == self->flag){
		struct entity* cam = self->pchip;
		cam->EVTYPE = EVSEND;
	}
}




void orthcam_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'r', 't', 'h', 'c', 'a', 'm', 0);

	p->oncreate = (void*)orthcam_create;
	p->ondelete = (void*)orthcam_delete;
	p->onsearch = (void*)orthcam_search;
	p->onmodify = (void*)orthcam_modify;

	p->onlinkup = (void*)orthcam_linkup;
	p->ondiscon = (void*)orthcam_discon;
	p->onread  = (void*)orthcam_read;
	p->onwrite = (void*)orthcam_write;
}
