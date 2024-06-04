#include "libuser.h"
#define EVSEND 666666
void world2clip_orthz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_orthznzp_transpose(mat4 mat, struct fstyle* frus);


#define FEATURE_LOG 1
#define FEATURE_AUXLINE 2
struct privdata{
	int evtype;
	u32 feature;

	struct halfrel* self;
	struct halfrel* peer;

	mat4 matbuf;

	struct gl41data* gl41cam;
};


static void orthcam_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
	struct fstyle* obb = 0;
	//logtoall("orthcam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

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
static void orthcam_modify(_obj* act)
{
}
static void orthcam_delete(_obj* act)
{
}
static void orthcam_create(_obj* act, void* arg, int argc, u8** argv)
{
    logtoall("@orthcam_create\n");
	struct privdata* priv = (void*)act->priv_256b;
	priv->gl41cam = memoryalloc(0x1000, 0);
	priv->evtype = 0;

	int j;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "log:", 4)){
			priv->feature |= FEATURE_LOG;
		}
	}
}




static void orthcam_camera(
	_obj* act, struct style* slot,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct privdata* priv = (void*)act->priv_256b;
	struct gl41data* data = priv->gl41cam;

	struct fstyle* frus = &geom->frus;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = priv->matbuf;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	wnd->gl41list.world[0].camera[0] = priv->gl41cam;
}
static int orthcam_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
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
	gl41frustum(ctx, sty);
*/
	return 0;
}
static int orthcam_event(
	_obj* act, struct style* pin,
	_obj* wld, struct style* sty,
	struct event* ev, int len)
{
	//logtoall("orthcam_event@%llx:%x,%x\n", act, ev->why, ev->what);
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
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	struct fstyle* frus = &geom->frus;
	float dx = rect->vq[0] * wnd->whdf.fbwidth;
	float dy = rect->vq[1] * wnd->whdf.fbheight;
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
	_obj* act, struct style* slot,
	_obj* wrd, struct style* geom)
{
	struct privdata* priv = (void*)act->priv_256b;
	if(priv->feature & FEATURE_LOG){
		logtoall("orthcam@%p: %f,%f,%f\n", act, geom->fs.vc[0], geom->fs.vc[1], geom->fs.vc[2]);
	}

	struct fstyle* frus = &geom->frus;
	world2clip_orthznzp_transpose(priv->matbuf, frus);
}
//world - camera - window
static int orthcam_visitworld(
_obj* wor,struct style* geom,
_obj* ent,void* slot,
_obj* wnd,struct style* area,
_syn* stack,int sp)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		gl41data_before(wnd);

		orthcam_ratio(wor, geom, wnd, area);
		orthocam_shape2frustum(&geom->fshape, &geom->frustum);
		orthcam_matrix(ent,slot, wor,geom);
		orthcam_camera(ent,slot, wor,geom, wnd,area);

		gl41data_taking(wor,0, stack,sp+2, 0,'v', 0,0);

		gl41data_after(wnd);
		break;
	}
	return 0;
}




static int orthcam_byworld_bycam_bywnd_read(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,geom		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
	struct style* slot;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	orthcam_draw_gl41(ent,slot, wor,geom, wnd,area);
	return 0;
}
static int orthcam_bywnd_read(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
//[-2,-1]: wnd,area -> cam,togl
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

//find world from camera
	struct halfrel* tmp[2];
	orthcam_search(ent, 0, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].foottype = tmp[0]->foottype;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].foottype = tmp[1]->foottype;

//[+0,+1]: cam,towr -> wor,geom
	struct style* slot = stack[sp-1].pfoot;
	_obj* world = stack[sp+1].pchip;
	struct style* geom = stack[sp+1].pfoot;
	orthcam_visitworld(world,geom, ent,slot, wnd,area, stack,sp);
	return 0;
}




void orthcam_move(vec3 dst, vec3 src, float t)
{
	dst[0] += src[0] * t;
	dst[1] += src[1] * t;
	dst[2] += src[2] * t;
}
void orthcam_rotate(vec3 a, vec3 b, vec3 axis, float angle)
{
	quaternion_operation(a, axis, angle);
	quaternion_operation(b, axis, angle);
}
static int orthcam_bywnd_write(_obj* ent,void* ef, _obj* wnd,void* wf, struct event* ev)
{
	struct privdata* priv = (void*)ent->priv_256b;
	struct halfrel* rel = priv->peer;

	_obj* wor = rel->pchip;
	struct style* geom = rel->pfoot;

	struct fstyle* obb = &geom->fshape;
	switch(ev->why){
		case 'a':orthcam_move(obb->vc, obb->vr,-1.0);break;
		case 'd':orthcam_move(obb->vc, obb->vr, 1.0);break;
		case 's':orthcam_move(obb->vc, obb->vf,-1.0);break;
		case 'w':orthcam_move(obb->vc, obb->vf, 1.0);break;
		case 'f':orthcam_move(obb->vc, obb->vt,-1.0);break;
		case 'r':orthcam_move(obb->vc, obb->vt, 1.0);break;

		case 'j':orthcam_rotate(obb->vr, obb->vf, obb->vt, 0.05);break;
		case 'l':orthcam_rotate(obb->vr, obb->vf, obb->vt,-0.05);break;
		case 'i':orthcam_rotate(obb->vf, obb->vt, obb->vr, 0.05);break;
		case 'k':orthcam_rotate(obb->vf, obb->vt, obb->vr,-0.05);break;
		case 'u':orthcam_rotate(obb->vr, obb->vt, obb->vf,-0.05);break;
		case 'o':orthcam_rotate(obb->vr, obb->vt, obb->vf, 0.05);break;
	}
	return 0;
}




static int orthcam_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller = stack[sp-2].pchip;
	if(0 == caller)return 0;

	switch(caller->type){
	case _wnd_:
	case _render_:
		return orthcam_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	default:
		return orthcam_byworld_bycam_bywnd_read(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int orthcam_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct privdata* priv = (void*)ent->priv_256b;
	if(EVSEND == priv->evtype){
		give_data_into_peer(ent,_evto_, stack,sp, arg,key, buf,len);
		return 0;
	}

	_obj* selfnode = ent; //stack[sp-1].pchip;
	void* selffoot = stack[sp-1].pfoot;
	_obj* callnode = stack[sp-2].pchip;
	void* callfoot = stack[sp-2].pfoot;
	return orthcam_bywnd_write(selfnode,selffoot, callnode,callfoot, buf);
}
static void orthcam_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void orthcam_attach(struct halfrel* self, struct halfrel* peer)
{
    logtoall("@orthcam_attach\n");
	_obj* cam = self->pchip;
	if(_evto_ == self->foottype){
		struct privdata* priv = (void*)cam->priv_256b;
		priv->evtype = EVSEND;
	}

	struct privdata* priv = (void*)cam->priv_256b;
	_obj* that = peer->pchip;
	logtoall("thattype=%.4s\n", &that->type);
	switch(that->type){
	case _virtual_:
	case _scene3d_:
		priv->self = self;
		priv->peer = peer;
		return;
	}
}




void orthcam_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('o', 'r', 't', 'h', 'c', 'a', 'm', 0);

	p->oncreate = (void*)orthcam_create;
	p->ondelete = (void*)orthcam_delete;
	p->onreader = (void*)orthcam_search;
	p->onwriter = (void*)orthcam_modify;

	p->onattach = (void*)orthcam_attach;
	p->ondetach = (void*)orthcam_detach;
	p->ontaking = (void*)orthcam_taking;
	p->ongiving = (void*)orthcam_giving;
}
