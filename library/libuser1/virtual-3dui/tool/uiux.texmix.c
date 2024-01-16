#include "libuser.h"
#define CTXBUF listptr.buf0
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




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
void texmix_ctxforwnd(struct gl41data* data)
{
	//shader
	data->src.vs = texmix_glsl_v;
	data->src.fs = texmix_glsl_f;
	data->src.shader_enq = 42;


	//texture
	struct texture* tex;
	data->dst.texname[0] = "tex0";
	data->dst.texname[1] = "tex1";

	tex = &data->src.tex[0];
	tex->fmt = hex32('r','g','b','a');
	tex->data = memoryalloc(2048*2048*4, 0);
	loadtexfromfile(tex, "datafile/jpg/wall.jpg");

	tex = &data->src.tex[1];
	tex->fmt = hex32('r','g','b','a');
	tex->data = memoryalloc(2048*2048*4, 0);
	loadtexfromfile(tex, "datafile/jpg/cartoon.jpg");

	data->src.tex_enq[0] = 42;
	data->src.tex_enq[1] = 42;


	//vertex
	struct vertex* vtx = &data->src.vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);

	data->src.vbuf_enq = 42;
}
static void texmix_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	if(0 == act->CTXBUF)return;

	struct mysrc* src = act->CTXBUF;
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




static int texmix_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;

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
static void texmix_modify(_obj* act)
{
}
static void texmix_delete(_obj* act)
{
	if(0 == act)return;
}
static void texmix_create(_obj* act, void* str)
{
	if(0 == act)return;

	act->CTXBUF = memoryalloc(0x1000, 0);
	texmix_ctxforwnd(act->CTXBUF);
}





static void texmix_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texmix_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texmix_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texmix_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texmix_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("texmix(%x,%x,%x)\n",win,act,sty);
}




static void texmix_read_bycam(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	texmix_draw_gl41(ent,slot, wor,geom, wnd,area);
//logtoall("@freecam_read_byeye.end\n");
}
static void texmix_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
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




static int texmix_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	if(0 == stack)return 0;

	switch(wnd->hfmt){
	case _gl41list_:
		texmix_read_bywnd(ent,slot, wnd,area);
		break;
	default:
		texmix_read_bycam(ent,slot, stack,sp, arg,key);
		break;
	}
	return 0;
}
static void texmix_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void texmix_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void texmix_attach(struct halfrel* self, struct halfrel* peer)
{
}




void texmix_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t', 'e', 'x', 'm', 'i', 'x', 0, 0);

	p->oncreate = (void*)texmix_create;
	p->ondelete = (void*)texmix_delete;
	p->onreader = (void*)texmix_search;
	p->onwriter = (void*)texmix_modify;

	p->onattach = (void*)texmix_attach;
	p->ondetach = (void*)texmix_detach;
	p->ontaking = (void*)texmix_taking;
	p->ongiving = (void*)texmix_giving;
}
