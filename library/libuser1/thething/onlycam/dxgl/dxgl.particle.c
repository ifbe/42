#include "libuser.h"
#define F32BUF buf0
#define CTXBUF buf1
#define COUNT (0x100000/36)
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);



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

void particle_ctxforwnd(struct glsrc* src, float* vbuf)
{
	//shader
	src->vs = particle_glsl_v;
	src->gs = particle_glsl_g;
	src->fs = particle_glsl_f;
	src->shader_enq = 42;

	//argument
	src->arg[0].name = "objmat";
	src->arg[0].data = memorycreate(4*4*4, 0);
	src->arg[0].fmt = 'm';

	//vertex
	src->geometry = 1;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*9;
	src->vbuf_h = COUNT;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = vbuf;
}



static void particle_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void particle_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	float x,y,z;
	struct glsrc* src;
	float* mat;
	float* buf;

	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	//gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vu);


	src = act->CTXBUF;
	if(0 == src)return;

	mat = (void*)src->arg[0].data;
	if(0 == mat)return;

	buf = src->vbuf;
	if(0 == buf)return;


	//update matrix
	mat[ 0] = geom->f.vr[0];
	mat[ 1] = geom->f.vr[1];
	mat[ 2] = geom->f.vr[2];
	mat[ 3] = 0.0;
	mat[ 4] = geom->f.vf[0];
	mat[ 5] = geom->f.vf[1];
	mat[ 6] = geom->f.vf[2];
	mat[ 7] = 0.0;
	mat[ 8] = geom->f.vt[0];
	mat[ 9] = geom->f.vt[1];
	mat[10] = geom->f.vt[2];
	mat[11] = 0.0;
	mat[12] = geom->f.vc[0];
	mat[13] = geom->f.vc[1];
	mat[14] = geom->f.vc[2];
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
	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', act->CTXBUF, 1);
}
static void particle_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void particle_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void particle_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void particle_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("particle(%x,%x,%x)\n",win,act,sty);
}




static void particle_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		particle_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void particle_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void particle_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void particle_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void particle_search(struct entity* act)
{
}
static void particle_modify(struct entity* act)
{
}
static void particle_delete(struct entity* act)
{
	if(0 == act)return;
}
static void particle_create(struct entity* act)
{
	int j;
	float* vbuf;
	struct glsrc* src;
	if(0 == act)return;


	vbuf = act->F32BUF = memorycreate(4*9 * COUNT, 0);
	if(0 == vbuf)return;
	for(j=0;j<COUNT;j++)
	{
		//vertex
		vbuf[9*j + 0] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 1] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 2] = (getrandom()%8192)/4096.0 - 1.0;

		//speed
		vbuf[9*j + 3] = 0.0;
		vbuf[9*j + 4] = -0.01*(getrandom()%8192)/8192.0;
		vbuf[9*j + 5] = 0.0;

		//colour
		vbuf[9*j + 6] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 7] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 8] = (getrandom()%8192)/8192.0;
	}


	src = act->CTXBUF = memorycreate(0x200, 0);
	particle_ctxforwnd(src, vbuf);
}




void particle_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'a', 'r', 't', 'i', 'c', 'l', 'e');

	p->oncreate = (void*)particle_create;
	p->ondelete = (void*)particle_delete;
	p->onsearch = (void*)particle_search;
	p->onmodify = (void*)particle_modify;

	p->onlinkup = (void*)particle_linkup;
	p->ondiscon = (void*)particle_discon;
	p->ontaking = (void*)particle_taking;
	p->ongiving = (void*)particle_giving;
}
