#include "libuser.h"
void loadtexfromfile(struct glsrc* src, int idx, u8* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* skillbar_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* skillbar_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"mediump vec3 bgr = texture(tex0, uvw).bgr;\n"
	"FragColor = vec4(bgr, 1.0);\n"
"}\n";

static void vertex_gen(float (*vbuf)[6], float x, float y, vec3 vc, vec3 vr, vec3 vf)
{
	int t = 0;
	vbuf[t+0][0] = vc[0] - vr[0] - vf[0];
	vbuf[t+0][1] = vc[1] - vr[1] - vf[1];
	vbuf[t+0][2] = vc[2] - vr[2] - vf[2];
	vbuf[t+0][3] = (x+0)/16.0;
	vbuf[t+0][4] = (y+1)/16.0;
	vbuf[t+0][5] = 0.0;

	vbuf[t+1][0] = vc[0] + vr[0] + vf[0];
	vbuf[t+1][1] = vc[1] + vr[1] + vf[1];
	vbuf[t+1][2] = vc[2] + vr[2] + vf[2];
	vbuf[t+1][3] = (x+1)/16.0;
	vbuf[t+1][4] = (y+0)/16.0;
	vbuf[t+1][5] = 0.0;

	vbuf[t+2][0] = vc[0] - vr[0] + vf[0];
	vbuf[t+2][1] = vc[1] - vr[1] + vf[1];
	vbuf[t+2][2] = vc[2] - vr[2] + vf[2];
	vbuf[t+2][3] = (x+0)/16.0;
	vbuf[t+2][4] = (y+0)/16.0;
	vbuf[t+2][5] = 0.0;

	vbuf[t+3][0] = vc[0] + vr[0] + vf[0];
	vbuf[t+3][1] = vc[1] + vr[1] + vf[1];
	vbuf[t+3][2] = vc[2] + vr[2] + vf[2];
	vbuf[t+3][3] = (x+1)/16.0;
	vbuf[t+3][4] = (y+0)/16.0;
	vbuf[t+3][5] = 0.0;

	vbuf[t+4][0] = vc[0] - vr[0] - vf[0];
	vbuf[t+4][1] = vc[1] - vr[1] - vf[1];
	vbuf[t+4][2] = vc[2] - vr[2] - vf[2];
	vbuf[t+4][3] = (x+0)/16.0;
	vbuf[t+4][4] = (y+1)/16.0;
	vbuf[t+4][5] = 0.0;

	vbuf[t+5][0] = vc[0] + vr[0] - vf[0];
	vbuf[t+5][1] = vc[1] + vr[1] - vf[1];
	vbuf[t+5][2] = vc[2] + vr[2] - vf[2];
	vbuf[t+5][3] = (x+1)/16.0;
	vbuf[t+5][4] = (y+1)/16.0;
	vbuf[t+5][5] = 0.0;
}
void skillbar_ctxforwnd(struct glsrc* src, u8* str)
{
	//property
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = skillbar_glsl_v;
	src->fs = skillbar_glsl_f;
	src->shader_enq = 42;

	//texture0
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
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




static void skillbar_draw_pixel(
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
	if(0 == act->buf)return;

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->width);
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
static void skillbar_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,k,t;
	vec3 tc, tr, tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_rect(wnd, 0xff00ff, vc, vr, vf);

	struct glsrc* src = act->buf;
	float (*vbuf)[6] = (void*)(src->vbuf);

	tr[0] = vr[0]/30;
	tr[1] = vr[1]/30;
	tr[2] = vr[2]/30;
	tf[0] = vf[0]/30;
	tf[1] = vf[1]/30;
	tf[2] = vf[2]/30;
	k = t = 0;
	for(y=0;y<2;y++){
		for(x=-12;x<12;x++){
			if(0 == x)continue;

			tc[0] = vc[0] + vr[0]*(2*x+1)/28.0 + vf[0]*(2*y-23)/24.0;
			tc[1] = vc[1] + vr[1]*(2*x+1)/28.0 + vf[1]*(2*y-23)/24.0;
			tc[2] = vc[2] + vr[2]*(2*x+1)/28.0 + vf[2]*(2*y-23)/24.0;
			gl41line_rectselect(wnd, 0xff00ff, tc, tr, tf);

			vertex_gen(&vbuf[t+0], k%16, k/16, tc, tr, tf);
			k ++;
			t += 6;
		}
	}
	for(x=0;x<2;x++){
	for(y=0;y<12;y++){
			tc[0] = vc[0] + vr[0]*(2*x+21)/24.0 + vf[0]*(2*y-11)/24.0;
			tc[1] = vc[1] + vr[1]*(2*x+21)/24.0 + vf[1]*(2*y-11)/24.0;
			tc[2] = vc[2] + vr[2]*(2*x+21)/24.0 + vf[2]*(2*y-11)/24.0;
			gl41line_rectselect(wnd, 0xff00ff, tc, tr, tf);

			vertex_gen(&vbuf[t+0], k%16, k/16, tc, tr, tf);
			k ++;
			t += 6;
		}
	}

	src->vbuf_h = 6*12*6;
	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}
static void skillbar_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skillbar_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skillbar_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void skillbar_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("skillbar(%x,%x,%x)\n",win,act,sty);
}
static void skillbar_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)skillbar_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)skillbar_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)skillbar_draw_html(act, pin, win, sty);
	else if(fmt == _json_)skillbar_draw_json(act, pin, win, sty);
}




static void skillbar_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> video
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)skillbar_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void skillbar_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void skillbar_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void skillbar_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void skillbar_search(struct entity* act)
{
}
static void skillbar_modify(struct entity* act)
{
}
static void skillbar_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void skillbar_create(struct entity* act, void* str)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	skillbar_ctxforwnd(src, str);
}




void skillbar_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'k', 'i', 'l', 'l', 0, 0, 0);

	p->oncreate = (void*)skillbar_create;
	p->ondelete = (void*)skillbar_delete;
	p->onsearch = (void*)skillbar_search;
	p->onmodify = (void*)skillbar_modify;

	p->onlinkup = (void*)skillbar_linkup;
	p->ondiscon = (void*)skillbar_discon;
	p->onread  = (void*)skillbar_read;
	p->onwrite = (void*)skillbar_write;
}
