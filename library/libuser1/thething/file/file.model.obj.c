#include "libuser.h"
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);
//
void loadtexfromcolor(struct texture* tex, u32 rgb, int w, int h);
void parsevertfromobj(struct mysrc* ctx, struct fstyle* sty, u8* buf, int len);
//
void world2local(mat4 mat, struct fstyle* src, struct fstyle* dst);
void world2local_transpose(mat4 mat, struct fstyle* src, struct fstyle* dst);
//
void local2world(mat4 mat, struct fstyle* src, struct fstyle* dst);
void local2world_transpose(mat4 mat, struct fstyle* src, struct fstyle* dst);
//
void mat4_transposefrom(mat4, mat4);
void mat4_multiplyfrom(mat4, mat4, mat4);
//
int rastersolid_triangle(void*,void*, void*,void*, float*,int,int,int, mat4,void*);


struct privdata{
	u8 vs[128];
	u8 fs[128];

	u8 albedo[128];
	u8 matter[128];
	u8 normal[128];

	int objlen;
	u8* objbuf;

	mat4 objmat;
	struct gl41data gl41;
};
static void obj3d_position(vec4 olist[], vec3 ilist[], mat4 m)
{
	float* oo = olist[0];
	float* ov = olist[1];
	float* on = olist[2];
	float* ot = olist[3];
	float* iv = ilist[0];
	float* in = ilist[1];
	float* it = ilist[2];

	ov[0] = iv[0];ov[1] = iv[1];ov[2] = iv[2];
	on[0] = in[0];on[1] = in[1];on[2] = in[2];
	ot[0] = it[0];ot[1] = it[1];ot[2] = it[2];

	//gl_Position = m*v
	oo[0] = m[0][0]*iv[0] + m[0][1]*iv[1] + m[0][2]*iv[2] + m[0][3];
	oo[1] = m[1][0]*iv[0] + m[1][1]*iv[1] + m[1][2]*iv[2] + m[1][3];
	oo[2] = m[2][0]*iv[0] + m[2][1]*iv[1] + m[2][2]*iv[2] + m[2][3];
	oo[3] = m[3][0]*iv[0] + m[3][1]*iv[1] + m[3][2]*iv[2] + m[3][3];
}
static u32 obj3d_fragment(vec4 out[], vec4 in[], struct privdata* own)
{
	//float* v = in[0];		//in vec3 v
	//float* n = in[1];		//in vec3 n
	float* t = in[2];		//in vec3 t
	//say("%f,%f,%f\n",t[0],t[1],t[2]);
/*
	float w = 1.0;
	u32 b = (u32)(256*(t[0]*w));
	u32 g = (u32)(256*(t[1]*w));
	u32 r = (u32)(256*(t[2]*w));
	return (r<<16) + (g<<8) + (b);*/
	struct mysrc* src = &own->gl41.src;
	u32* tex = src->tex[0].data;
	int w = src->tex[0].w;
	int h = src->tex[0].h;
	int x = w*t[0];
	int y = h*t[1];
	if(x < 0)x = 0;
	if(x > w-1)x = w-1;
	if(y < 0)y = 0;
	if(y > h-1)y = h-1;

	u32 rgba = tex[y*w+x];
	u32 r = rgba&0xff;
	u32 b = (rgba>>16)&0xff;
	rgba &= 0xff00ff00;
	return rgba|(r<<16)|b;
}


static void obj3d_ctxforgl41(struct gl41data* data, char* albedo, char* matter, char* normal, char* vs, char* fs)
{
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;
//say("%s\n%s\n%s\n%s\n%s\n",albedo,matter,normal,vs,fs);

	//shader
	src->vs = memorycreate(0x10000, 0);
	openreadclose(vs, 0, src->vs, 0x10000);
	src->fs = memorycreate(0x10000, 0);
	openreadclose(fs, 0, src->fs, 0x10000);
	src->shader_enq = 42;

	//argument
	dst->arg[0].fmt = 'm';
	dst->arg[0].name = "objmat";

	//albedo
	dst->texname[0] = "albedomap";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], albedo);
	src->tex_enq[0] = 42;

	//matter
	dst->texname[1] = "mattermap";
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	if(matter)loadtexfromfile(&src->tex[1], matter);
	else loadtexfromcolor(&src->tex[1], 0xff8001, 2048, 2048);
	src->tex_enq[1] = 42;

	//normal
	dst->texname[2] = "normalmap";
	src->tex[2].fmt = hex32('r','g','b','a');
	src->tex[2].data = memorycreate(2048*2048*4, 0);
	if(normal)loadtexfromfile(&src->tex[2], normal);
	else loadtexfromcolor(&src->tex[2], 0x0000ff, 2048, 2048);
	src->tex_enq[2] = 42;

	//vertex
	src->vtx[0].geometry = 3;
	src->vtx[0].opaque = 0;
	//src->vtx[0].fill = 2;
}
static void obj3d_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* none)
{
	struct privdata* own = act->OWNBUF;
	if(0 == own)return;

	local2world_transpose(own->objmat, &part->fs, &geom->fs);

	struct mysrc* src = &own->gl41.src;
	gl41data_insert(ctx, 's', src, 1);
}



