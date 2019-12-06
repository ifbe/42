#include "libuser.h"
#define GL41BUF buf0




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
static void fractal_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 c;
	int x,y;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
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
/*
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
	}*/
}
static void fractal_draw_vbo3d(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct glsrc* src;
	float (*vbuf)[6];
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	src = act->GL41BUF;
	if(0 == src)return;
	vbuf = src->vbuf;
	if(0 == vbuf)return;

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
static void fractal_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fractal_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fractal_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fractal_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("fractal(%x,%x,%x)\n",win,act,sty);
}
static void fractal_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)fractal_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)fractal_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)fractal_draw_html(act, pin, win, sty);
	else if(fmt == _json_)fractal_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)fractal_draw_vbo2d(act, pin, win, sty);
		//else fractal_draw_vbo3d(act, pin, win, sty);
	}
	else fractal_draw_pixel(act, pin, win, sty);
}
static void fractal_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	//say("%llx,%llx\n",ev->why, ev->what);
	u64 why = ev->why;
	u64 what = ev->what;
/*
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
	}*/
}




static void fractal_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
	//scene -> codeimg
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	scn = peer->pchip;geom = peer->pfoot;
	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		if('v' == len)fractal_draw_vbo3d(act,slot, scn,geom, wnd,area);
	}
}
static void fractal_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//fractal_event(act, pin, win, sty, ev, 0);
}
static void fractal_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void fractal_start(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->GL41BUF);
}




static void fractal_search(struct entity* act)
{
}
static void fractal_modify(struct entity* act)
{
}
static void fractal_delete(struct entity* act)
{
	if(0 == act)return;
}
static void fractal_create(struct entity* act)
{
	struct glsrc* src;
	if(0 == act)return;

	src = act->GL41BUF = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = fractal_glsl_v;
	src->fs = glsl_julia;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




void fractal_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onsearch = (void*)fractal_search;
	p->onmodify = (void*)fractal_modify;

	p->onstart = (void*)fractal_start;
	p->onstop  = (void*)fractal_stop;
	p->onread  = (void*)fractal_read;
	p->onwrite = (void*)fractal_write;
}
