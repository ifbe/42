#include "libuser.h"
#define MATBUF buf0
#define CAMBUF buf1
#define EVTYPE iw0
#define EVSEND 666666
int matorth(mat4 m, struct fstyle* s);
//
void gl41data_before(void*);
void gl41data_after(void*);
int gl41data_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);




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
		if(_virtual_ == world->type){
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
	act->CAMBUF = memorycreate(0x1000, 0);
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
	wnd->glfull_camera[0] = act->CAMBUF;
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




static int orthcam_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		orthcam_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int orthcam_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//find world from camera
	struct halfrel* tmp[2];
	orthcam_search(ent, 0, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].flag = tmp[0]->flag;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].flag = tmp[1]->flag;

//[-2,-1]: wnd,area -> cam,togl
//[+0,+1]: cam,towr -> wor,geom
	struct style* slot;
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* geom;
	slot = stack[sp-1].pfoot;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;
	if('v' == key){
		gl41data_before(wnd);

		orthcam_ratio(wor, geom, wnd, area);
		orthocam_shape2frustum(&geom->fshape, &geom->frustum);
		orthcam_matrix(ent,slot, wor,geom);
		orthcam_camera(ent,slot, wor,geom, wnd,area);

		gl41data_taking(wor,0, stack,sp+2, 0,'v', buf,len);

		gl41data_after(wnd);
		return -1;
	}
	if('?' == key){
		gl41data_taking(wor,0, stack,sp+2, 0,'?', buf,len);
		return 0;
	}
	return 0;
}
static int orthcam_write_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}




static int orthcam_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(sp < 2)return 0;
	struct entity* sup = stack[sp-2].pchip;
	if(0 == sup)return 0;

	switch(sup->fmt){
	case _gl41full_:return orthcam_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	default:return orthcam_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int orthcam_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(EVSEND == ent->EVTYPE)give_data_into_peer(ent,_evto_, stack,sp, arg,key, buf,len);
	return 0;
}
static void orthcam_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void orthcam_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@orthcam_linkup\n");
	if(_evto_ == self->flag){
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
	p->ontaking = (void*)orthcam_taking;
	p->ongiving = (void*)orthcam_giving;
}
