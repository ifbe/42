#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);


#define CAMBUF listptr.buf0
#define CTXBUF listptr.buf1
struct glassbuf{
	mat4 mvp;
	u8 data[0];
};




char* glass_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out vec2 uvw;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* glass_glsl_f =
GLSL_VERSION
GLSL_PRECISION
"uniform sampler2D tex0;\n"
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
	"vec3 c = texture(tex0, uvw).rgb * vec3(1.0, 0.8, 0.6);\n"
	"FragColor = vec4(c, 0.5);\n"
"}\n";




static void glass_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void glass_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void glass_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void glass_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void glass_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




//for framebuffer
void glass_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;


//-------------p,q---------------
	//glass.n
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * glass.n
	t = (cam[0] - obb->vc[0])*x
	  + (cam[1] - obb->vc[1])*y
	  + (cam[2] - obb->vc[2])*z;

	//foot of a perpendicular: q = p - t*glass.n
	frus->vq[0] = cam[0] - t*x;
	frus->vq[1] = cam[1] - t*y;
	frus->vq[2] = cam[2] - t*z;

	//reflected point: p' = p - 2*t*glass.n
	frus->vc[0] = cam[0] - 2*t*x;
	frus->vc[1] = cam[1] - 2*t*y;
	frus->vc[2] = cam[2] - 2*t*z;


//----------------n,f----------------
	//vec(frus.n) = vec(glass.n), len(frus.n) > len(cam to plane)
	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = t*1.001;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e20;


//--------------l,r--------------------
	//nr = -norm(glass.r)
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
	//nt = norm(glass.t)
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
static void glass_forfbo_update(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wrl, struct style* camg,
	_obj* fbo, struct style* area)
{
	//frus from shape and eye
	struct fstyle* shap = &geom->fshape;
	struct fstyle* frus = &geom->frustum;
	glass_frustum(frus, shap, camg->frus.vc);

	//mvp from frus
	struct glassbuf* glass = act->CAMBUF;
	if(0 == glass)return;
	world2clip_projznzp_transpose(glass->mvp, frus);

	//give arg(matrix and position) to fbo
	struct gl41data* data = (void*)(glass->data);
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = glass->mvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	fbo->gl41list.world[0].camera[0] = data;
}
static void glass_forfbo_prepare(struct gl41data* data)
{
}




//for window
static void glass_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	struct glassbuf* glass = act->CTXBUF;
	if(0 == glass)return;
	struct mysrc* src = (void*)(glass->data);
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

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
void glass_forwnd_update(_obj* act, struct style* slot, _obj* fbo, struct style* rect)
{
	struct glassbuf* glass = act->CTXBUF;
	if(0 == glass)return;

	struct gl41data* data = (void*)(glass->data);
	if(0 == data)return;

	data->dst.texname[0] = "tex0";
	//data->src.tex[0].glfd = fbo->gl41list.tex[0];
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;
}
void glass_forwnd_prepare(struct gl41data* data)
{
	//shader
	data->src.vs = glass_glsl_v;
	data->src.fs = glass_glsl_f;
	data->src.shader_enq = 42;

	//texture
	data->dst.texname[0] = "tex0";

	//vertex
	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 1;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	data->src.vbuf_enq = 0;
}








static void glass_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* dup;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	//search for myown fbo
	int ret;
	struct halfrel* rel[2];
	ret = relationsearch(ent, _fbo_, &rel[0], &rel[1]);
	if(ret <= 0)return;

	//update matrix for fbo
	_obj* fbo = rel[1]->pchip;
	struct style* rect = rel[1]->pfoot;
	glass_forfbo_update(ent,slot, wor,geom, dup,camg, fbo,rect);

	//wnd.data -> fbo.texture
	give_data_into_peer(ent,_fbo_, stack,sp, 0,0, 0,0);

	//fbo.texture -> my.data -> wnd.data
	glass_forwnd_update(ent,slot, fbo,rect);

	glass_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void glass_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		glass_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void glass_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void glass_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void glass_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void glass_search(_obj* act)
{
}
static void glass_modify(_obj* act)
{
}
static void glass_delete(_obj* act)
{
	if(0 == act)return;
	if(act->CTXBUF){
		memorydelete(act->CTXBUF);
		act->CTXBUF = 0;
	}
	if(act->CAMBUF){
		memorydelete(act->CAMBUF);
		act->CAMBUF = 0;
	}
}
static void glass_create(_obj* act, void* str)
{
	struct glassbuf* glass;
	struct gl41data* data;
	if(0 == act)return;

	glass = act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == glass)return;
	data = (void*)(glass->data);
	glass_forwnd_prepare(data);

	glass = act->CAMBUF = memorycreate(0x1000, 0);
	if(0 == glass)return;
	data = (void*)(glass->data);
	glass_forfbo_prepare(data);
}




void glass_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('g', 'l', 'a', 's', 's', 0, 0, 0);

	p->oncreate = (void*)glass_create;
	p->ondelete = (void*)glass_delete;
	p->onreader = (void*)glass_search;
	p->onwriter = (void*)glass_modify;

	p->onattach = (void*)glass_attach;
	p->ondetach = (void*)glass_detach;
	p->ontaking = (void*)glass_taking;
	p->ongiving = (void*)glass_giving;
}
