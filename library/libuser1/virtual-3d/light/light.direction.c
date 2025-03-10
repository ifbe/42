#include "libuser.h"
void world2clip_orthz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_orthznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_addcam(_obj* wnd, struct gl41data* data);
void gl41data_addlit(_obj* wnd, struct gl41data* data);
void gl41data_insert(_obj* ctx, int type, struct gl41data* src, int cnt);


#define OWNBUF listptr.buf0
struct sunbuf{
	mat4 wvp;
	vec4 rgb;
	u32 u_rgb;
	struct gl41data cam;
	struct gl41data lit;
	struct gl41data ctx;
};




char* dirlit_glsl_v =
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

char* dirlit_glsl_f =
GLSL_VERSION
GLSL_PRECISION
"uniform sampler2D shadowmap;\n"
"in vec2 uvw;\n"
"layout(location = 0)out vec4 FragColor;\n"
"void main(){\n"
	"float d = texture(shadowmap, uvw).r;\n"
	"float c = 1.0-(1.0-d)*3;\n"
	"FragColor = vec4(c, c, c, 1.0);\n"
	//"float r=(d>0.5)?1.0:0.0;\n"
	//"float g=(d>0.8)?1.0:0.0;\n"
	//"float b=1.0-(1.0-d)*2;\n"
	//"FragColor = vec4(r, g, b, 1.0);\n"
"}\n";




static void dirlight_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dirlight_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dirlight_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dirlight_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dirlight_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void dirlight_frustum(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	//left, right
	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	d->vr[3] = n;
	d->vl[0] = - d->vr[0];
	d->vl[1] = - d->vr[1];
	d->vl[2] = - d->vr[2];
	d->vl[3] = -n;


	//bottom, top
	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	d->vt[3] = n;
	d->vb[0] = - d->vt[0];
	d->vb[1] = - d->vt[1];
	d->vb[2] = - d->vt[2];
	d->vb[3] = -n;


	//near, front
	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	d->vf[3] = n;

	d->vn[0] = d->vf[0];
	d->vn[1] = d->vf[1];
	d->vn[2] = d->vf[2];
	d->vn[3] = 0.1;			//must > 0
}
static void dirlight_lit_update(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
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
	data->dst.arg[0].data = sun->wvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "sunrgb";
	data->dst.arg[1].data = sun->rgb;

	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "sundir";
	data->dst.arg[2].data = &geom->frus.vf;

	data->dst.texname[0] = "shadowmap";
	gl41data_addlit(wnd, data);
}
static void dirlight_lit_prep(struct gl41data* data)
{
	data->dst.lighttype = _dirlight_;
}




static void dirlight_cam_update(
	_obj* act, struct style* slot,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* data = &sun->cam;
	if(0 == data)return;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = sun->wvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = &geom->frus.vc;
	gl41data_addcam(wnd, data);
}
static void dirlight_cam_prep(struct mysrc* src)
{
	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'd';
	src->target_enq = 42;
}




static void dirlight_mesh_update(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y,j;
	vec3 ta, tb;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);

	//light ray (for debug)
	struct sunbuf* sun = act->OWNBUF;
	for(y=-1;y<2;y+=2){
	for(x=-4;x<=4;x++){
		for(j=0;j<3;j++){
			ta[j] = vc[j] + x*vr[j]/4.0 + y*vt[j];
			tb[j] = ta[j] + vf[j];
		}
		gl41line(ctx, sun->u_rgb, ta, tb);
	}
	}
	for(x=-1;x<2;x+=2){
	for(y=-4;y<=4;y++){
		for(j=0;j<3;j++){
			ta[j] = vc[j] + x*vr[j] + y*vt[j]/4.0;
			tb[j] = ta[j] + vf[j];
		}
		gl41line(ctx, sun->u_rgb, ta, tb);
	}
	}

	//depth fbo (for debug)
	struct gl41data* mesh = &sun->ctx;
	if(0 == mesh)return;
	float (*vbuf)[6] = (void*)(mesh->src.vtx[0].vbuf);
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vt[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	mesh->src.vbuf_enq += 1;
	gl41data_insert(ctx, 's', mesh, 1);
}
static void dirlight_mesh_prep(struct mysrc* src)
{
	//shader
	src->vs = dirlit_glsl_v;
	src->fs = dirlit_glsl_f;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
}




static void dirlight_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	}
}
static void dirlight_read_byworld_bycam_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	if(0 == stack)return;

	_obj* wor;struct style* geom;
	_obj* dup;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	struct sunbuf* sun = ent->OWNBUF;
	dirlight_frustum(&geom->frus, &geom->fs);

	switch(wnd->vfmt){
	case _gl41list_:
		world2clip_orthz0z1_transpose(sun->wvp, &geom->frus);

		dirlight_cam_update(ent,foot, wor,geom, wnd,area);
		dirlight_lit_update(ent,foot, wor,geom, wnd,area);
		dirlight_mesh_update(ent,foot, wor,geom, wnd,area);
		break;
	default:
		break;
	}
}




static void dirlight_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		dirlight_read_bywnd(ent,slot, caller,area);
		break;
	default:
		dirlight_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void dirlight_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void dirlight_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void dirlight_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void dirlight_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
	struct fstyle* obb = 0;
	//logtoall("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

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
static void dirlight_modify(_obj* act)
{
}
static void dirlight_delete(_obj* act)
{
}
static void dirlight_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct sunbuf* sun = act->OWNBUF = memoryalloc(0x10000, 0);
	sun->u_rgb = 0xffff00;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	dirlight_cam_prep(&sun->cam.src);

	dirlight_lit_prep(&sun->lit);

	dirlight_mesh_prep(&sun->ctx.src);
}




void dirlight_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('d','i','r','l', 'i', 't', 0, 0);

	p->oncreate = (void*)dirlight_create;
	p->ondelete = (void*)dirlight_delete;
	p->onreader = (void*)dirlight_search;
	p->onwriter = (void*)dirlight_modify;

	p->onattach = (void*)dirlight_attach;
	p->ondetach = (void*)dirlight_detach;
	p->ontaking = (void*)dirlight_taking;
	p->ongiving = (void*)dirlight_giving;
}