static void obj3d_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}
static void obj3d_draw_raster(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* wnd, struct style* area,
	mat4 clip_from_world)
{
	struct privdata* own = act->OWNBUF;
	if(0 == own)return;

	mat4 m,world_from_local;
	local2world(world_from_local, &part->fs, &geom->fs);
	mat4_multiplyfrom(m, clip_from_world, world_from_local);

	struct vertex* vtx = own->gl41.src.vtx;
	rastersolid_triangle(
		wnd, area, obj3d_position, obj3d_fragment,
		vtx->vbuf, 12, 12*3, vtx->vbuf_h/3,
		m, own);
}
static void obj3d_draw_raytrace(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* wnd, struct style* area,
	mat4 clip_from_world)
{
}




static void obj3d_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void obj3d_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("obj3d(%x,%x,%x)\n",win,act,sty);
}




static void obj3d_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
}




static void obj3d_world_camera_window(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key)
{
	struct entity* scn;struct style* geom;
	struct entity* wrd;struct style* camg;
	struct entity* wnd;struct style* area;

	scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	if(_rgba_ == wnd->fmt){
		if(0==key)obj3d_draw_raster(ent,slot, scn,geom, wrd,camg, wnd,area, arg);
		else obj3d_draw_raytrace(ent,slot, scn,geom, wrd,camg, wnd,area, arg);
		return;
	}

	obj3d_draw_gl41(ent,slot, scn,geom, wrd,camg, wnd,area);
}
static void obj3d_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		break;
	default:
		obj3d_world_camera_window(ent,slot, stack,sp, arg,key);
	}
}
static void obj3d_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void obj3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void obj3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = self->pchip;
	if(0 == act)return;
	struct privdata* own = act->buf0;
	if(0 == own)return;

	struct style* pin = self->pfoot;
	if(0 == pin)return;
	//struct style* sty = peer->pfoot;
	//if(0 == sty)return;

	//vertex
	struct mysrc* src = &own->gl41.src;
	src->vtx[0].vbuf_len = 0x100000;
	src->vtx[0].vbuf = memorycreate(src->vtx[0].vbuf_len, 0);
	src->vtx[0].vbuf_fmt = vbuffmt_3333;
	src->vtx[0].vbuf_w = 4*12;
	parsevertfromobj(src, &pin->fs, own->objbuf, own->objlen);
	src->vbuf_enq = 42;
}




static void obj3d_search(struct entity* act)
{
}
static void obj3d_modify(struct entity* act)
{
}
static void obj3d_delete(struct entity* act)
{
	if(0 == act)return;
}
static void obj3d_create(struct entity* act, void* arg, int argc, u8** argv)
{
	int j;
	if(0 == act)return;

	struct privdata* own = act->OWNBUF = memorycreate(0x10000, 0);
	if(0 == own)return;

	//char* dxvs = 0;
	//char* dxfs = 0;
	char* glvs = 0;
	char* glfs = 0;
	char* albedo = 0;
	char* matter = 0;
	char* normal = 0;
	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "glvs:", 5)){
			copypath(own->vs, argv[j]+5);
			glvs = (void*)own->vs;
		}
		if(0 == ncmp(argv[j], "glfs:", 5)){
			copypath(own->fs, argv[j]+5);
			glfs = (void*)own->fs;
		}
		if(0 == ncmp(argv[j], "albedo:", 7)){
			copypath(own->albedo, argv[j]+7);
			albedo = (void*)own->albedo;
		}
		if(0 == ncmp(argv[j], "matter:", 7)){
			copypath(own->matter, argv[j]+7);
			matter = (void*)own->matter;
		}
		if(0 == ncmp(argv[j], "normal:", 7)){
			copypath(own->normal, argv[j]+7);
			normal = (void*)own->normal;
		}
	}
	//if(0 == dxvs)dxvs = "datafile/shader/obj/dxfv.glsl";
	//if(0 == dxfs)dxfs = "datafile/shader/obj/dxff.glsl";
	if(0 == glvs)glvs = "datafile/shader/obj/glfv.glsl";
	if(0 == glfs)glfs = "datafile/shader/obj/glff.glsl";
	if(0 == albedo)albedo = "datafile/jpg/wall.jpg";
	//if(0 == matter)matter = "datafile/jpg/wallnormal.jpg";
	//if(0 == normal)normal = "datafile/jpg/wallnormal.jpg";

	//obj3d_ctxforgl41(&own->gl41.src, albedo, normal, dxvs, dxfs);
	obj3d_ctxforgl41(&own->gl41, albedo, matter, normal, glvs, glfs);
	own->gl41.dst.arg[0].data = own->objmat;

	if(0 == arg)arg = "datafile/obj/cube.obj";
	own->objbuf = memorycreate(0x200000, 0);
	own->objlen = openreadclose(arg, 0, own->objbuf, 0x100000);
}




void obj3d_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'b', 'j', '3', 'd', 0, 0, 0);

	p->oncreate = (void*)obj3d_create;
	p->ondelete = (void*)obj3d_delete;
	p->onsearch = (void*)obj3d_search;
	p->onmodify = (void*)obj3d_modify;

	p->onlinkup = (void*)obj3d_linkup;
	p->ondiscon = (void*)obj3d_discon;
	p->ontaking = (void*)obj3d_taking;
	p->ongiving = (void*)obj3d_giving;
}
