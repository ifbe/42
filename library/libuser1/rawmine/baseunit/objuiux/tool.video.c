#include "libuser.h"
#define _cam_ hex32('c','a','m',0)
#define _yuv_ hex32('y','u','v',0)
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);
void* actorcreate(u64, void*);




//opengl shader
/*
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
*/
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
void video_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	struct glsrc* data = act->buf;
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

	data->vbuf_enq += 1;


	struct relation* rel = act->orel0;
	if(0 == rel)return;

	u8* srcbuf = 0;
	relationread(act, _src_, 0, 0, &srcbuf, 0);
	say("srcbuf=%llx\n",srcbuf);
	if(0 == srcbuf)return;

	video_update(data->tex_data[0], 1024*1024*4, srcbuf, 640*480*2);
	data->tex_w[0] = 640;
	data->tex_h[0] = 480;
	data->tex_enq[0] += 1;
}
void video_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
void video_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	//<head>
	htmlprintf(win, 1, ".video{width:50%%;height:100px;float:left;background-color:#1984ea;}\n");

	//<body>
	htmlprintf(win, 2, "<div class=\"video\">\n");
}
void video_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
void video_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u8* src = act->idx;
	say("src@%llx\n", src);
}
static void video_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev)
{
}




static void video_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@texball_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)video_draw_vbo3d(act,pin,ctx,sty);
	}
	//video_draw(act, pin, win, sty);
}
static void video_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	//struct actor* win = (void*)(peer->chip);
	//struct style* sty = (void*)(peer->foot);
	if(_yuv_ == self->flag){
		say("@video_write.yuv: %llx,%x\n", buf, len);
	}
/*
	switch(self->flag){
		case _yuv_:video_update(act, pin, win, sty, buf, len);break;
		default:   video_event( act, pin, win, sty, buf);break;
	}
*/
}
static void video_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void video_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->buf);
	say("@video_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




static void video_search(struct actor* act)
{
}
static void video_modify(struct actor* act)
{
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
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = video_glsl_v;
	src->fs = video_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_data[0] = memorycreate(1024*1024*4, 0);
	src->tex_fmt[0] = hex32('r','g','b','a');

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




void video_register(struct actor* p)
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
