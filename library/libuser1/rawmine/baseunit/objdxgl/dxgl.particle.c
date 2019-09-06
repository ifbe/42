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




static void particle_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}/*
static void particle_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}*/
static void particle_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int j;
	float x,y,z;
	struct glsrc* src;
	float* mat;
	float* buf;

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	//carveline_prism4(win, 0xffffff, vc, vr, vf, vu);


	src = act->buf;
	if(0 == src)return;

	mat = (void*)src->arg_data[0];
	if(0 == mat)return;

	buf = src->vbuf;
	if(0 == buf)return;


	//update matrix
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
}
static void particle_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void particle_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void particle_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void particle_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("particle(%x,%x,%x)\n",win,act,sty);
}
static void particle_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)particle_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)particle_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)particle_draw_html(act, pin, win, sty);
	else if(fmt == _json_)particle_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)particle_draw_vbo2d(act, pin, win, sty);
		//else particle_draw_vbo3d(act, pin, win, sty);
	}
	else particle_draw_pixel(act, pin, win, sty);
}




static void particle_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@texball_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)particle_draw_vbo3d(act,pin,ctx,sty);
	}
	//particle_draw(act, pin, win, sty);
}
static void particle_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
}
static void particle_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void particle_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->buf);
	say("@particle_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




static void particle_search(struct actor* act)
{
}
static void particle_modify(struct actor* act)
{
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
	float* vbuf;
	struct glsrc* src;
	if(0 == act)return;


	vbuf = memorycreate(4*9 * COUNT, 0);
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


	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

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
	src->arg_data[0] = memorycreate(4*4*4, 0);
	src->arg_fmt[0] = 'm';

	//vertex
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*9;
	src->vbuf_h = COUNT;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = vbuf;
}




void particle_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'a', 'r', 't', 'i', 'c', 'l', 'e');

	p->oncreate = (void*)particle_create;
	p->ondelete = (void*)particle_delete;
	p->onsearch = (void*)particle_search;
	p->onmodify = (void*)particle_modify;

	p->onstart = (void*)particle_start;
	p->onstop  = (void*)particle_stop;
	p->onread  = (void*)particle_read;
	p->onwrite = (void*)particle_write;
}
