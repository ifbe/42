#include "libuser.h"
#define _in_ hex32('i','n', 0, 0)
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
	act->MATBUF = memorycreate(64*2, 0);
	act->CAMBUF = memorycreate(0x1000, 0);
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
static int vrglass_event(_obj* act, struct fstyle* pin, struct event* ev, int len)
{
	short* t;
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	//say("vrglass_event@%llx:%x,%x\n", act, ev->why, ev->what);

	relationsearch(act, _in_, &self, &peer);
	obb = peer->pfoot;

	switch(ev->what){
	case 0x4070:
	case touch_move:
		if(0 == act->whdf.fwn)return 0;
		t = (void*)ev;

		obb->vq[0] += t[0] - act->whdf.fxn;
		obb->vq[2] -= t[1] - act->whdf.fyn;
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
		if(t[3] & joyl_left   )obb->vq[0] -= 10.0;
		if(t[3] & joyl_right  )obb->vq[0] += 10.0;
		if(t[3] & joyl_down   )obb->vq[2] += 10.0;
		if(t[3] & joyl_up     )obb->vq[2] -= 10.0;
		if(t[3] & joyl_trigger)obb->vq[1] -= 10.0;
		if(t[3] & joyl_bumper )obb->vq[1] += 10.0;
	}
	if(_char_ == ev->what){
		switch(ev->why){
			case 'a':obb->vq[0] -= 10.0;break;
			case 'd':obb->vq[0] += 10.0;break;
			case 's':obb->vq[1] -= 10.0;break;
			case 'w':obb->vq[1] += 10.0;break;
			case 'q':obb->vq[2] -= 10.0;break;
			case 'e':obb->vq[2] += 10.0;break;
		}
		say("%f,%f,%f\n", obb->vq[0], obb->vq[1], obb->vq[2]);
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
	//say("@vrglass_frus:%llx,%llx,%llx,%f,%f,%f\n",frus,plane,eye,eye[0],eye[1],eye[2]);

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
/*	say("vrglass_frustum: (%f,%f), (%f,%f), (%f,%f), (%f,%f,%f)\n",
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
	wnd->gl41list.camera[0] = act->CAMBUF;
}




static int vrglass_read_bycam(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->hfmt){
	case _tui_:
	case _rgba_:
		return 0;
	case _gl41list_:
		 vrglass_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int vrglass_read_bywnd(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//find world from camera
	struct halfrel* tmp[2];
	relationsearch(ent, _in_, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].flag = tmp[0]->flag;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].flag = tmp[1]->flag;

//[-2,-1]: wnd,area -> cam,togl
//[+0,+1]: cam,towr -> wor,geom
	_obj* wnd;struct style* area;
	_obj* wor;struct style* geom;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wor = stack[sp+1].pchip;geom = stack[sp+1].pfoot;

	switch(wnd->hfmt){
	default:
		//clear all
		gl41data_before(wnd);
		//camera matrix
		vrglass_ratio(wor, geom, wnd, area);
		vrglass_frustum(&geom->frustum, &geom->fshape);
		vrglass_matrix(ent,slot, wor,geom);
		vrglass_camera(ent,slot, wor,geom, wnd,area);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', buf,len);
		//enq++
		gl41data_after(wnd);
		break;
	}
	return 0;
}




static int vrglass_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		return vrglass_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);
	default:
		return vrglass_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
static int vrglass_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	vrglass_event(ent, 0, buf, 0);
	return 0;
}
static void vrglass_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void vrglass_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void vrglass_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onsearch = (void*)vrglass_search;
	p->onmodify = (void*)vrglass_modify;

	p->onlinkup = (void*)vrglass_linkup;
	p->ondiscon = (void*)vrglass_discon;
	p->ontaking = (void*)vrglass_taking;
	p->ongiving = (void*)vrglass_giving;
}
