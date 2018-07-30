#include "libuser.h"
void* allocofoot();
void actorcreatefromfile(struct actor* act, char* name);
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
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* codeimg_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
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




static void codeimg_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int width = win->width;
	int stride = win->stride;
	int height = win->height;
	int cx, cy, ww, hh;
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
	u32* src = (u32*)(act->buf);
	u32* dst = (u32*)(win->buf);

	scale_image(src, dst,
		1024, 1024, 0, 0, 1024, 1024,
		stride, height, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
static void codeimg_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float (*vbuf)[6];
	struct ofoot* opin;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	if(0 == act->buf)return;

	opin = (void*)(pin->foot[0]);
	vbuf = (void*)(opin->vbuf);

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

	opin->vbuf_enq += 1;
}
static void codeimg_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void codeimg_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void codeimg_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void codeimg_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("codeimg(%x,%x,%x)\n",win,act,sty);
}
static void codeimg_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)codeimg_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)codeimg_read_tui(win, sty, act, pin);
	else if(fmt == _html_)codeimg_read_html(win, sty, act, pin);
	else if(fmt == _json_)codeimg_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)codeimg_read_vbo(win, sty, act, pin);
	else codeimg_read_pixel(win, sty, act, pin);
}
static void codeimg_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void codeimg_list()
{
}
static void codeimg_change()
{
}
static void codeimg_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void codeimg_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct ofoot* opin;
	if(0 == pin)return;

	//
	opin = allocofoot();

	//shader
	opin->vs = (u64)codeimg_glsl_v;
	opin->fs = (u64)codeimg_glsl_f;

	//texture
	opin->tex[0] = (u64)(act->buf);
	opin->tex_fmt[0] = hex32('r','g','b','a');
	opin->tex_w[0] = act->width;
	opin->tex_h[0] = act->height;

	//vertex
	opin->vbuf = (u64)memorycreate(4*6*6);
	opin->vbuf_fmt = vbuffmt_33;
	opin->vbuf_w = 6*4;
	opin->vbuf_h = 6;
	opin->method = 'v';

	//send!
	opin->shader_enq[0] = 42;
	opin->arg_enq[0] = 0;
	opin->tex_enq[0] = 42;
	opin->vbuf_enq = 0;
	opin->ibuf_enq = 0;
	pin->foot[0] = (u64)opin;
}
static void codeimg_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 != act->buf)memorydelete(act->buf);
	act->buf = 0;
}
static void codeimg_create(struct actor* act)
{
	int rr,gg,bb;
	int x,y;
	u32* src;
	if(0 == act)return;

	src = memorycreate(1024*1024*4);
	act->buf = src;
	act->width = 1024;
	act->height = 1024;

	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			rr = RED4(x,y);
			gg = GREEN4(x,y);
			bb = BLUE4(x,y);
			src[(y*1024)+x] = 0xff000000 + (rr<<16) + (gg<<8) + bb;
		}
	}
}




void codeimg_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('c', 'o', 'd', 'e', 'i', 'm', 'g', 0);

	p->oncreate = (void*)codeimg_create;
	p->ondelete = (void*)codeimg_delete;
	p->onstart  = (void*)codeimg_start;
	p->onstop   = (void*)codeimg_stop;
	p->onlist   = (void*)codeimg_list;
	p->onchoose = (void*)codeimg_change;
	p->onread   = (void*)codeimg_read;
	p->onwrite  = (void*)codeimg_write;
}
