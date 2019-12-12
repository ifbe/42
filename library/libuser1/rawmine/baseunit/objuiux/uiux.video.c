#include "libuser.h"
#define CTXBUF buf0
#define DATBUF buf1
#define _cam_ hex32('c','a','m',0)
#define _yuv_ hex32('y','u','v',0)
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);




char* video_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uv;\n"
	"void main()\n"
	"{\n"
		"uv = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* video_glsl_t = 0;
char* video_glsl_g = 0;
char* video_glsl_f = 
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uv;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 yuv = texture(tex0, uv).rgb;\n"
		"mediump float y = yuv.r;\n"
		"mediump float u = yuv.g - 0.5;\n"
		"mediump float v = yuv.b - 0.5;\n"
		"mediump float r = y + 1.402*v;\n"
		"mediump float g = y - 0.34414*u - 0.71414*v;\n"
		"mediump float b = y + 1.1772*u;\n"
		"FragColor = vec4(r, g, b, 1.0);\n"
	"}\n";
//directx shader
char* video_hlsl_v = 0;
char* video_hlsl_t = 0;
char* video_hlsl_g = 0;
char* video_hlsl_f = 0;
static void video_ctxforwnd(struct glsrc* src)
{
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = video_glsl_v;
	src->fs = video_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(1024*1024*4, 0);
	src->tex[0].fmt = hex32('r','g','b','a');

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}
void video_draw_vbo3d(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;

	struct glsrc* data = act->CTXBUF;
	float (*vbuf)[6] = data->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	//video_update(data->tex[0].data, 1024*1024*4, srcbuf, 640*480*2);
	data->tex[0].w = 640;
	data->tex[0].h = 480;
	data->tex[0].enq += 1;

	data->vbuf_enq += 1;
	gl41data_insert(ctx, 's', act->CTXBUF, 1);
}




void video_update(
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen)
{
	int x,y;
	u8* dst;
	u8* src;
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;

	for(y=0;y<480;y++)
	{
		dst = dstbuf + (y*640*4);
		src = srcbuf + (y*640*2);
		for(x=0;x<640;x+=2)
		{
			dst[4*x + 0] = src[2*x + 0];
			dst[4*x + 1] = src[2*x + 1];
			dst[4*x + 2] = src[2*x + 3];

			dst[4*x + 4] = src[2*x + 2];
			dst[4*x + 5] = src[2*x + 1];
			dst[4*x + 6] = src[2*x + 3];
		}
	}
}




void video_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* src;
	u8* dst;
	int cx, cy, ww, hh, j;
	int w = win->stride;
	int h = win->height;
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

	src = (u8*)(act->idx);
	if(0 == src)return;

	dst = (u8*)(win->buf);
	if(0 == dst)return;

	yuyv2rgba(
		  src, 0, 640, 480,     0,     0,     0,     0,
		  dst, 0,   w,   h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void video_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void video_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	//<head>
	htmlprintf(win, 1, ".video{width:50%%;height:100px;float:left;background-color:#1984ea;}\n");

	//<body>
	htmlprintf(win, 2, "<div class=\"video\">\n");
}
void video_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void video_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* src = act->idx;
	say("src@%llx\n", src);
}
static void video_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)video_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)video_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)video_draw_html(act, pin, win, sty);
	else if(fmt == _json_)video_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)video_draw_vbo2d(act, pin, win, sty);
		//else video_draw_vbo3d(act, pin, win, sty);
	}
	else video_draw_pixel(act, pin, win, sty);
}
void video_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev)
{
}




static void video_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> video
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)video_draw_vbo3d(act,part, win,geom, wnd,area);
	}
}
static void video_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct glsrc* data;
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(_yuv_ == self->flag){
		say("@video_write.yuv: %llx,%x,%llx,%x\n", arg, idx, buf, len);

		data = act->DATBUF;
		if(0 == data)return;
		if(0 == data->tex[0].data)return;

		video_update(data->tex[0].data, 1024*1024*4, buf, 640*480*2);
	}
}
static void video_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void video_start(struct halfrel* self, struct halfrel* peer)
{
}




static void video_search(struct entity* act)
{
}
static void video_modify(struct entity* act)
{
}
static void video_delete(struct entity* act)
{
	if(0 == act)return;
}
static void video_create(struct entity* act)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->CTXBUF = memorycreate(0x200, 0);
	if(0 == src)return;

	video_ctxforwnd(act->CTXBUF);
}




void video_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'i', 'd', 'e', 'o', 0, 0, 0);

	p->oncreate = (void*)video_create;
	p->ondelete = (void*)video_delete;
	p->onsearch = (void*)video_search;
	p->onmodify = (void*)video_modify;

	p->onstart = (void*)video_start;
	p->onstop  = (void*)video_stop;
	p->onread  = (void*)video_read;
	p->onwrite = (void*)video_write;
}
