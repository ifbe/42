#include "libuser.h"
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_insert(_obj* ctx, int type, struct gl41data* data, int cnt);
void gl41data_addcam(_obj* wnd, struct gl41data* data);


#define CTXBUF listptr.buf0
struct mirrbuf{
	mat4 wvp;
	struct gl41data geom;
	struct gl41data dest;
};




char* mirror_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* mirror_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump vec3 c = 0.9*texture(tex0, uvw).rgb + vec3(0.1);\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




static void mirror_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mirror_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mirror_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mirror_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mirror_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mirror_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;

//-------------p,q---------------
	//mirror.n
	x = obb->vt[0];
	y = obb->vt[1];
	z = obb->vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * mirror.n
	t = (cam[0] - obb->vc[0])*x
	  + (cam[1] - obb->vc[1])*y
	  + (cam[2] - obb->vc[2])*z;

	//foot of a perpendicular: q = p - t*mirror.n
	frus->vq[0] = cam[0] - t*x;
	frus->vq[1] = cam[1] - t*y;
	frus->vq[2] = cam[2] - t*z;

	//reflected point: p' = p - 2*t*mirror.n
	frus->vc[0] = cam[0] - 2*t*x;
	frus->vc[1] = cam[1] - 2*t*y;
	frus->vc[2] = cam[2] - 2*t*z;


//----------------n,f----------------
	//vec(frus.n) = vec(mirr.n), len(frus.n) > len(cam to plane)
	frus->vn[0] = x;
	frus->vn[1] = y;
	frus->vn[2] = z;
	frus->vn[3] = t*1.001;

	frus->vf[0] = x;
	frus->vf[1] = y;
	frus->vf[2] = z;
	frus->vf[3] = 1e20;


//--------------l,r--------------------
	//nr = -norm(mirror.r)
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
	//nt = norm(mirror.t)
	x = obb->vf[0];
	y = obb->vf[1];
	z = obb->vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len = (b-q) * nt
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
static void mirror_gl41fbo_update(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wrl, struct style* camg,
	_obj* wnd, struct style* area)
{
	struct mirrbuf* mirr = act->CTXBUF;
	struct gl41data* data = &mirr->dest;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = mirr->wvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = geom->frus.vc;
	gl41data_addcam((void*)wnd, data);
}
void mirror_gl41fbo_prepare(struct mysrc* src)
{
	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'c';
	src->target_enq = 42;
}




static void mirror_gl41geom_update(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//say("%f,%f,%f	%f,%f,%f\n",vc[0],vc[1],vc[2],vr[0],vf[1],vu[2]);
	gl41line_rect(wnd, 0x404040, vc, vr, vf);


	struct mirrbuf* mirr = act->CTXBUF;
	if(0 == mirr)return;
	struct gl41data* body = &mirr->geom;
	if(0 == body)return;
	struct gl41data* dest = &mirr->dest;
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
	gl41data_insert(wnd, 's', body, 1);
}
void mirror_gl41geom_prepare(struct mysrc* src)
{
	//shader
	src->vs = mirror_glsl_v;
	src->fs = mirror_glsl_f;
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
	src->vbuf_enq = 0;
}




//[-6,-5]: wnd,area -> cam,togl
//[-4,-3]: cam,gl41 -> wor,camg		//the camera taking photo
//[-2,-1]: wor,geom -> ent,gl41		//the entity being taken
static void mirror_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	if(0 == stack)return;
	_obj* wor;struct style* geom;
	_obj* dup;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	//frus from shape and eye
	struct mirrbuf* mirr = ent->CTXBUF;
	mirror_frustum(&geom->frus, &geom->fshape, camg->frus.vc);

	//wvp from frus
	if(_gl41list_ == wnd->hfmt)world2clip_projznzp_transpose(mirr->wvp, &geom->frus);
	else world2clip_projz0z1_transpose(mirr->wvp, &geom->frus);

	//create or update fbo
	mirror_gl41fbo_update(ent,slot, wor,geom, dup,camg, (void*)wnd,area);

	//geom
	mirror_gl41geom_update(ent,slot, wor,geom, wnd,area);
}




static void mirror_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
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
		mirror_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void mirror_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mirror_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mirror_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mirror_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
}
static void mirror_modify(_obj* act)
{
}
static void mirror_delete(_obj* act)
{
}
static void mirror_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct mirrbuf* mirr = act->CTXBUF = memorycreate(0x10000, 0);
	if(0 == mirr)return;

	mirror_gl41fbo_prepare(&mirr->dest.src);
	mirror_gl41geom_prepare(&mirr->geom.src);
}




void mirror_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'i', 'r', 'r', 'o', 'r', 0, 0);

	p->oncreate = (void*)mirror_create;
	p->ondelete = (void*)mirror_delete;
	p->onsearch = (void*)mirror_search;
	p->onmodify = (void*)mirror_modify;

	p->onlinkup = (void*)mirror_linkup;
	p->ondiscon = (void*)mirror_discon;
	p->ontaking = (void*)mirror_taking;
	p->ongiving = (void*)mirror_giving;
}
