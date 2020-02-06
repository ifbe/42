#include "libuser.h"
void loadtexfromfile(struct glsrc* src, int idx, char* name);




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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
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
	if(0 == act->buf0)return;

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->rgbabuf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf0) + 4*y*(act->width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->fmt)&0xff))
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,t;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	gl41line_rect(win, 0xffffff, vc, vr, vf);

	struct glsrc* src = act->buf0;
	if(0 == src)return;
	float (*vbuf)[6] = (void*)(src->vbuf);
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

	src->vbuf_h = 6*(16+16+16+16+1);
	src->vbuf_enq += 1;
}
static void mario_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mario_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mario_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mario_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("mario(%x,%x,%x)\n",win,act,sty);
}




static void mario_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void mario_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void mario_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mario_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mario_search(struct entity* act)
{
}
static void mario_modify(struct entity* act)
{
}
static void mario_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void mario_create(struct entity* act, void* str)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf0 = memorycreate(0x200, 0);
	if(0 == src)return;

	//property
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = mario_glsl_v;
	src->fs = mario_glsl_f;
	src->shader_enq = 42;

	//texture0
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	loadtexfromfile(src, 0, str);
	src->tex[0].enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 0;
	src->vbuf_len = (src->vbuf_w) * 6*16*16;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;
}




void mario_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'a', 'r', 'i', 'o', 0, 0, 0);

	p->oncreate = (void*)mario_create;
	p->ondelete = (void*)mario_delete;
	p->onsearch = (void*)mario_search;
	p->onmodify = (void*)mario_modify;

	p->onlinkup = (void*)mario_linkup;
	p->ondiscon = (void*)mario_discon;
	p->onread  = (void*)mario_read;
	p->onwrite = (void*)mario_write;
}
