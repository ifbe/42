#include "libuser.h"
#define GL41BUF listptr.buf0
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 c;
	int x,y;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	u32* buf = win->rgbanode.buf;
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
static void fractal_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	struct mysrc* src = act->GL41BUF;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
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
	gl41data_insert(wnd, 's', src, 1);
}
static void fractal_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fractal_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fractal_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fractal_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("fractal(%x,%x,%x)\n",win,act,sty);
}




static void fractal_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	//logtoall("%llx,%llx\n",ev->why, ev->what);
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




static void fractal_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void fractal_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	fractal_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void fractal_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		fractal_read_bywnd(ent,slot, caller,area);
		break;
	default:
		fractal_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void fractal_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void fractal_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void fractal_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void fractal_search(_obj* act)
{
}
static void fractal_modify(_obj* act)
{
}
static void fractal_delete(_obj* act)
{
	if(0 == act)return;
}
static void fractal_create(_obj* act)
{
	struct mysrc* src;
	if(0 == act)return;

	src = act->GL41BUF = memoryalloc(0x1000, 0);
	if(0 == src)return;

	//shader
	src->vs = fractal_glsl_v;
	src->fs = glsl_julia;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
}




void fractal_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onreader = (void*)fractal_search;
	p->onwriter = (void*)fractal_modify;

	p->onattach = (void*)fractal_attach;
	p->ondetach = (void*)fractal_detach;
	p->ontaking = (void*)fractal_taking;
	p->ongiving = (void*)fractal_giving;
}
