#include "libuser.h"
#define _in_ hex32('i','n', 0, 0)
#define _at_ hex32('a','t', 0, 0)
#define MATBUF listptr.buf0
#define CAMBUF listptr.buf1
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);




static void vrglass_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void vrglass_modify(_obj* act)
{
}
static void vrglass_delete(_obj* act)
{
}
static void vrglass_create(_obj* act, void* str)
{
	act->MATBUF = memoryalloc(64*2, 0);
	act->CAMBUF = memoryalloc(0x1000, 0);
}




static int vrglass_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,j;
	vec3 tc;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(y=0;y<10;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*y/10.0;
		gl41line_rect(wnd, 0xff0000, tc, vr, vt);
	}

	for(y=-1;y<2;y+=2){
		for(x=-1;x<2;x+=2){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*x +vt[j]*y;
				tc[j] = tc[j]*10 - geom->fs.vq[j]*9;
			}
			gl41line(wnd, 0xff0000, geom->fs.vq, tc);
		}
	}
	return 0;
}
static int vrglass_where(_obj* act, struct fstyle* pin, float* f, int len)
{
	logtoall("%f,%f,%f,%f\n", f[0], f[1], f[2], f[3]);
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	relationsearch(act, _in_, &self, &peer);
	obb = peer->pfoot;

	obb->vq[0] = (320-f[0])/640/2;
	obb->vq[2] = (240-f[1])/480/2 + obb->vc[2];
	obb->vq[1] = f[2]/100 - 4.0;
	return 0;
}
static int vrglass_event(_obj* act, struct fstyle* pin, struct event* ev, int len)
{
	short* t;
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	//logtoall("vrglass_event@%llx:%x,%x\n", act, ev->why, ev->what);

	relationsearch(act, _in_, &self, &peer);
	obb = peer->pfoot;

	switch(ev->what){
	case 0x4070:
	case touch_abs:
		if(0 == act->whdf.fwn)return 0;
		t = (void*)ev;

		obb->vq[0] += (t[0] - act->whdf.fxn)/10;
		obb->vq[2] -= (t[1] - act->whdf.fyn)/10;
		act->whdf.fxn = t[0];
		act->whdf.fyn = t[1];
		return 0;
	case 0x2b70:
	case touch_onto:
		t = (void*)ev;
		act->whdf.fxn = t[0];
		act->whdf.fyn = t[1];
		act->whdf.fwn = 1;
		return 0;
	case 0x2d70:
	case touch_away:
		act->whdf.fwn = 0;
		return 0;
	}

	if(joy_left == (ev->what&joy_mask)){
		t = (void*)ev;
		if(t[3] & joyl_left   )obb->vq[0] -= 1.0;
		if(t[3] & joyl_right  )obb->vq[0] += 1.0;
		if(t[3] & joyl_down   )obb->vq[2] += 1.0;
		if(t[3] & joyl_up     )obb->vq[2] -= 1.0;
		if(t[3] & joyl_trigger)obb->vq[1] -= 1.0;
		if(t[3] & joyl_bumper )obb->vq[1] += 1.0;
	}
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':obb->vq[0] -= 0.1;break;
			case 'd':obb->vq[0] += 0.1;break;
			case 's':obb->vq[1] -= 0.1;break;
			case 'w':obb->vq[1] += 0.1;break;
			case 'f':obb->vq[2] -= 0.1;break;
			case 'r':obb->vq[2] += 0.1;break;
		}
		logtoall("%f,%f,%f\n", obb->vq[0], obb->vq[1], obb->vq[2]);
	}
	return 1;
}




