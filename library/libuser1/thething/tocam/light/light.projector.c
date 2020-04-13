#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void matproj_transpose(void* m, struct fstyle* sty);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


#define CAMBUF buf0
#define LITBUF buf1
#define CTXBUF buf2
#define OWNBUF buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 glfd;
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
"uniform sampler2D suntex;\n"
"uniform sampler2D sunimg;\n"
"void main(){\n"
	//"FragColor = vec4(texture(tex0, uvw).rgb, 1.0);\n"
	"mediump float n = 1.0;"
	"mediump float f = 10000.0;"
	"mediump float d = texture(suntex, uvw).r;"
	"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
	"FragColor = vec4(c*texture(sunimg, uvw).bgr, 1.0);\n"
"}\n";




static void projector_forfbo_update(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	struct glsrc* src = act->CAMBUF;
	if(0 == src)return;

	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = sun->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = &geom->frus.vc;

	fbo->gl_camera[0] = act->CAMBUF;
}
static void projector_forfbo_prepare(struct glsrc* src)
{
}




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
static void projector_forwnd_light_update(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	projector_frustum(&geom->frus, &geom->fs);
	matproj_transpose(sun->mvp, &geom->frus);

	struct glsrc* src = act->LITBUF;
	if(0 == src)return;

	src->arg[0].fmt = 'm';
	src->arg[0].name = "sunmvp";
	src->arg[0].data = sun->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "sunrgb";
	src->arg[1].data = sun->rgb;

	src->arg[2].fmt = 'v';
	src->arg[2].name = "sunxyz";
	src->arg[2].data = geom->frus.vc;

	src->arg[3].fmt = 'v';
	src->arg[3].name = "sundir";
	src->arg[3].data = geom->frus.vf;

	src->tex[0].glfd = sun->glfd;
	src->tex[0].name = "suntex";
	src->tex[0].fmt = '!';
	src->tex[0].enq += 1;

	wnd->gl_light[0] = act->LITBUF;
}
static void projector_forwnd_light_prepare(struct glsrc* src)
{
	src->routine_name = "passtype";
	src->routine_detail = "projector";

	src->tex[1].name = "sunimg";
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(src, 1, "datafile/jpg/cartoon.jpg");
	src->tex[1].enq = 42;
}




static void projector_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun;
	struct glsrc* src;
	float (*vbuf)[6];

	vec3 tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);

	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	gl41solid_cone(ctx, 0xffffff, vc, vr, tt);

	src = act->CTXBUF;
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	//depth fbo (for debug)
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

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 's', act->CTXBUF, 1);
}
static void projector_forwnd_vertex_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	sun->glfd = fbo->tex0;
}
static void projector_forwnd_vertex_prepare(struct glsrc* src)
{
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = projector_glsl_v;
	src->fs = projector_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




static void projector_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		projector_forwnd_light_update(ent,slot, wor,geom, wnd,area);
		projector_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	if('?' == key){
		//search for myown fbo
		int ret;
		struct halfrel* rel[2];
		ret = relationsearch(ent, _fbo_, &rel[0], &rel[1]);
		if(ret <= 0)return;

		//update matrix for fbo
		struct supply* fbo = rel[1]->pchip;
		struct style* rect = rel[1]->pfoot;
		projector_forfbo_update(ent,slot, wor,geom, fbo,rect);

		//wnd.data -> fbo.texture
		relationwrite(ent,_fbo_, stack,sp, 0,0, 0,0);

		//fbo.texture -> my.data -> wnd.data
		projector_forwnd_vertex_update(ent,slot, fbo,rect);
	}
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




static void projector_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	projector_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void projector_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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

	sun = act->OWNBUF = memorycreate(0x400, 0);
	sun->rgb[0] = 1.0;
	sun->rgb[1] = 1.0;
	sun->rgb[2] = 1.0;

	//
	act->CAMBUF = memorycreate(0x400, 0);
	projector_forfbo_prepare(act->CAMBUF);

	act->LITBUF = memorycreate(0x400, 0);
	projector_forwnd_light_prepare(act->LITBUF);

	act->CTXBUF = memorycreate(0x400, 0);
	projector_forwnd_vertex_prepare(act->CTXBUF);
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
	p->onread  = (void*)projector_read;
	p->onwrite = (void*)projector_write;
}
