#include "libuser.h"
#define GL41BUF buf1
#define RGBABUF buf0
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
void scale_image(void* src, void* dst,
	int sw, int sh, int sx1, int sy1, int sx2, int sy2,
	int dw, int dh, int dx1, int dy1, int dx2, int dy2);




#define DIM 1024
#define _sq(x) ((x)*(x))
#define r(n)(getrandom()%n)
static float vbuf[6][5] = {
	{0.0, 0.0, 0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 0.0, 1.0},
	{0.0, 0.0, 0.0, 1.0, 1.0},
	{0.0, 0.0, 0.0, 0.0, 1.0},
	{0.0, 0.0, 0.0, 1.0, 0.0}
};




char* codeimg_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* codeimg_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
"}\n";




unsigned char RED1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2))*255);
}
unsigned char GREEN1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2-2*arccos(-1)/3))*255);
}
unsigned char BLUE1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2+2*arccos(-1)/3))*255);
}
unsigned char RED2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):RED2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char GREEN2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):GREEN2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char BLUE2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):BLUE2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char RED3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char GREEN3(int i,int j)
{
	float x=0,y=0;
	int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char BLUE3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return 128-ln(k)*23;
}
unsigned char RED4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*((n-80)/800)*((n-80)/800)*((n-80)/800);
}
unsigned char GREEN4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*power((n-80)/800,0.7);
}
unsigned char BLUE4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*squareroot((n-80)/800);
}
unsigned char RED6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)((i+DIM)*s+y)%2+(int)((DIM*2-i)*s+y)%2)*127;
}
unsigned char GREEN6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(5*((i+DIM)*s+y))%2+(int)(5*((DIM*2-i)*s+y))%2)*127;
}
unsigned char BLUE6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(29*((i+DIM)*s+y))%2+(int)(29*((DIM*2-i)*s+y))%2)*127;
}




static void codeimg_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	int width = win->width;
	int stride = win->stride;
	int height = win->height;
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
	u32* src = (u32*)(act->RGBABUF);
	u32* dst = (u32*)(win->rgbabuf);

	scale_image(src, dst,
		1024, 1024, 0, 0, 1024, 1024,
		stride, height, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
static void codeimg_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float (*vbuf)[6];
	struct glsrc* src;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	if(0 == act->RGBABUF)return;

	src = act->GL41BUF;
	if(0 == src)return;
	vbuf = src->vbuf;
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

	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}
static void codeimg_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void codeimg_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void codeimg_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void codeimg_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("codeimg(%x,%x,%x)\n",win,act,sty);
}




static void codeimg_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void codeimg_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)codeimg_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void codeimg_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//codeimg_event(act, pin, win, sty, ev, 0);
}
static void codeimg_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void codeimg_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void codeimg_search(struct entity* act)
{
}
static void codeimg_modify(struct entity* act)
{
}
static void codeimg_delete(struct entity* act)
{
	if(0 == act)return;
}
static void codeimg_create(struct entity* act)
{
	int x,y;
	int rr,gg,bb;
	u32* rgba;
	struct glsrc* src;
	if(0 == act)return;

	//own
	rgba = act->RGBABUF = memorycreate(1024*1024*4, 0);
	act->width = 1024;
	act->height = 1024;
	for(y=0;y<1024;y++){
		for(x=0;x<1024;x++){
			rr = RED4(x,y);
			gg = GREEN4(x,y);
			bb = BLUE4(x,y);
			rgba[(y*1024)+x] = 0xff000000 + (rr<<16) + (gg<<8) + bb;
		}
	}

	//gl
	src = act->GL41BUF = memorycreate(0x200, 0);
	if(0 == src)return;

	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = codeimg_glsl_v;
	src->fs = codeimg_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);

	//texture
	src->tex[0].name = "tex0";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = act->RGBABUF;
	src->tex[0].w = act->width;
	src->tex[0].h = act->height;
	src->tex[0].enq = 42;
}




void codeimg_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'd', 'e', 'i', 'm', 'g', 0);

	p->oncreate = (void*)codeimg_create;
	p->ondelete = (void*)codeimg_delete;
	p->onsearch = (void*)codeimg_search;
	p->onmodify = (void*)codeimg_modify;

	p->onlinkup = (void*)codeimg_linkup;
	p->ondiscon = (void*)codeimg_discon;
	p->onread  = (void*)codeimg_read;
	p->onwrite = (void*)codeimg_write;
}