void vrglass_ratio(
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* rect = &area->fshape;
	float dx = rect->vq[0] * wnd->whdf.fbwidth;
	float dy = rect->vq[1] * wnd->whdf.fbheight;

	struct fstyle* shape = &geom->fshape;
	float lr = vec3_getlen(shape->vr);
	float lt = vec3_getlen(shape->vt);

	int j;
	for(j=0;j<3;j++)shape->vt[j] *= dy*lr/dx/lt;
}
void vrglass_frustum(struct fstyle* frus, struct fstyle* plane)
{
	float x,y,z,n;
	float* eye = plane->vq;
	//logtoall("@vrglass_frus:%llx,%llx,%llx,%f,%f,%f\n",frus,plane,eye,eye[0],eye[1],eye[2]);

	frus->vc[0] = eye[0];
	frus->vc[1] = eye[1];
	frus->vc[2] = eye[2];


	//l,r
	x = plane->vr[0];
	y = plane->vr[1];
	z = plane->vr[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vl[0] =-x;
	frus->vl[1] =-y;
	frus->vl[2] =-z;
	frus->vl[3] = 
	  (plane->vc[0] - plane->vr[0] - eye[0])*x
	+ (plane->vc[1] - plane->vr[1] - eye[1])*y
	+ (plane->vc[2] - plane->vr[2] - eye[2])*z;

	frus->vr[0] = x;
	frus->vr[1] = y;
	frus->vr[2] = z;
	frus->vr[3] = 
	  (plane->vc[0] + plane->vr[0] - eye[0])*x
	+ (plane->vc[1] + plane->vr[1] - eye[1])*y
	+ (plane->vc[2] + plane->vr[2] - eye[2])*z;


	//b,t
	x = plane->vt[0];
	y = plane->vt[1];
	z = plane->vt[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vb[0] =-x;
	frus->vb[1] =-y;
	frus->vb[2] =-z;
	frus->vb[3] = 
	  (plane->vc[0] - plane->vt[0] - eye[0])*x
	+ (plane->vc[1] - plane->vt[1] - eye[1])*y
	+ (plane->vc[2] - plane->vt[2] - eye[2])*z;

	frus->vt[0] = x;
	frus->vt[1] = y;
	frus->vt[2] = z;
	frus->vt[3] = 
	  (plane->vc[0] + plane->vt[0] - eye[0])*x
	+ (plane->vc[1] + plane->vt[1] - eye[1])*y
	+ (plane->vc[2] + plane->vt[2] - eye[2])*z;


	//n,f
	x = plane->vf[0];
	y = plane->vf[1];
	z = plane->vf[2];
	n = squareroot(x*x + y*y + z*z);
	x /= n;
	y /= n;
	z /= n;

	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = 
	  (plane->vc[0] - eye[0])*x
	+ (plane->vc[1] - eye[1])*y
	+ (plane->vc[2] - eye[2])*z;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e10;
/*	logtoall("vrglass_frustum: (%f,%f), (%f,%f), (%f,%f), (%f,%f,%f)\n",
		frus->vn[3], frus->vf[3], frus->vl[3], frus->vr[3], frus->vb[3], frus->vt[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/
}
static void vrglass_matrix(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;

	float* mat = act->MATBUF;
	world2clip_projznzp_transpose((void*)mat, frus);
	//printmat4(mat);
}
static void vrglass_camera(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct fstyle* frus = &geom->frus;
	struct gl41data* data = act->CAMBUF;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = act->MATBUF;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	wnd->gl41list.world[0].camera[0] = act->CAMBUF;
}
//world - camera - window
static int vrglass_visitworld(
_obj* wor,struct style* geom,
_obj* ent,void* slot,
_obj* wnd,struct style* area,
_syn* stack,int sp)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		//clear all
		gl41data_before(wnd);
		//camera matrix
		vrglass_ratio(wor, geom, wnd, area);
		vrglass_frustum(&geom->frustum, &geom->fshape);
		vrglass_matrix(ent,slot, wor,geom);
		vrglass_camera(ent,slot, wor,geom, wnd,area);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', 0,0);
		//enq++
		gl41data_after(wnd);
		break;
	}
	return 0;
}




static int vrglass_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->type){
	case _tui_:
	case _rgba_:
		return 0;
	case _gl41list_:
		 vrglass_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int vrglass_read_bywnd(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
//[-2,-1]: wnd,area -> cam,togl
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

//find world from camera
	struct halfrel* tmp[2];
	relationsearch(ent, _in_, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].foottype = tmp[0]->foottype;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].foottype = tmp[1]->foottype;

//[+0,+1]: cam,towr -> wor,geom
	_obj* world = stack[sp+1].pchip;
	struct style* geom = stack[sp+1].pfoot;

	vrglass_visitworld(world,geom, ent,slot, wnd,area, stack,sp);
	return 0;
}




static int vrglass_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		return vrglass_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	default:
		return vrglass_read_byworld_bycam_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int vrglass_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(stack[sp-1].foottype){
	case _at_:
		vrglass_where(ent, 0, buf, 0);
	default:
		vrglass_event(ent, 0, buf, 0);
	}
	return 0;
}
static void vrglass_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void vrglass_attach(struct halfrel* self, struct halfrel* peer)
{
}




void vrglass_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onreader = (void*)vrglass_search;
	p->onwriter = (void*)vrglass_modify;

	p->onattach = (void*)vrglass_attach;
	p->ondetach = (void*)vrglass_detach;
	p->ontaking = (void*)vrglass_taking;
	p->ongiving = (void*)vrglass_giving;
}
