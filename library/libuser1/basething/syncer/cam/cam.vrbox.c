#include "libuser.h"
#define _in_ hex32('i','n', 0, 0)
#define MATBUF listptr.buf0
#define CAMBUF listptr.buf1
void quaternion4axisandangle(float* q, float* a, float angle);
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);




static int vrbox_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(wnd, 0xff0000, vc, vr, vf, vt);

	int j;
	vec3 tr,tf,tt;
	vec3 north,earth;
	float lr = vec3_getlen(vr);
	float lf = vec3_getlen(vf);
	float lt = vec3_getlen(vt);
	for(j=0;j<3;j++){
		earth[j] = vc[j];
		north[j] = vc[j];
		tr[j] = vr[j];
		tf[j] = vf[j];
		tt[j] = vt[j];
	}
	vec3_setlen(tr, lf*0.5);
	vec3_setlen(tf, lf*0.5);
	vec3_setlen(tt, lf*0.5);

	earth[2] -= (lr+lt)*0.5;
	gl41line(wnd,0xff00ff,vc,earth);
	gl41opaque_sphere(wnd,0x8000ff00, earth,tr,tf,tt);

	north[1] += (lr+lt)*0.5;
	gl41line(wnd,0x00ffff,vc,north);
	gl41opaque_sphere(wnd,0x8000ff00, north,tr,tf,tt);
	return 0;
}
static int vrbox_event(_obj* act, struct fstyle* pin, struct event* ev, int len)
{
	struct halfrel* self;
	struct halfrel* peer;
	relationsearch(act, _in_, &self, &peer);

	struct fstyle* obb = peer->pfoot;
	if(0 == obb)return 0;

	short* t;
	switch(ev->what){
	case 0x4070:
	case touch_abs:
		if(0 == act->whdf.fwn)return 1;
		t = (void*)ev;

        float dr = (t[0] - act->whdf.fxn)/10;
        float db = (t[1] - act->whdf.fyn)/10;

		quaternion_operation(obb->vr, obb->vt, dr/100.0);
		quaternion_operation(obb->vf, obb->vt, dr/100.0);
		quaternion_operation(obb->vf, obb->vr, db/100.0);
		quaternion_operation(obb->vt, obb->vr, db/100.0);

/*		obb->vq[0] += dr*obb->vr[0] - db*obb->vt[0];
        obb->vq[1] += dr*obb->vr[1] - db*obb->vt[1];
		obb->vq[2] += dr*obb->vr[2] - db*obb->vt[2];*/

/*		vec4 q = {0.0, 0.0, 0.0, 1.0};
		obb->vr[0] = 1.0;obb->vr[1] = 0.0;obb->vr[2] = 0.0;
		obb->vf[0] = 0.0;obb->vf[1] = 1.0;obb->vf[2] = 0.0;
		obb->vt[0] = 0.0;obb->vt[1] = 0.0;obb->vt[2] = 1.0;
		quaternion_rotate(obb->vr, q);
		quaternion_rotate(obb->vf, q);
		quaternion_rotate(obb->vt, q);
		vec3_setlen(obb->vr, obb->vr[3]);
		vec3_setlen(obb->vt, obb->vt[3]);
		vec3_setlen(obb->vf, obb->vf[3]);*/

		act->whdf.fxn = t[0];
		act->whdf.fyn = t[1];
		return 1;
	case 0x2b70:
	case touch_onto:
		t = (void*)ev;
		act->whdf.fxn = t[0];
		act->whdf.fyn = t[1];
		act->whdf.fwn = 1;
		return 1;
	case 0x2d70:
	case touch_away:
		act->whdf.fwn = 0;
		return 1;
	}

	printmemory(ev, 16);
	return 0;
}
static int vrbox_sensor(_obj* act, struct fstyle* pin, float* f, int len)
{
	say("g(%f,%f,%f),a(%f,%f,%f),m(%f,%f,%f)\n",f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7],f[8]);
	return 1;
}
static int vrbox_quaternion(_obj* act, struct fstyle* pin, float* q, int len)
{
	//say("q(%f,%f,%f,%f)\n",q[0],q[1],q[2],q[3]);

	struct halfrel* self;
	struct halfrel* peer;
	relationsearch(act, _in_, &self, &peer);

	struct fstyle* obb = peer->pfoot;
	if(0 == obb)return 0;

	//fix quaternion: head west => heading north
	vec4 fix = {0, 0, sqrt2/2, sqrt2/2};
	quaternion_multiply(fix, q);

	float* vr = obb->vr;
	float* vf = obb->vf;
	float* vt = obb->vt;
	vr[0] = 1.0;vr[1] = 0.0;vr[2] = 0.0;
	vf[0] = 0.0;vf[1] = 0.0;vf[2] =-1.0;
	vt[0] = 0.0;vt[1] = 1.0;vt[2] = 0.0;
	quaternion_rotate(vr, fix);
	quaternion_rotate(vf, fix);
	quaternion_rotate(vt, fix);
	//say("r(%f,%f,%f),f(%f,%f,%f),t(%f,%f,%f)\n", vr[0],vr[1],vr[2], vf[0],vf[1],vf[2], vt[0],vt[1],vt[2]);
	vec3_setlen(vr, vr[3]);
	vec3_setlen(vt, vt[3]);
	vec3_setlen(vf, vf[3]);
	return 1;
}




