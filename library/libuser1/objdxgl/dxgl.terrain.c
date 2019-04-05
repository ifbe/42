#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* terrain_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 v;\n"
	"layout(location = 1)in mediump vec3 n;\n"
	"layout(location = 2)in mediump vec2 t;\n"
	"out mediump vec2 texuvw;\n"
	"out mediump vec3 normal;\n"
	"uniform mat4 cammvp;\n"
	"void main()\n"
	"{\n"
		"texuvw = t;\n"
		"normal = n;\n"
		"gl_Position = cammvp * vec4(v, 1.0);\n"
	"}\n";
char* terrain_glsl_f =
	GLSL_VERSION
	"in mediump vec2 texuvw;\n"
	"in mediump vec3 normal;\n"
	"out mediump vec4 FragColor;\n"
	"uniform sampler2D tex0;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(texture(tex0, texuvw).bgr, 1.0);\n"
	"}\n";
void terrain_generate(float (*vbuf)[9], u16* ibuf, float w, float h)
{
	int x,y,j;
	for(y=0;y<255;y++)
	{
		for(x=0;x<255;x++)
		{
			//vertex
			vbuf[y*256+x][0] = x*w/127.0 - w;
			vbuf[y*256+x][1] = y*h/127.0 - h;
			vbuf[y*256+x][2] = (float)(getrandom()%16384) - 8192.0;		//8848?
//say("%f\n",vbuf[y*256+x][2]);
			//uv
			vbuf[y*256+x][6] = x*1.0;
			vbuf[y*256+x][7] = y*1.0;
			vbuf[y*256+x][8] = 0.0;
		}
	}

	for(y=1;y<254;y++)
	{
		for(x=1;x<254;x++)
		{
			//normal
			vbuf[y*256+x][3] = 0.0;
			vbuf[y*256+x][4] = 0.0;
			vbuf[y*256+x][5] = 1.0;
		}
	}

	j = 0;
	for(y=0;y<254;y++)
	{
		for(x=0;x<254;x++)
		{
			ibuf[j] = y*256+x;
			ibuf[j+1] = y*256+x+1;
			ibuf[j+2] = y*256+x+256;

			ibuf[j+3] = y*256+x+1;
			ibuf[j+4] = y*256+x+256;
			ibuf[j+5] = y*256+x+257;

			j += 6;
		}
	}
}




static void terrain_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
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
}
static void terrain_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	//if(0 == act->buf)return;
}
static void terrain_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terrain_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)terrain_read_tui(win, sty, act, pin);
	else if(fmt == _html_)terrain_read_html(win, sty, act, pin);
	else if(fmt == _json_)terrain_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)terrain_read_vbo(win, sty, act, pin);
	else terrain_read_pixel(win, sty, act, pin);
}
static void terrain_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void terrain_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrain_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void terrain_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void terrain_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	void* vbuf;
	void* ibuf;
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//shader
	src->vs = terrain_glsl_v;
	src->fs = terrain_glsl_f;

	//texture
	src->tex[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;

	//vertex
	vbuf = memorycreate(4*9 * 256*255);
	ibuf = memorycreate(2*3 * 256*256*2);
	terrain_generate(vbuf, ibuf, tf->vr[0], tf->vf[1]);
	src->method = 'i';

	src->vbuf_fmt = vbuffmt_333;
	src->vbuf = vbuf;
	src->vbuf_w = 4*9;
	src->vbuf_h = 256*255;
	src->ibuf_fmt = 0x222;
	src->ibuf = ibuf;
	src->ibuf_w = 2*3;
	src->ibuf_h = 254*254*2;

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 0;
	src->tex_enq[0] = 42;
	src->vbuf_enq = 42;
	src->ibuf_enq = 42;
}
static void terrain_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void terrain_create(struct actor* act)
{
	if(0 == act)return;
	actorcreatefromfile(act, "datafile/jpg/terrain.jpg");
}




void terrain_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onstart  = (void*)terrain_start;
	p->onstop   = (void*)terrain_stop;
	p->onget    = (void*)terrain_cread;
	p->onpost   = (void*)terrain_cwrite;
	p->onread   = (void*)terrain_sread;
	p->onwrite  = (void*)terrain_swrite;
}
