#include "libuser.h"




char* fractal_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
char* fractal_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* glsl_manderbrot =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"int j;\n"
		"mediump float tx,ty;\n"
		"mediump float fx = 0.0;\n"
		"mediump float fy = 0.0;\n"
		"for(j=0;j<256;j++)\n"
		"{\n"
			"tx = fx*fx - fy*fy + uvw.x;\n"
			"ty = 2.0*fx*fy + uvw.y;\n"
			"if(tx*tx + ty*ty > 2.0)break;\n"
			"fx = tx;\n"
			"fy = ty;\n"
		"}\n"
		"mediump float k = float(j) / 256.0;\n"
		"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
		//"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
		//"FragColor = vec4(uvw, 1.0, 1.0);\n"
	"}\n";
char* glsl_julia =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"int j;\n"
		"mediump float tx,ty;\n"
		"mediump float fx = uvw.x;\n"
		"mediump float fy = uvw.y;\n"
		"for(j=0;j<256;j++)\n"
		"{\n"
			"tx = fx*fx - fy*fy - 0.8;\n"
			"ty = 2.0*fx*fy + 0.156;\n"
			"if(tx*tx + ty*ty > 2.0)break;\n"
			"fx = tx;\n"
			"fy = ty;\n"
		"}\n"
		"mediump float k = float(j) / 256.0;\n"
		"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
	"}\n";




int manderbrot_check(double x, double y)
{
	int j;
	double tx,ty;
	double fx = 0.0;
	double fy = 0.0;
	for(j=0;j<255;j++)
	{
		tx = fx*fx - fy*fy + x;
		ty = 2*fx*fy + y;
		if(tx*tx + ty*ty > 4)break;
		fx = tx;
		fy = ty;
	}
	return j;
}
static void fractal_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	int w = win->stride;
	u32* buf = win->buf;
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=1-hh;y<hh;y++)
	{
		for(x=1-ww;x<ww;x++)
		{
			c = manderbrot_check(
				(act->target.vc[0]) + 2.0*x*(act->target.vr[0])/ww,
				(act->target.vc[0]) + 2.0*y*(act->target.vf[1])/hh
			);
			buf[w*(cy+y) + cx+x] = 0xff000000 | (c*0x010101);
		}
	}
}
static void fractal_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	float* tc = act->target.vc;
	float* tr = act->target.vr;
	float* tf = act->target.vf;
	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = src->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = tc[0] - tr[0];
	vbuf[0][4] = tc[1] - tf[1];
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = tc[0] + tr[0];
	vbuf[1][4] = tc[1] + tf[1];
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = tc[0] - tr[0];
	vbuf[2][4] = tc[1] + tf[1];
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = tc[0] + tr[0];
	vbuf[3][4] = tc[1] + tf[1];
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = tc[0] - tr[0];
	vbuf[4][4] = tc[1] - tf[1];
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = tc[0] + tr[0];
	vbuf[5][4] = tc[1] - tf[1];
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void fractal_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	struct glsrc* src = (void*)(pin->foot[0]);
	float (*vbuf)[6] = src->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = -1.0;
	vbuf[0][4] = -1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = -1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = -1.0;
	vbuf[4][4] = -1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = -1.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void fractal_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void fractal_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("fractal(%x,%x,%x)\n",win,act,sty);
}
static void fractal_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)fractal_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)fractal_read_tui(win, sty, act, pin);
	else if(fmt == _html_)fractal_read_html(win, sty, act, pin);
	else if(fmt == _json_)fractal_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)fractal_read_vbo2d(win, sty, act, pin);
		else fractal_read_vbo3d(win, sty, act, pin);
	}
	else fractal_read_pixel(win, sty, act, pin);
}
static void fractal_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//say("%llx,%llx\n",ev->why, ev->what);
	u64 why = ev->why;
	u64 what = ev->what;
	if(0x2b70 == what)
	{
		why >>= 48;
		if('f' == why)
		{
			act->target.vr[0] *= 0.9;
			act->target.vf[1] *= 0.9;
		}
		else if('b' == why)
		{
			act->target.vr[0] *= 1.1;
			act->target.vf[1] *= 1.1;
		}
	}
	if(_kbd_ == what)
	{
		if(why == 0x48)act->target.vc[1] += act->target.vf[1]*0.1;
		else if(why == 0x4b)act->target.vc[0] -= act->target.vr[0]*0.1;
		else if(why == 0x4d)act->target.vc[0] += act->target.vr[0]*0.1;
		else if(why == 0x50)act->target.vc[1] -= act->target.vf[1]*0.1;
	}
}
static void fractal_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void fractal_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void fractal_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void fractal_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	//alloc
	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//shader
	src->vs = fractal_glsl_v;
	src->fs = glsl_julia;
	if(twig){if(_fg2d_ == twig->fmt)src->vs = fractal_glsl2d_v;}

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->method = 'v';

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void fractal_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void fractal_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(16);

	act->target.vc[0] = 0;
	act->target.vc[1] = 0;
	act->target.vr[0] = 1.0;
	act->target.vf[1] = 1.0;
}




void fractal_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onstart  = (void*)fractal_start;
	p->onstop   = (void*)fractal_stop;
	p->oncread  = (void*)fractal_cread;
	p->oncwrite = (void*)fractal_cwrite;
	p->onsread  = (void*)fractal_sread;
	p->onswrite = (void*)fractal_swrite;
}