void vrbox_ratio(
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
void vrbox_frustum(struct fstyle* frus, struct fstyle* plane)
{
	float* eye;
	float rx,ry,rz,rn;
	float fx,fy,fz,fn;
	float ux,uy,uz,un;
	float dr = plane->vq[0];
	float df = plane->vq[1];
	float du = plane->vq[2];
	//say("@vrbox_frus:%llx,%llx,%llx,%f,%f,%f\n",frus,plane,eye,eye[0],eye[1],eye[2]);

	//l,r
	rx = plane->vr[0];
	ry = plane->vr[1];
	rz = plane->vr[2];
	rn = squareroot(rx*rx + ry*ry + rz*rz);
	rx /= rn;
	ry /= rn;
	rz /= rn;
	frus->vl[0] = -rx;
	frus->vl[1] = -ry;
	frus->vl[2] = -rz;
	frus->vr[0] = rx;
	frus->vr[1] = ry;
	frus->vr[2] = rz;

	//b,t
	ux = plane->vt[0];
	uy = plane->vt[1];
	uz = plane->vt[2];
	un = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= un;
	uy /= un;
	uz /= un;
	frus->vb[0] = -ux;
	frus->vb[1] = -uy;
	frus->vb[2] = -uz;
	frus->vt[0] = ux;
	frus->vt[1] = uy;
	frus->vt[2] = uz;

	//n,f
	fx = plane->vf[0];
	fy = plane->vf[1];
	fz = plane->vf[2];
	fn = squareroot(fx*fx + fy*fy + fz*fz);
	fx /= fn;
	fy /= fn;
	fz /= fn;
	frus->vn[0] = fx;
	frus->vn[1] = fy;
	frus->vn[2] = fz;
	frus->vf[0] = fx;
	frus->vf[1] = fy;
	frus->vf[2] = fz;

	eye = frus->vc;
	eye[0] = plane->vc[0] + dr*rx + df*fx + du*ux;
	eye[1] = plane->vc[1] + dr*ry + df*fy + du*uy;
	eye[2] = plane->vc[2] + dr*rz + df*fz + du*uz;

	frus->vl[3] = 
		(plane->vc[0] - plane->vr[0] - eye[0])*rx +
		(plane->vc[1] - plane->vr[1] - eye[1])*ry +
		(plane->vc[2] - plane->vr[2] - eye[2])*rz;
	frus->vr[3] = 
		(plane->vc[0] + plane->vr[0] - eye[0])*rx +
		(plane->vc[1] + plane->vr[1] - eye[1])*ry +
		(plane->vc[2] + plane->vr[2] - eye[2])*rz;
	frus->vb[3] = 
		(plane->vc[0] - plane->vt[0] - eye[0])*ux +
		(plane->vc[1] - plane->vt[1] - eye[1])*uy +
		(plane->vc[2] - plane->vt[2] - eye[2])*uz;
	frus->vt[3] = 
		(plane->vc[0] + plane->vt[0] - eye[0])*ux +
		(plane->vc[1] + plane->vt[1] - eye[1])*uy +
		(plane->vc[2] + plane->vt[2] - eye[2])*uz;
	frus->vn[3] = 
		(plane->vc[0] - plane->vf[0] - eye[0])*fx +
		(plane->vc[1] - plane->vf[1] - eye[1])*fy +
		(plane->vc[2] - plane->vf[2] - eye[2])*fz;
	frus->vf[3] = frus->vn[3]*1000*1000;
/*	say("vrbox_frustum: (%f,%f), (%f,%f), (%f,%f), (%f,%f,%f)\n",
		frus->vn[3], frus->vf[3], frus->vl[3], frus->vr[3], frus->vb[3], frus->vt[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/
}
static void vrbox_matrix(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom)
{
	struct fstyle* frus = &geom->frus;

	float* mat = act->MATBUF;
	world2clip_projznzp_transpose((void*)mat, frus);
	//printmat4(mat);
}
static void vrbox_camera(
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




static int vrbox_read_bycam(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		 vrbox_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	return 0;
}
static int vrbox_read_bywnd(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//find world from camera
	struct halfrel* tmp[2];
	relationsearch(ent, _in_, &tmp[0], &tmp[1]);
	stack[sp+0].pchip = tmp[0]->pchip;
	stack[sp+0].pfoot = tmp[0]->pfoot;
	stack[sp+0].foottype = tmp[0]->foottype;
	stack[sp+1].pchip = tmp[1]->pchip;
	stack[sp+1].pfoot = tmp[1]->pfoot;
	stack[sp+1].foottype = tmp[1]->foottype;

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
		vrbox_ratio(wor, geom, wnd, area);
		vrbox_frustum(&geom->frustum, &geom->fshape);
		vrbox_matrix(ent,slot, wor,geom);
		vrbox_camera(ent,slot, wor,geom, wnd,area);
		//render data
		gl41data_taking(wor,0, stack,sp+2, 0,'v', buf,len);
		//enq++
		gl41data_after(wnd);
		break;
	}
	return 0;
}




static void vrbox_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void vrbox_modify(_obj* act)
{
}
static void vrbox_delete(_obj* act)
{
}
static void vrbox_create(_obj* act, void* str)
{
	act->MATBUF = memorycreate(64*2, 0);
	act->CAMBUF = memorycreate(0x1000, 0);
}




static int vrbox_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		return vrbox_read_bywnd(ent,foot, stack,sp, arg,cmd, buf,len);
	default:
		return vrbox_read_bycam(ent,foot, stack,sp, arg,cmd, buf,len);
	}
	return 0;
}
static int vrbox_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
#define _sens_ hex32('s','e','n','s')
#define _quat_ hex32('q','u','a','t')
	switch(cmd){
	case _quat_:
		vrbox_quaternion(ent,0, buf,0);
		break;
	case _sens_:
		vrbox_sensor(ent,0, buf,0);
		break;
	default:
		vrbox_event(ent, 0, buf, 0);
	}
	return 0;
}
static void vrbox_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void vrbox_attach(struct halfrel* self, struct halfrel* peer)
{
}




void vrbox_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('v', 'r', 'b', 'o', 'x', 0, 0, 0);

	p->oncreate = (void*)vrbox_create;
	p->ondelete = (void*)vrbox_delete;
	p->onreader = (void*)vrbox_search;
	p->onwriter = (void*)vrbox_modify;

	p->onattach = (void*)vrbox_attach;
	p->ondetach = (void*)vrbox_detach;
	p->ontaking = (void*)vrbox_taking;
	p->ongiving = (void*)vrbox_giving;
}
