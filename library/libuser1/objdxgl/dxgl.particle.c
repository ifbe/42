#include "libuser.h"
#define COUNT (0x100000/36)



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
	"gl_Position = position + vec4(-5.0, 0.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 0.0, -5.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 0.0,  5.0, 0.0, 0.0);\n"
	"colour = rgb[0];\n"
	"EmitVertex();\n"
	"gl_Position = position + vec4( 5.0,  0.0, 0.0, 0.0);\n"
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




static void particle_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void particle_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void particle_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int j;
	float x,y,z;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	float* sbuf = act->buf;
	struct glsrc* src = (void*)(pin->foot[0]);
	float* mat = (void*)src->arg_data[0];

	mat[ 0] = sty->f.vr[0];
	mat[ 1] = sty->f.vr[1];
	mat[ 2] = sty->f.vr[2];
	mat[ 3] = 0.0;
	mat[ 4] = sty->f.vf[0];
	mat[ 5] = sty->f.vf[1];
	mat[ 6] = sty->f.vf[2];
	mat[ 7] = 0.0;
	mat[ 8] = sty->f.vt[0];
	mat[ 9] = sty->f.vt[1];
	mat[10] = sty->f.vt[2];
	mat[11] = 0.0;
	mat[12] = sty->f.vc[0];
	mat[13] = sty->f.vc[1];
	mat[14] = sty->f.vc[2];
	mat[15] = 1.0;

//for(j=0;j<16;j++)mat[j] = 0.0;
//mat[0] = mat[5] = mat[10] = mat[15] = 1000.0;
//mat[15] = 1.0;

	carveline_prism4(win, 0xffffff, vc, vr, vf, vu);
	for(j=0;j<COUNT;j++)
	{
		x = sbuf[9*j + 0] + sbuf[9*j + 3];
		if((x < -1.0) | (x > 1.0))sbuf[9*j + 3] = - sbuf[9*j + 3];
		sbuf[9*j + 0] += sbuf[9*j + 3];

		y = sbuf[9*j + 1] + sbuf[9*j + 4];
		if((y < -1.0) | (y > 1.0))sbuf[9*j + 4] = - sbuf[9*j + 4];
		sbuf[9*j + 1] += sbuf[9*j + 4];

		z = sbuf[9*j + 2] + sbuf[9*j + 5];
		if((z < -1.0) | (z > 1.0))sbuf[9*j + 5] = - sbuf[9*j + 5];
		sbuf[9*j + 2] += sbuf[9*j + 5];
	}
	src->vbuf_enq += 1;
}
static void particle_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void particle_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void particle_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void particle_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	say("particle(%x,%x,%x)\n",win,act,sty);
}
static void particle_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)particle_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)particle_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)particle_draw_html(act, pin, win, sty);
	else if(fmt == _json_)particle_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)particle_draw_vbo2d(act, pin, win, sty);
		else particle_draw_vbo3d(act, pin, win, sty);
	}
	else particle_draw_pixel(act, pin, win, sty);
}




static void particle_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	particle_draw(act, pin, win, sty);
}
static void particle_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void particle_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void particle_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void particle_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void particle_start(struct halfrel* self, struct halfrel* peer)
{
	void* vbuf;
	void* ibuf;
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);

	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 1;
	src->method = 'v';

	//shader
	src->vs = particle_glsl_v;
	src->gs = particle_glsl_g;
	src->fs = particle_glsl_f;
	src->shader_enq = 42;

	//argument
	src->arg_name[0] = "objmat";
	src->arg_data[0] = memorycreate(4*4*4);
	src->arg_fmt[0] = 'm';

	//vertex
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf = act->buf;
	src->vbuf_w = 4*9;
	src->vbuf_h = COUNT;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf_enq = 42;
}
static void particle_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void particle_create(struct actor* act)
{
	int j;
	if(0 == act)return;

	act->buf = memorycreate(4*9 * COUNT);
	float* vbuf = act->buf;

	for(j=0;j<COUNT;j++)
	{
		//vertex
		vbuf[9*j + 0] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 1] = (getrandom()%8192)/4096.0 - 1.0;
		vbuf[9*j + 2] = (getrandom()%8192)/4096.0 - 1.0;

		//speed
		vbuf[9*j + 3] = (getrandom()%8191 - 4095)/409600.0;
		vbuf[9*j + 4] = (getrandom()%8191 - 4095)/409600.0;
		vbuf[9*j + 5] = (getrandom()%8191 - 4095)/409600.0;

		//colour
		vbuf[9*j + 6] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 7] = (getrandom()%8192)/8192.0;
		vbuf[9*j + 8] = (getrandom()%8192)/8192.0;
	}
}




void particle_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'a', 'r', 't', 'i', 'c', 'l', 'e');

	p->oncreate = (void*)particle_create;
	p->ondelete = (void*)particle_delete;
	p->onstart  = (void*)particle_start;
	p->onstop   = (void*)particle_stop;
	p->oncread  = (void*)particle_cread;
	p->oncwrite = (void*)particle_cwrite;
	p->onsread  = (void*)particle_sread;
	p->onswrite = (void*)particle_swrite;
}
