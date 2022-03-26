#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_insert(_obj* ctx, int type, struct gl41data* src, int cnt);
void gl41data_addcam(_obj* wnd, struct gl41data* data);


#define CTXBUF listptr.buf0
struct waterbuf{
	mat4 wvp;
	float time;
	struct gl41data geom;
	struct gl41data dest;
};




static void water_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void water_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void water_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void water_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void water_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




void water_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;


//-------------p,q---------------
	//water.n
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * water.n
	t = (cam[0] - obb->vc[0])*x
	  + (cam[1] - obb->vc[1])*y
	  + (cam[2] - obb->vc[2])*z;

	//foot of a perpendicular: q = p - t*water.n
	frus->vq[0] = cam[0] - t*x;
	frus->vq[1] = cam[1] - t*y;
	frus->vq[2] = cam[2] - t*z;

	//reflected point: p' = p - 2*t*water.n
	frus->vc[0] = cam[0] - 2*t*x;
	frus->vc[1] = cam[1] - 2*t*y;
	frus->vc[2] = cam[2] - 2*t*z;


//----------------n,f----------------
	//vec(frus.n) = vec(water.n), len(frus.n) > len(cam to plane)
	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = t*1.001;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e20;


//--------------l,r--------------------
	//nr = -norm(water.r)
	x = -obb->vr[0];
	y = -obb->vr[1];
	z = -obb->vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//l.len = (l-q) * nr
	t = (obb->vc[0] + obb->vr[0] - frus->vq[0]) * x
	  + (obb->vc[1] + obb->vr[1] - frus->vq[1]) * y
	  + (obb->vc[2] + obb->vr[2] - frus->vq[2]) * z;
	frus->vl[0] = -x;
	frus->vl[1] = -y;
	frus->vl[2] = -z;
	frus->vl[3] = t;

	//r.len = (r-q) * nr
	t = (obb->vc[0] - obb->vr[0] - frus->vq[0]) * x
	  + (obb->vc[1] - obb->vr[1] - frus->vq[1]) * y
	  + (obb->vc[2] - obb->vr[2] - frus->vq[2]) * z;
	frus->vr[0] = x;
	frus->vr[1] = y;
	frus->vr[2] = z;
	frus->vr[3] = t;


//----------------b,t-----------------
	//nt = norm(water.t)
	x = obb->vf[0];
	y = obb->vf[1];
	z = obb->vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len =  = (b-q) * nt
	t = (obb->vc[0] - obb->vf[0] - frus->vq[0]) * x
	  + (obb->vc[1] - obb->vf[1] - frus->vq[1]) * y
	  + (obb->vc[2] - obb->vf[2] - frus->vq[2]) * z;
	frus->vb[0] = x;
	frus->vb[1] = y;
	frus->vb[2] = z;
	frus->vb[3] = t;

	//t.len = (u-q) * nt
	t = (obb->vc[0] + obb->vf[0] - frus->vq[0]) * x
	  + (obb->vc[1] + obb->vf[1] - frus->vq[1]) * y
	  + (obb->vc[2] + obb->vf[2] - frus->vq[2]) * z;
	frus->vt[0] = x;
	frus->vt[1] = y;
	frus->vt[2] = z;
	frus->vt[3] = t;
}
static void water_gl41fbo_update(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wrl, struct style* camg,
	_obj* wnd, struct style* area)
{
	struct waterbuf* water = act->CTXBUF;
	if(0 == water)return;

	//give arg(matrix and position) to fbo
	struct gl41data* data = &water->dest;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = water->wvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = geom->frus.vc;
	gl41data_addcam((void*)wnd, data);
}
void water_gl41fbo_prepare(struct mysrc* src)
{
	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'c';
	src->target_enq = 42;
}




static void water_gl41geom_update(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vf);


	struct waterbuf* water = act->CTXBUF;
	if(0 == water)return;

	water->time = (timeread_us()%10000000)/10000000.0;


	struct gl41data* body = &water->geom;
	if(0 == body)return;
	struct gl41data* dest = &water->dest;
	if(0 == dest)return;

//.texture
	body->dst.texname[0] = "tex0";
	body->src.tex[0].glfd = dest->dst.tex[0];
	body->src.tex[0].fmt = '!';
	body->src.tex_enq[0] += 1;

//.vertex
	float (*vbuf)[6] = body->src.vtx[0].vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 1.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 0.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 0.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 1.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 0.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	body->src.vbuf_enq += 1;
	gl41data_insert(ctx, 'o', body, 1);
}
void water_gl41geom_prepare(struct gl41data* data, struct waterbuf* water, char* str)
{
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;

	//
	src->vs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/water/vert.glsl", 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/water/frag.glsl", 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//argument
	dst->arg[0].name = "time";
	dst->arg[0].data = &water->time;
	dst->arg[0].fmt = 'f';

	//texture0
	dst->texname[0] = "dudvmap";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], str);
	src->tex_enq[0] = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 1;

	vtx->vbuf = memorycreate(4*6*6, 0);
	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}




static void water_wrl_cam_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	if(0 == stack)return;

	_obj* wor;struct style* geom;
	_obj* dup;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	//frus from shape and eye
	water_frustum(&geom->frus, &geom->fshape, camg->frus.vc);

	//mvp from frus
	struct waterbuf* water = ent->CTXBUF;
	if(0 == water)return;

	if(_gl41list_ == wnd->hfmt)world2clip_projznzp_transpose(water->wvp, &geom->frus);
	else world2clip_projz0z1_transpose(water->wvp, &geom->frus);

	//create or update fbo
	water_gl41fbo_update(ent,foot, wor,geom, dup,camg, (void*)wnd,area);

	//geom
	water_gl41geom_update(ent,foot, wor,geom, wnd,area);
}




static void water_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		water_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void water_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void water_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void water_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void water_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
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
static void water_modify(_obj* act)
{
}
static void water_delete(_obj* act)
{
}
static void water_create(_obj* act, char* str)
{
	if(0 == act)return;

	struct waterbuf* water = act->CTXBUF = memorycreate(0x10000, 0);
	if(0 == water)return;

	if(0 == str)str = "datafile/jpg/dudvmap.jpg";
	water_gl41geom_prepare(&water->geom, water, str);

	water_gl41fbo_prepare(&water->dest.src);
}




void water_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('w', 'a', 't', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)water_create;
	p->ondelete = (void*)water_delete;
	p->onsearch = (void*)water_search;
	p->onmodify = (void*)water_modify;

	p->onattach = (void*)water_attach;
	p->ondetach = (void*)water_detach;
	p->ontaking = (void*)water_taking;
	p->ongiving = (void*)water_giving;
}
