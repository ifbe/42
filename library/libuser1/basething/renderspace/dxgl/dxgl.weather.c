#include "libuser.h"




char* weather_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* weather_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump float k = uvw.x*uvw.x+uvw.y*uvw.y;\n"
	"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
	//"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(uvw, 1.0, 1.0);\n"
"}\n";
static void weather_ctxforwnd(struct mysrc* src)
{
	//shader
	src->vs = weather_glsl_v;
	src->fs = weather_glsl_f;
	src->shader_enq = 42;

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




static void weather_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,z;
	vec3 tc;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	for(z=-16;z<=16;z++)
	{
		for(y=-16;y<=16;y++)
		{
			for(x=-16;x<=16;x++)
			{
				tc[0] = vc[0] + vr[0]*x/16 + vf[0]*y/16 + vu[0]*z/16;
				tc[1] = vc[1] + vr[1]*x/16 + vf[1]*y/16 + vu[1]*z/16;
				tc[2] = vc[2] + vr[2]*x/16 + vf[2]*y/16 + vu[2]*z/16;
				gl41point(wnd, 0xffffff, tc);
			}
		}
	}
}
static void weather_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void weather_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void weather_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void weather_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void weather_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("weather(%x,%x,%x)\n",win,act,sty);
}




static void weather_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	weather_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void weather_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void weather_wnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
}




static void weather_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		weather_wnd(ent,slot, caller,area);
		break;
	default:
		weather_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void weather_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void weather_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void weather_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void weather_search(_obj* act)
{
}
static void weather_modify(_obj* act)
{
}
static void weather_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->listptr.buf0);
}
static void weather_create(_obj* act)
{
	if(0 == act)return;
	act->listptr.buf0 = memorycreate(0x1000, 0);
	weather_ctxforwnd(act->listptr.buf0);
}




void weather_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('w', 'e', 'a', 't', 'h', 'e', 'r', 0);

	p->oncreate = (void*)weather_create;
	p->ondelete = (void*)weather_delete;
	p->onreader = (void*)weather_search;
	p->onwriter = (void*)weather_modify;

	p->onattach = (void*)weather_attach;
	p->ondetach = (void*)weather_detach;
	p->ontaking = (void*)weather_taking;
	p->ongiving = (void*)weather_giving;
}
