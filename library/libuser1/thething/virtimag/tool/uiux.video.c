#include "libuser.h"
#define OWNBUF buf0
#define _cam_ hex32('c','a','m',0)
#define _yuv_ hex32('y','u','v',0)
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);


struct own{
	union{
		u64 must8;
		void* yuyv;
	};
	int width;
	int height;

	struct gl41data gl41;
};


char* video_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uv;\n"
"void main(){\n"
	"uv = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* video_glsl_f = 
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uv;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump vec3 yuv = texture(tex0, uv).rgb;\n"
	"mediump float y = yuv.r;\n"
	"mediump float u = yuv.g - 0.5;\n"
	"mediump float v = yuv.b - 0.5;\n"
	"mediump float r = y + 1.402*v;\n"
	"mediump float g = y - 0.34414*u - 0.71414*v;\n"
	"mediump float b = y + 1.1772*u;\n"
	"FragColor = vec4(r, g, b, 1.0);\n"
"}\n";
static void video_ctxforwnd(struct glsrc* src)
{
	//shader
	src->vs = video_glsl_v;
	src->fs = video_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(1024*1024*4, 0);
	src->tex[0].fmt = hex32('r','g','b','a');

	//vertex
	src->geometry = 3;
	src->opaque = 0;

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}
void video_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;

	struct own* own = act->OWNBUF;
	if(0 == own)return;
	struct glsrc* data = &own->gl41.src;
	if(0 == data)return;
	float (*vbuf)[6] = data->vbuf;
	if(0 == vbuf)return;

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
	gl41data_insert(ctx, 's', data, 1);
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
	struct own* own;
	u8* dst;
	int cx, cy, ww, hh, j;
	int w = win->fbwidth;
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

	own = act->OWNBUF;
	if(0 == own)return;

	dst = (u8*)(win->rgbabuf);
	if(0 == dst)return;

	yuyv2rgba(
		  own->yuyv, 0, 640, 480,     0,     0,     0,     0,
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
}
void video_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev)
{
}




static void video_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		video_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void video_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* sty = stack[sp-2].pfoot;
	if(_rgba_ == wnd->fmt){
		video_draw_pixel(ent,0, wnd, sty);
	}
	else{
		video_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void video_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct own* own = ent->OWNBUF;
	if(0 == own)return;

	if(_yuv_ == foot){
		say("@video_write.yuv: %llx,%x,%llx,%x\n", arg, key, buf, len);
		own->yuyv = buf;

		if(0 == own->gl41.src.tex[0].data)return;
		video_update(own->gl41.src.tex[0].data, 1024*1024*4, buf, 640*480*2);
	}
}
static void video_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void video_linkup(struct halfrel* self, struct halfrel* peer)
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
	if(0 == act)return;

	struct own* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	video_ctxforwnd(&own->gl41.src);
}




void video_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'i', 'd', 'e', 'o', 0, 0, 0);

	p->oncreate = (void*)video_create;
	p->ondelete = (void*)video_delete;
	p->onsearch = (void*)video_search;
	p->onmodify = (void*)video_modify;

	p->onlinkup = (void*)video_linkup;
	p->ondiscon = (void*)video_discon;
	p->onread  = (void*)video_read;
	p->onwrite = (void*)video_write;
}
