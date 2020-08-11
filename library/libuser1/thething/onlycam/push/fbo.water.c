#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void matproj_transpose(void* m, struct fstyle* sty);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


#define CAMBUF buf0
#define CTXBUF buf1
struct waterbuf{
	mat4 mvp;
	float time;
	u8 data[0];
};




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
static void water_forfbo_update(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wrl, struct style* camg,
	struct supply* fbo, struct style* area)
{
	//frus from shape and eye
	struct fstyle* shap = &geom->fshape;
	struct fstyle* frus = &geom->frustum;
	water_frustum(frus, shap, camg->frus.vc);

	//mvp from frus
	struct waterbuf* water = act->CAMBUF;
	if(0 == water)return;
	matproj_transpose(water->mvp, frus);

	//give arg(matrix and position) to fbo
	struct gl41data* data = (void*)(water->data);
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = water->mvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	fbo->glfull_camera[0] = data;
}
void water_forfbo_prepare(struct gl41data* data)
{
}




static void water_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vf);

	struct waterbuf* water = act->CTXBUF;
	if(0 == water)return;
	struct mysrc* src = (void*)(water->data);
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	water->time = (timeread()%10000000)/10000000.0;

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

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 'o', src, 1);
}
void water_forwnd_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* area)
{
	struct waterbuf* water = act->CTXBUF;
	if(0 == water)return;

	struct gl41data* data = (void*)(water->data);
	if(0 == data)return;

	data->dst.texname[0] = "tex0";
	data->src.tex[0].glfd = fbo->tex0;
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;
}
void water_forwnd_prepare(struct gl41data* data, struct waterbuf* water, char* str)
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




static void water_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if('v' == key){
		water_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	if('?' == key){
		//search for myown fbo
		int ret;
		struct halfrel* rel[2];
		ret = relationsearch(ent,_fbo_, &rel[0], &rel[1]);
		if(ret <= 0)return;

		//update matrix for fbo
		struct supply* fbo = rel[1]->pchip;
		struct style* rect = rel[1]->pfoot;
		water_forfbo_update(ent,slot, wor,geom, dup,camg, fbo,rect);

		//wnd.data -> fbo.texture
		give_data_into_peer(ent,_fbo_, stack,sp, 0,0, 0,0);

		//fbo.texture -> my.data -> wnd.data
		water_forwnd_update(ent,slot, fbo,rect);
	}
}
static void water_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void water_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void water_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void water_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void water_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void water_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	water_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void water_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void water_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void water_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void water_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static void water_modify(struct entity* act)
{
}
static void water_delete(struct entity* act)
{
}
static void water_create(struct entity* act, char* str)
{
	struct waterbuf* water;
	struct gl41data* data;
	if(0 == act)return;

	water = act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == water)return;
	data = (void*)(water->data);
	if(0 == str)str = "datafile/jpg/dudvmap.jpg";
	water_forwnd_prepare(data, water, str);

	water = act->CAMBUF = memorycreate(0x1000, 0);
	if(0 == water)return;
	data = (void*)(water->data);
	water_forfbo_prepare(data);
}




void water_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'a', 't', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)water_create;
	p->ondelete = (void*)water_delete;
	p->onsearch = (void*)water_search;
	p->onmodify = (void*)water_modify;

	p->onlinkup = (void*)water_linkup;
	p->ondiscon = (void*)water_discon;
	p->ontaking = (void*)water_taking;
	p->ongiving = (void*)water_giving;
}
