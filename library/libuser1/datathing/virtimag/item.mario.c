#include "libuser.h"




char* mario_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* mario_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"mediump vec3 bgr = texture(tex0, uvw).bgr;\n"
	"FragColor = vec4(bgr, 1.0);\n"
"}\n";





static void mario_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
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
	if(0 == act->listptr.buf0)return;

	xmax = act->whdf.width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->whdf.height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->whdf.fbwidth;
	for(y=0;y<ymax;y++)
	{
		dst = (win->rgbanode.buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->listptr.buf0) + 4*y*(act->whdf.width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->hfmt)&0xff))
		{
			for(x=0;x<xmax;x++)dst[x] = src[x];
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}
}
static void mario_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,t;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc, vr, vf);

	struct mysrc* src = act->listptr.buf0;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	for(x=0;x<16;x++){
		t = (0+x)*6;

		vbuf[t+0][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]/8;
		vbuf[t+0][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]/8;
		vbuf[t+0][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]/8;
		vbuf[t+0][3] = 3.0/16;
		vbuf[t+0][4] = 1.0/16;
		vbuf[t+0][5] = 0.0;

		vbuf[t+1][0] = vc[0] + vr[0]*(x-7)/8;
		vbuf[t+1][1] = vc[1] + vr[1]*(x-7)/8;
		vbuf[t+1][2] = vc[2] + vr[2]*(x-7)/8;
		vbuf[t+1][3] = 4.0/16;
		vbuf[t+1][4] = 0.0;
		vbuf[t+1][5] = 0.0;

		vbuf[t+2][0] = vc[0] + vr[0]*(x-8)/8;
		vbuf[t+2][1] = vc[1] + vr[1]*(x-8)/8;
		vbuf[t+2][2] = vc[2] + vr[2]*(x-8)/8;
		vbuf[t+2][3] = 3.0/16;
		vbuf[t+2][4] = 0.0;
		vbuf[t+2][5] = 0.0;

		vbuf[t+3][0] = vc[0] + vr[0]*(x-7)/8;
		vbuf[t+3][1] = vc[1] + vr[1]*(x-7)/8;
		vbuf[t+3][2] = vc[2] + vr[2]*(x-7)/8;
		vbuf[t+3][3] = 4.0/16;
		vbuf[t+3][4] = 0.0;
		vbuf[t+3][5] = 0.0;

		vbuf[t+4][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]/8;
		vbuf[t+4][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]/8;
		vbuf[t+4][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]/8;
		vbuf[t+4][3] = 3.0/16;
		vbuf[t+4][4] = 1.0/16;
		vbuf[t+4][5] = 0.0;

		vbuf[t+5][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]/8;
		vbuf[t+5][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]/8;
		vbuf[t+5][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]/8;
		vbuf[t+5][3] = 4.0/16;
		vbuf[t+5][4] = 1.0/16;
		vbuf[t+5][5] = 0.0;
	}
	for(x=0;x<16;x++){
		t = (16+x)*6;

		vbuf[t+0][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*2/8;
		vbuf[t+0][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*2/8;
		vbuf[t+0][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*2/8;
		vbuf[t+0][3] = 2.0/16;
		vbuf[t+0][4] = 1.0/16;
		vbuf[t+0][5] = 0.0;

		vbuf[t+1][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]/8;
		vbuf[t+1][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]/8;
		vbuf[t+1][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]/8;
		vbuf[t+1][3] = 3.0/16;
		vbuf[t+1][4] = 0.0;
		vbuf[t+1][5] = 0.0;

		vbuf[t+2][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]/8;
		vbuf[t+2][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]/8;
		vbuf[t+2][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]/8;
		vbuf[t+2][3] = 2.0/16;
		vbuf[t+2][4] = 0.0;
		vbuf[t+2][5] = 0.0;

		vbuf[t+3][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]/8;
		vbuf[t+3][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]/8;
		vbuf[t+3][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]/8;
		vbuf[t+3][3] = 3.0/16;
		vbuf[t+3][4] = 0.0;
		vbuf[t+3][5] = 0.0;

		vbuf[t+4][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*2/8;
		vbuf[t+4][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*2/8;
		vbuf[t+4][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*2/8;
		vbuf[t+4][3] = 2.0/16;
		vbuf[t+4][4] = 1.0/16;
		vbuf[t+4][5] = 0.0;

		vbuf[t+5][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*2/8;
		vbuf[t+5][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*2/8;
		vbuf[t+5][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*2/8;
		vbuf[t+5][3] = 3.0/16;
		vbuf[t+5][4] = 1.0/16;
		vbuf[t+5][5] = 0.0;
	}
	for(x=0;x<16;x++){
		t = (32+x)*6;

		vbuf[t+0][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*3/8;
		vbuf[t+0][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*3/8;
		vbuf[t+0][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*3/8;
		vbuf[t+0][3] = 1.0/16;
		vbuf[t+0][4] = 1.0/16;
		vbuf[t+0][5] = 0.0;

		vbuf[t+1][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*2/8;
		vbuf[t+1][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*2/8;
		vbuf[t+1][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*2/8;
		vbuf[t+1][3] = 2.0/16;
		vbuf[t+1][4] = 2.0/16;
		vbuf[t+1][5] = 0.0;

		vbuf[t+2][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*2/8;
		vbuf[t+2][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*2/8;
		vbuf[t+2][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*2/8;
		vbuf[t+2][3] = 1.0/16;
		vbuf[t+2][4] = 2.0/16;
		vbuf[t+2][5] = 0.0;

		vbuf[t+3][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*2/8;
		vbuf[t+3][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*2/8;
		vbuf[t+3][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*2/8;
		vbuf[t+3][3] = 2.0/16;
		vbuf[t+3][4] = 2.0/16;
		vbuf[t+3][5] = 0.0;

		vbuf[t+4][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*3/8;
		vbuf[t+4][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*3/8;
		vbuf[t+4][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*3/8;
		vbuf[t+4][3] = 1.0/16;
		vbuf[t+4][4] = 1.0/16;
		vbuf[t+4][5] = 0.0;

		vbuf[t+5][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*3/8;
		vbuf[t+5][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*3/8;
		vbuf[t+5][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*3/8;
		vbuf[t+5][3] = 2.0/16;
		vbuf[t+5][4] = 1.0/16;
		vbuf[t+5][5] = 0.0;
	}
	for(x=0;x<16;x++){
		t = (48+x)*6;

		vbuf[t+0][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*4/8;
		vbuf[t+0][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*4/8;
		vbuf[t+0][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*4/8;
		vbuf[t+0][3] = 15.0/16;
		vbuf[t+0][4] = 15.0/16;
		vbuf[t+0][5] = 0.0;

		vbuf[t+1][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*3/8;
		vbuf[t+1][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*3/8;
		vbuf[t+1][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*3/8;
		vbuf[t+1][3] = 16.0/16;
		vbuf[t+1][4] = 1.0;
		vbuf[t+1][5] = 0.0;

		vbuf[t+2][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*3/8;
		vbuf[t+2][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*3/8;
		vbuf[t+2][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*3/8;
		vbuf[t+2][3] = 15.0/16;
		vbuf[t+2][4] = 1.0;
		vbuf[t+2][5] = 0.0;

		vbuf[t+3][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*3/8;
		vbuf[t+3][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*3/8;
		vbuf[t+3][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*3/8;
		vbuf[t+3][3] = 16.0/16;
		vbuf[t+3][4] = 1.0;
		vbuf[t+3][5] = 0.0;

		vbuf[t+4][0] = vc[0] + vr[0]*(x-8)/8 - vf[0]*4/8;
		vbuf[t+4][1] = vc[1] + vr[1]*(x-8)/8 - vf[1]*4/8;
		vbuf[t+4][2] = vc[2] + vr[2]*(x-8)/8 - vf[2]*4/8;
		vbuf[t+4][3] = 15.0/16;
		vbuf[t+4][4] = 15.0/16;
		vbuf[t+4][5] = 0.0;

		vbuf[t+5][0] = vc[0] + vr[0]*(x-7)/8 - vf[0]*4/8;
		vbuf[t+5][1] = vc[1] + vr[1]*(x-7)/8 - vf[1]*4/8;
		vbuf[t+5][2] = vc[2] + vr[2]*(x-7)/8 - vf[2]*4/8;
		vbuf[t+5][3] = 16.0/16;
		vbuf[t+5][4] = 15.0/16;
		vbuf[t+5][5] = 0.0;
	}

	x=5;
	t = 64*6;

	vbuf[t+0][0] = vc[0] + vr[0]*(x-8)/8 + vf[0]*0/8;
	vbuf[t+0][1] = vc[1] + vr[1]*(x-8)/8 + vf[1]*0/8;
	vbuf[t+0][2] = vc[2] + vr[2]*(x-8)/8 + vf[2]*0/8;
	vbuf[t+0][3] = 15.0/16;
	vbuf[t+0][4] = 1.0/16;
	vbuf[t+0][5] = 0.0;

	vbuf[t+1][0] = vc[0] + vr[0]*(x-7)/8 + vf[0]*1/8;
	vbuf[t+1][1] = vc[1] + vr[1]*(x-7)/8 + vf[1]*1/8;
	vbuf[t+1][2] = vc[2] + vr[2]*(x-7)/8 + vf[2]*1/8;
	vbuf[t+1][3] = 16.0/16;
	vbuf[t+1][4] = 0.0/16;
	vbuf[t+1][5] = 0.0;

	vbuf[t+2][0] = vc[0] + vr[0]*(x-8)/8 + vf[0]*1/8;
	vbuf[t+2][1] = vc[1] + vr[1]*(x-8)/8 + vf[1]*1/8;
	vbuf[t+2][2] = vc[2] + vr[2]*(x-8)/8 + vf[2]*1/8;
	vbuf[t+2][3] = 15.0/16;
	vbuf[t+2][4] = 0.0/16;
	vbuf[t+2][5] = 0.0;

	vbuf[t+3][0] = vc[0] + vr[0]*(x-7)/8 + vf[0]*1/8;
	vbuf[t+3][1] = vc[1] + vr[1]*(x-7)/8 + vf[1]*1/8;
	vbuf[t+3][2] = vc[2] + vr[2]*(x-7)/8 + vf[2]*1/8;
	vbuf[t+3][3] = 16.0/16;
	vbuf[t+3][4] = 0.0/16;
	vbuf[t+3][5] = 0.0;

	vbuf[t+4][0] = vc[0] + vr[0]*(x-8)/8 + vf[0]*0/8;
	vbuf[t+4][1] = vc[1] + vr[1]*(x-8)/8 + vf[1]*0/8;
	vbuf[t+4][2] = vc[2] + vr[2]*(x-8)/8 + vf[2]*0/8;
	vbuf[t+4][3] = 15.0/16;
	vbuf[t+4][4] = 1.0/16;
	vbuf[t+4][5] = 0.0;

	vbuf[t+5][0] = vc[0] + vr[0]*(x-7)/8 + vf[0]*0/8;
	vbuf[t+5][1] = vc[1] + vr[1]*(x-7)/8 + vf[1]*0/8;
	vbuf[t+5][2] = vc[2] + vr[2]*(x-7)/8 + vf[2]*0/8;
	vbuf[t+5][3] = 16.0/16;
	vbuf[t+5][4] = 1.0/16;
	vbuf[t+5][5] = 0.0;

	src->vtx[0].vbuf_h = 6*(16+16+16+16+1);
	src->vbuf_enq += 1;
}
static void mario_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mario_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mario_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mario_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("mario(%x,%x,%x)\n",win,act,sty);
}




static void mario_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mario_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mario_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		break;
	default:
		mario_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void mario_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mario_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mario_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mario_search(_obj* act)
{
}
static void mario_modify(_obj* act)
{
}
static void mario_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void mario_create(_obj* act, void* str)
{
	if(0 == act)return;
	struct gl41data* data = act->listptr.buf0 = memorycreate(0x1000, 0);
	if(0 == data)return;

	//shader
	data->src.vs = mario_glsl_v;
	data->src.fs = mario_glsl_f;
	data->src.shader_enq = 42;

	//texture
	struct texture* tex = &data->src.tex[0];
	tex->fmt = hex32('r','g','b','a');
	tex->data = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	loadtexfromfile(tex, str);
	data->src.tex_enq[0] = 42;

	//vertex
	struct vertex* vtx = &data->src.vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 0;
	vtx->vbuf_len = (vtx->vbuf_w) * 6*16*16;
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	//
	data->dst.texname[0] = "tex0";
	data->src.vbuf_enq = 42;
}




void mario_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'a', 'r', 'i', 'o', 0, 0, 0);

	p->oncreate = (void*)mario_create;
	p->ondelete = (void*)mario_delete;
	p->onsearch = (void*)mario_search;
	p->onmodify = (void*)mario_modify;

	p->onlinkup = (void*)mario_linkup;
	p->ondiscon = (void*)mario_discon;
	p->ontaking = (void*)mario_taking;
	p->ongiving = (void*)mario_giving;
}
