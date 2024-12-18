#include "libuser.h"
int copypath(u8* path, u8* data);
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);


char* dbggnd_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"out vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* dbggnd_glsl_f =
GLSL_VERSION
GLSL_PRECISION
"in vec2 uvw;\n"
"layout(location = 0)out vec4 FragColor;\n"
"void main(){\n"
	"float x = mod(abs(uvw.x), 1000.0);\n"
	"float y = mod(abs(uvw.y), 1000.0);\n"
	"if( (x>5.0) && (x<995.0) && (y>5.0) && (y<995.0) )discard;\n"
	"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

struct privdata{
	struct gl41data gl41;
};
void dbggnd_singlepiece(float (*vbuf)[6], float* vc,float* vr,float* vf)
{
	vbuf[0][3] = vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][4] = vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][5] = vbuf[0][2] = vc[2] - vr[2] - vf[2];

	vbuf[1][3] = vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][4] = vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][5] = vbuf[1][2] = vc[2] + vr[2] + vf[2];

	vbuf[2][3] = vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][4] = vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][5] = vbuf[2][2] = vc[2] - vr[2] + vf[2];

	vbuf[3][3] = vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][4] = vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][5] = vbuf[3][2] = vc[2] + vr[2] + vf[2];

	vbuf[4][3] = vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][4] = vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][5] = vbuf[4][2] = vc[2] - vr[2] - vf[2];

	vbuf[5][3] = vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][4] = vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][5] = vbuf[5][2] = vc[2] + vr[2] - vf[2];
}




static void dbggnd_gl41_prep(struct gl41data* data, char* vs, char* fs)
{
	//shader
	data->src.vs = vs;
	data->src.fs = fs;
	data->src.shader_enq = 42;

	//vertex
	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*3*2;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
}
static void dbggnd_gl41_draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_rect(wnd, 0xffffff, vc, vr, vf);

	struct privdata* own = act->priv_ptr;
	if(0 == own)return;
	struct mysrc* src = &own->gl41.src;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	dbggnd_singlepiece(vbuf, vc, vr, vf);

	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}




static void dbggnd_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dbggnd_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dbggnd_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dbggnd_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dbggnd_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void dbggnd_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void dbggnd_read_bymgr_bywnd(_obj* ent,void* foot, _obj* mgr,void* mgrarea, _syn* stack,int sp)
{
	_obj* wnd = stack[sp-4].pchip;
	struct style* wndarea = stack[sp-4].pfoot;
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void dbggnd_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->vfmt){
	case _gl41list_:dbggnd_gl41_draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void dbggnd_read_byworld_bycam_bymgr_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-8].pchip;area = stack[sp-6].pfoot;

	switch(wnd->vfmt){
	case _gl41list_:dbggnd_gl41_draw(ent,slot, wor,geom, wnd,area);break;
	}
}




static void dbggnd_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		dbggnd_read_bywnd(ent,slot, caller,area);
		break;
	case _camrts_:
		dbggnd_read_bymgr_bywnd(ent,slot, caller,area, stack,sp);
		break;
	default:
		dbggnd_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void dbggnd_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void dbggnd_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void dbggnd_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void dbggnd_search(_obj* act)
{
}
static void dbggnd_modify(_obj* act)
{
}
static void dbggnd_delete(_obj* act)
{
}
static void dbggnd_create(_obj* act, void* str, int argc, u8** argv)
{
	if(0 == act)return;

	struct privdata* own = act->priv_ptr = memoryalloc(0x1000, 0);
	if(0 == own)return;

	dbggnd_gl41_prep(&own->gl41, dbggnd_glsl_v, dbggnd_glsl_f);
}




void dbggnd_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('d', 'b', 'g', 'g', 'n', 'd', 0, 0);

	p->oncreate = (void*)dbggnd_create;
	p->ondelete = (void*)dbggnd_delete;
	p->onreader = (void*)dbggnd_search;
	p->onwriter = (void*)dbggnd_modify;

	p->onattach = (void*)dbggnd_attach;
	p->ondetach = (void*)dbggnd_detach;
	p->ontaking = (void*)dbggnd_taking;
	p->ongiving = (void*)dbggnd_giving;
}
