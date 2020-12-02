#include "libuser.h"
void matproj_transpose(void* m, struct fstyle* sty);
void gl41data_addcam(struct entity* wnd, struct gl41data* data);
void gl41data_addlit(struct entity* wnd, struct gl41data* data);
void gl41data_insert(struct entity* ctx, int type, struct gl41data* src, int cnt);


#define OWNBUF buf0
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	struct gl41data cam;
	struct gl41data lit;
	struct gl41data ctx;
};




char* projector_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* projector_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"uniform sampler2D shadowmap;\n"
"uniform sampler2D prjtormap;\n"
"void main(){\n"
	"mediump float n = 1.0;\n"
	"mediump float f = 10000.0;\n"
	"mediump float d = texture(shadowmap, uvw).r;\n"
	"mediump float c = (2.0 * n) / (f + n - d * (f - n));\n"
	"mediump vec3 tmp = 0.9*vec3(c) + 0.1*texture(prjtormap, uvw).bgr;\n"
	"FragColor = vec4(tmp, 1.0);\n"
"}\n";




static void projector_frustum(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	//d->vr[3] = 1.0;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -1.0;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 1.0;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -1.0;


	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	//d->vn[3] = 1.0;
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 1e20;
}
static void projector_lit_update(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	struct gl41data* data = &sun->lit;
	if(0 == data)return;

	//common
	data->src.tex[0].glfd = sun->cam.dst.tex[0];
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;

	//gl41
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "sunmvp";
	data->dst.arg[0].data = sun->mvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "sunrgb";
	data->dst.arg[1].data = sun->rgb;

	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "sunxyz";
	data->dst.arg[2].data = geom->frus.vc;

	data->dst.arg[3].fmt = 'v';
	data->dst.arg[3].name = "sundir";
	data->dst.arg[3].data = geom->frus.vf;

	data->dst.texname[0] = "shadowmap";
	gl41data_addlit(wnd, data);
}
static void projector_lit_prepare(struct gl41data* data)
{
	data->dst.routine_name = "passtype";
	data->dst.routine_detail = "projector";
	data->dst.texname[1] = "prjtormap";

	data->src.tex[1].fmt = hex32('r','g','b','a');
	data->src.tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&data->src.tex[1], "datafile/jpg/cartoon.jpg");
	data->src.tex_enq[1] = 42;
}




static void projector_cam_update(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* data = &sun->cam;
	if(0 == data)return;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = sun->mvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = &geom->frus.vc;

	gl41data_addcam(wnd, data);
}
static void projector_cam_prepare(struct mysrc* src)
{
	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'd';
	src->target_enq = 42;
}




static void projector_mesh_update(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);

	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	gl41solid_cone(ctx, 0xffffff, vc, vr, tt);

	//depth fbo (for debug)
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* mesh = &sun->ctx;
	if(0 == mesh)return;
	float (*vbuf)[6] = (void*)(mesh->src.vtx[0].vbuf);
	if(0 == vbuf)return;

//.vertex
	vbuf[0][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0] - vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1] - vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2] - vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	mesh->src.vbuf_enq += 1;
	gl41data_insert(ctx, 's', mesh, 1);
}
static void projector_mesh_prepare(struct mysrc* src)
{
	//shader
	src->vs = projector_glsl_v;
	src->fs = projector_glsl_f;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}




static void projector_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	struct sunbuf* sun = ent->OWNBUF;
	projector_frustum(&geom->frus, &geom->fs);
	matproj_transpose(sun->mvp, &geom->frus);

	projector_cam_update(ent,foot, wor,geom, wnd,area);
	projector_lit_update(ent,foot, wor,geom, wnd,area);
	projector_mesh_update(ent,foot, wor,geom, wnd,area);
}
static void projector_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void projector_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void projector_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void projector_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void projector_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void projector_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	projector_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void projector_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void projector_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void projector_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void projector_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static void projector_modify(struct entity* act)
{
}
static void projector_delete(struct entity* act)
{
}
static void projector_create(struct entity* act, void* str)
{
	struct sunbuf* sun;
	if(0 == act)return;

	sun = act->OWNBUF = memorycreate(0x10000, 0);
	sun->rgb[0] = 1.0;
	sun->rgb[1] = 1.0;
	sun->rgb[2] = 1.0;

	projector_cam_prepare(&sun->cam.src);

	projector_lit_prepare(&sun->lit);

	projector_mesh_prepare(&sun->ctx.src);
}




void projector_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p','r','j','t','o', 'r', 0, 0);

	p->oncreate = (void*)projector_create;
	p->ondelete = (void*)projector_delete;
	p->onsearch = (void*)projector_search;
	p->onmodify = (void*)projector_modify;

	p->onlinkup = (void*)projector_linkup;
	p->ondiscon = (void*)projector_discon;
	p->ontaking = (void*)projector_taking;
	p->ongiving = (void*)projector_giving;
}
