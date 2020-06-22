#include "libuser.h"
#define CTXBUF buf0
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* texmix_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* texmix_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D tex1;\n"
"void main(){\n"
	"mediump vec3 c0 = texture(tex0, uvw).bgr;\n"
	"mediump vec3 c1 = texture(tex1, uvw).bgr;\n"
	"FragColor = vec4((c0+c1)*0.5, 1.0);\n"
"}\n";
void texmix_ctxforwnd(struct glsrc* src)
{
	//shader
	src->vs = texmix_glsl_v;
	src->fs = texmix_glsl_f;
	src->shader_enq = 42;


	//texture
	struct texture* tex;

	tex = &src->tex[0];
	tex->fmt = hex32('r','g','b','a');
	tex->name = "tex0";
	tex->data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(tex, "datafile/jpg/wall.jpg");

	tex = &src->tex[1];
	tex->fmt = hex32('r','g','b','a');
	tex->name = "tex1";
	tex->data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(tex, "datafile/jpg/cartoon.jpg");

	src->tex_enq[0] = 42;
	src->tex_enq[1] = 42;


	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	src->vbuf_enq = 42;
}
static void texmix_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	if(0 == act->CTXBUF)return;

	struct glsrc* src = act->CTXBUF;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static int texmix_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static void texmix_modify(struct entity* act)
{
}
static void texmix_delete(struct entity* act)
{
	if(0 == act)return;
}
static void texmix_create(struct entity* act, void* str)
{
	if(0 == act)return;

	act->CTXBUF = memorycreate(0x200, 0);
	texmix_ctxforwnd(act->CTXBUF);
}





static void texmix_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texmix_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("texmix(%x,%x,%x)\n",win,act,sty);
}




static void texmix_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		texmix_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
//say("@freecam_read_byeye.end\n");
}
static void texmix_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	gl41data_before(wnd);
	texmix_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static int texmix_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		texmix_read_bywnd(ent,slot, wnd,area);break;
	}
	default:{
		texmix_read_bycam(ent,foot, stack,sp, arg,key);break;
	}
	}
	return 0;
}
static void texmix_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void texmix_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void texmix_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void texmix_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'm', 'i', 'x', 0, 0);

	p->oncreate = (void*)texmix_create;
	p->ondelete = (void*)texmix_delete;
	p->onsearch = (void*)texmix_search;
	p->onmodify = (void*)texmix_modify;

	p->onlinkup = (void*)texmix_linkup;
	p->ondiscon = (void*)texmix_discon;
	p->ontaking = (void*)texmix_taking;
	p->ongiving = (void*)texmix_giving;
}
