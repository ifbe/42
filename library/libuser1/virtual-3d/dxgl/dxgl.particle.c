#include "libuser.h"
#define F32BUF listptr.buf0
#define CTXBUF listptr.buf1
#define COUNT (0x100000/36)
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);



char* particle_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 s;\n"
"layout(location = 2)in mediump vec3 c;\n"
"out mediump vec3 rgb;\n"
"uniform mat4 objmat;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"rgb = c;\n"
	"gl_Position = cammvp * objmat * vec4(v, 1.0);\n"
"}\n";

char* particle_glsl_g =
GLSL_VERSION
"layout(points) in;\n"
//"layout(points, max_vertices = 6) out;\n"
//"layout(line_strip, max_vertices = 6) out;\n"
"layout(triangle_strip, max_vertices = 6) out;\n"
"in mediump vec3 rgb[];\n"
"out mediump vec3 colour;\n"
"void build_house(vec4 position){\n"    
	"gl_Position = position + vec4(-2.0, 0.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 0.0, -2.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 0.0,  2.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 2.0,  0.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
"}\n"
"void main(){\n"
	"build_house(gl_in[0].gl_Position);\n"
	"EndPrimitive();\n"
"}\n";

char* particle_glsl_f =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";

void particle_ctxforwnd(struct gl41data* data, float* vbuf)
{
	struct mysrc* src = &data->src;
	struct gldst* dst = &data->dst;

	//shader
	src->vs = particle_glsl_v;
	src->gs = particle_glsl_g;
	src->fs = particle_glsl_f;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 1;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
	vtx->vbuf_h = COUNT;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = vbuf;

	//argument
	dst->arg[0].name = "objmat";
	dst->arg[0].data = memoryalloc(4*4*4, 0);
	dst->arg[0].fmt = 'm';
}



static void particle_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void particle_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	float x,y,z;
	float* mat;
	float* buf;

	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vu);


	struct gl41data* data = act->CTXBUF;
	if(0 == data)return;

	mat = (void*)data->dst.arg[0].data;
	if(0 == mat)return;

	buf = data->src.vtx[0].vbuf;
	if(0 == buf)return;


	//update matrix
	mat[ 0] = geom->fs.vr[0];
	mat[ 1] = geom->fs.vr[1];
	mat[ 2] = geom->fs.vr[2];
	mat[ 3] = 0.0;
	mat[ 4] = geom->fs.vf[0];
	mat[ 5] = geom->fs.vf[1];
	mat[ 6] = geom->fs.vf[2];
	mat[ 7] = 0.0;
	mat[ 8] = geom->fs.vt[0];
	mat[ 9] = geom->fs.vt[1];
	mat[10] = geom->fs.vt[2];
	mat[11] = 0.0;
	mat[12] = geom->fs.vc[0];
	mat[13] = geom->fs.vc[1];
	mat[14] = geom->fs.vc[2];
	mat[15] = 1.0;


	//update vertex
	for(j=0;j<COUNT;j++)
	{
		x = buf[9*j + 0] + buf[9*j + 3];
		if(x <-1.0)x = 1.0;
		else if(x > 1.0)x = -1.0;
		buf[9*j + 0] = x;

		y = buf[9*j + 1] + buf[9*j + 4];
		if(y <-1.0)y = 1.0;
		else if(y > 1.0)y = -1.0;
		buf[9*j + 1] = y;

		z = buf[9*j + 2] + buf[9*j + 5];
		if(z <-1.0)z = 1.0;
		else if(z > 1.0)z = -1.0;
		buf[9*j + 2] = z;
	}
	data->src.vbuf_enq += 1;
	gl41data_insert(wnd, 's', act->CTXBUF, 1);
}
static void particle_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void particle_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void particle_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void particle_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("particle(%x,%x,%x)\n",win,act,sty);
}




static void particle_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void particle_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	particle_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void particle_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		particle_read_bywnd(ent,slot, caller,area);
		break;
	default:
		particle_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void particle_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void particle_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void particle_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void particle_search(_obj* act)
{
}
static void particle_modify(_obj* act)
{
}
static void particle_delete(_obj* act)
{
	if(0 == act)return;
}
static void particle_create(_obj* act)
{
	if(0 == act)return;

	int j;
	float* vbuf = act->F32BUF = memoryalloc(4*9 * COUNT, 0);
	if(0 == vbuf)return;
	for(j=0;j<COUNT;j++)
	{
		//vertex
		vbuf[9*j + 0] = (random_read()%8192)/4096.0 - 1.0;
		vbuf[9*j + 1] = (random_read()%8192)/4096.0 - 1.0;
		vbuf[9*j + 2] = (random_read()%8192)/4096.0 - 1.0;

		//speed
		vbuf[9*j + 3] = 0.0;
		vbuf[9*j + 4] = -0.01*(random_read()%8192)/8192.0;
		vbuf[9*j + 5] = 0.0;

		//colour
		vbuf[9*j + 6] = (random_read()%8192)/8192.0;
		vbuf[9*j + 7] = (random_read()%8192)/8192.0;
		vbuf[9*j + 8] = (random_read()%8192)/8192.0;
	}

	struct gl41data* data = act->CTXBUF = memoryalloc(0x1000, 0);
	particle_ctxforwnd(data, vbuf);
}




void particle_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('p', 'a', 'r', 't', 'i', 'c', 'l', 'e');

	p->oncreate = (void*)particle_create;
	p->ondelete = (void*)particle_delete;
	p->onreader = (void*)particle_search;
	p->onwriter = (void*)particle_modify;

	p->onattach = (void*)particle_attach;
	p->ondetach = (void*)particle_detach;
	p->ontaking = (void*)particle_taking;
	p->ongiving = (void*)particle_giving;
}
