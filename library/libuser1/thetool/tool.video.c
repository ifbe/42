#include "libuser.h"
#define _cam_ hex32('c','a','m',0)
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);
void* arenacreate(u64, void*);




//opengl shader
char* video_glsl2d_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"out mediump vec2 uv;\n"
	"void main()\n"
	"{\n"
		"uv = texuvw;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n";
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




void video_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8* src;
	u8* dst;
	int cx, cy, ww, hh, j;
	int w = win->stride;
	int h = win->height;
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

	src = (u8*)(act->idx);
	if(0 == src)return;

	dst = (u8*)(win->buf);
	if(0 == dst)return;

	yuyv2rgba(
		  src, 0, 640, 480,     0,     0,     0,     0,
		  dst, 0,   w,   h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void video_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8* dst;
	u8* src;
	float (*vbuf)[6];
	struct glsrc* data;
	if(0 == act->idx)return;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;

	data = (void*)(pin->foot[0]);
	vbuf = data->vbuf;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 480/1024.0;//1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 640/1024.0;//0.0;
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
	vbuf[3][3] = 640/1024.0;//0.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 480/1024.0;//1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 640/1024.0;//1.0;
	vbuf[5][4] = 480/1024.0;//1.0;
	vbuf[5][5] = 0.0;

	data->tex[0] = act->buf;
	for(y=0;y<480;y++)
	{
		dst = (act->buf) + (y*1024*4);
		src = (act->idx) + (y*320*4);
		for(x=0;x<320;x++)
		{
			dst[8*x + 0] = src[4*x + 0];
			dst[8*x + 1] = src[4*x + 1];
			dst[8*x + 2] = src[4*x + 3];

			dst[8*x + 4] = src[4*x + 2];
			dst[8*x + 5] = src[4*x + 1];
			dst[8*x + 6] = src[4*x + 3];
		}
	}

	data->vbuf_enq += 1;
	data->tex_enq[0] += 1;
}
void video_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8* dst;
	u8* src;
	float (*vbuf)[6];
	struct glsrc* data;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->idx)return;

	data = (void*)(pin->foot[0]);
	vbuf = data->vbuf;

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

	data->tex[0] = act->buf;
	for(y=0;y<480;y++)
	{
		dst = (act->buf) + (y*1024*4);
		src = (act->idx) + (y*320*4);
		for(x=0;x<320;x++)
		{
			dst[8*x + 0] = src[4*x + 0];
			dst[8*x + 1] = src[4*x + 1];
			dst[8*x + 2] = src[4*x + 3];

			dst[8*x + 4] = src[4*x + 2];
			dst[8*x + 5] = src[4*x + 1];
			dst[8*x + 6] = src[4*x + 3];
		}
	}

	data->vbuf_enq += 1;
	data->tex_enq[0] += 1;
}
void video_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void video_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	//<head>
	htmlprintf(win, 1, ".video{width:50%%;height:100px;float:left;background-color:#1984ea;}\n");

	//<body>
	htmlprintf(win, 2, "<div class=\"video\">\n");
}
void video_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void video_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8* src = act->idx;
	say("src@%llx\n", src);
}
static void video_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)video_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)video_read_tui(win, sty, act, pin);
	else if(fmt == _html_)video_read_html(win, sty, act, pin);
	else if(fmt == _json_)video_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)video_read_vbo2d(win, sty, act, pin);
		else video_read_vbo3d(win, sty, act, pin);
	}
	else video_read_pixel(win, sty, act, pin);
}
static void video_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	if(0 == win)return;

	act->idx = buf;
}
static void video_get()
{
}
static void video_post()
{
}
static void video_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	struct glsrc* src;
	if(0 == lf)return;

	src = (void*)(lf->foot[0]);
	if(src->vbuf)
	{
		memorydelete(src->vbuf);
		src->vbuf = 0;
	}
}
static void video_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;
	if(0 == tf)return;
	if(_vbo_ != root->fmt)return;

	//
	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//shader
	src->vs = video_glsl_v;
	src->fs = video_glsl_f;
	if(twig)
	{
		if(_fg2d_ == twig->fmt)src->vs = video_glsl2d_v;
	}

	//texture
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex[0] = leaf->buf;
	src->tex_w[0] = 1024;
	src->tex_h[0] = 1024;

	//vertex
	src->vbuf = memorycreate(4*6*6);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->method = 'v';

	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 0;
	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void video_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void video_create(struct actor* act)
{
	struct arena* win;
	if(0 == act)return;

	act->buf = memorycreate(0x400000);

	win = arenacreate(_cam_, "0");
	if(0 == win)return;

	relationcreate(act, 0, _act_, win, 0, _win_);
}




void video_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('v', 'i', 'd', 'e', 'o', 0, 0, 0);

	p->oncreate = (void*)video_create;
	p->ondelete = (void*)video_delete;
	p->onstart  = (void*)video_start;
	p->onstop   = (void*)video_stop;
	p->onget    = (void*)video_get;
	p->onpost   = (void*)video_post;
	p->onread   = (void*)video_read;
	p->onwrite  = (void*)video_write;
}
