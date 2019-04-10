#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void actorcreatefromfile(struct actor* act, char* name);




char* terrain_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 t;\n"
"out mediump vec3 uvw;\n"
"out mediump vec3 xyz;\n"
"uniform mat4 objmat;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = t;\n"
	"xyz = vec3(objmat * vec4(v, 1.0));\n"
	"gl_Position = cammvp * objmat * vec4(v, 1.0);\n"
"}\n";

char* terrain_glsl_g =
GLSL_VERSION
"layout(triangles) in;\n"
//"layout(line_strip, max_vertices = 6) out;\n"
"layout(triangle_strip, max_vertices = 6) out;\n"
"in mediump vec3 uvw[];\n"
"in mediump vec3 xyz[];\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 texuvw;\n"
"out mediump vec3 normal;\n"
"void main(){\n"
	"vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);\n"
	"vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);\n"
	"vec3 n = 1000*normalize(cross(a, b));\n"
	"gl_Position = gl_in[0].gl_Position;\n"
	"vertex = xyz[0];\n"
	"texuvw = uvw[0];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"gl_Position = gl_in[1].gl_Position;\n"
	"vertex = xyz[1];\n"
	"texuvw = uvw[1];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"gl_Position = gl_in[2].gl_Position;\n"
	"vertex = xyz[2];\n"
	"texuvw = uvw[2];\n"
	"normal = n;\n"
	"EmitVertex();\n"
	"EndPrimitive();\n"
"}\n";

char* terrain_glsl_f =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 texuvw;\n"
"in mediump vec3 normal;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform mediump vec3 camxyz;\n"
"mediump vec3 dirsun0 = vec3(-1.0, 0.0, 1.0);\n"
"mediump vec3 dirsun1 = vec3(0.0, -1.0, 1.0);\n"
"mediump vec3 sunxyz = vec3(1000000.0, 1000000.0, 1000000.0);\n"
"mediump vec3 kambi = vec3(0.231250, 0.231250, 0.231250);\n"
"mediump vec3 kdiff = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 kspec = vec3(0.773911, 0.773911, 0.773911);\n"
"vec3 blinnphong(){\n"
	"vec3 lightcolor = 0.3*vec3(0.9, 0.9, 1.0);\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(sunxyz - vertex);\n"
	"mediump float SN = max(dot(N, L), 0.0);\n"
	"mediump vec3 ret = kambi + kdiff*SN;\n"
	"if(SN<0.0)return lightcolor*ret;\n"

	//blinn phong
	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"ret += kspec*pow(NH, 25.0);\n"

	"return lightcolor * ret;\n"
"}\n"
"vec3 sun0(){\n"
	"vec3 lightcolor = 0.3*vec3(1.0, 0.9, 0.9);\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun0);\n"
	"mediump float SN = max(dot(N, L), 0.0);\n"
	"mediump vec3 ret = kambi + kdiff*SN;\n"
	"if(SN<0.0)return lightcolor*ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"ret += kspec*pow(NH, 25.0);\n"

	"return lightcolor * ret;\n"
"}\n"
"vec3 sun1(){\n"
	"vec3 lightcolor = 0.3*vec3(0.9, 1.0, 0.9);\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun1);\n"
	"mediump float SN = max(dot(N, L), 0.0);\n"
	"mediump vec3 ret = kambi + kdiff*SN;\n"
	"if(SN<0.0)return lightcolor*ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"ret += kspec*pow(NH, 25.0);\n"

	"return lightcolor * ret;\n"
"}\n"
"void main(){\n"
	//"FragColor = vec4(normal, 1.0);\n"
	"FragColor = vec4(blinnphong() + sun0() + sun1(), 1.0);\n"
"}\n";




void terrain_generate(float (*vbuf)[6], u16* ibuf, struct actor* act)
{
	int x,y,x1,y1,j;
	int w = act->width;
	int h = act->height;
	u8* rgba = act->buf;

	for(y=0;y<255;y++)
	{
		for(x=0;x<255;x++)
		{
			//vertex
			vbuf[y*256+x][0] = x/127.0 - 1.0;
			vbuf[y*256+x][1] = y/127.0 - 1.0;
			x1 = x*w/256;
			y1 = (255-y)*h/256;
			//x1 = x+256;
			//y1 = y+512;
			vbuf[y*256+x][2] = rgba[(w*y1 + x1) * 4] / 256.0;
//say("%f\n",vbuf[y*256+x][2]);
			//uv
			vbuf[y*256+x][3] = x*1.0;
			vbuf[y*256+x][4] = y*1.0;
			vbuf[y*256+x][5] = 0.0;
		}
	}

	j = 0;
	for(y=0;y<254;y++)
	{
		for(x=0;x<254;x++)
		{
			ibuf[j+0] = y*256+x;
			ibuf[j+1] = y*256+x+1;
			ibuf[j+2] = y*256+x+256;

			ibuf[j+3] = y*256+x+1;
			ibuf[j+4] = y*256+x+257;
			ibuf[j+5] = y*256+x+256;

			j += 6;
		}
	}
}
void terrain_locate(vec4 v, struct actor* act)
{
	int x,y;
	int w = act->width;
	int h = act->height;
	u8* rgba = act->buf;

	x = w * (200000.0 + v[0]) / 400000.0;
	y = h * (200000.0 - v[1]) / 400000.0;
	if( (x<0) | (x>=w) | (y<0) | (y>=h) )v[2] = 0.0;
	else v[2] = rgba[(w*y + x) * 4] * 8000.0 / 256.0;
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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	float* v = (void*)buf;
	terrain_locate(v, act);
	say("%f,%f,%f\n", v[0], v[1], v[2]);
}
static void terrain_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	say("@terrain_cwrite\n");
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
	src->gs = terrain_glsl_g;
	src->fs = terrain_glsl_f;

	//argument
	float* mat = memorycreate(4*4*4);
	mat[ 0] = 200.0*1000.0;
	mat[ 1] = 0.0;
	mat[ 2] = 0.0;
	mat[ 3] = 0.0;
	mat[ 4] = 0.0;
	mat[ 5] = 200.0*1000.0;
	mat[ 6] = 0.0;
	mat[ 7] = 0.0;
	mat[ 8] = 0.0;
	mat[ 9] = 0.0;
	mat[10] = 8000.0;
	mat[11] = 0.0;
	mat[12] = tf->vc[0];
	mat[13] = tf->vc[1];
	mat[14] = tf->vc[2];
	mat[15] = 1.0;
	src->arg_data[0] = (u64)mat;
	src->arg[0] = "objmat";
/*
	//texture
	src->tex[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;
*/
	//vertex
	vbuf = memorycreate(4*6 * 256*255);
	ibuf = memorycreate(2*3 * 256*256*2);
	terrain_generate(vbuf, ibuf, leaf);
	src->method = 'i';

	src->vbuf_fmt = vbuffmt_33;
	src->vbuf = vbuf;
	src->vbuf_w = 4*6;
	src->vbuf_h = 256*255;
	src->ibuf_fmt = 0x222;
	src->ibuf = ibuf;
	src->ibuf_w = 2*3;
	src->ibuf_h = 254*254*2;

	//send!
	src->shader_enq[0] = 42;
	src->arg_enq[0] = 42;
	//src->tex_enq[0] = 42;
	src->vbuf_enq = 42;
	src->ibuf_enq = 42;
}
static void terrain_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void terrain_create(struct actor* act, void* str)
{
	int x,y,c;
	u32* rgba;
	if(0 == act)return;

	//max=16MB
	if(0 == act->buf)act->buf = memorycreate(2048*2048*4);

	//try file
	if(0 == str)str = "datafile/jpg/terrain.jpg";
	actorcreatefromfile(act, str);

	//gen terr
	rgba = act->buf;
	if((0 == act->width) | (0 == act->height))
	{
		act->width = 2048;
		act->height = 2048;
		for(y=0;y<2048;y++)
		{
			for(x=0;x<2048;x++)
			{
				rgba[y*2048 + x] = getrandom();
			}
		}
	}
}




void terrain_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onstart  = (void*)terrain_start;
	p->onstop   = (void*)terrain_stop;
	p->oncread  = (void*)terrain_cread;
	p->oncwrite = (void*)terrain_cwrite;
	p->onsread  = (void*)terrain_sread;
	p->onswrite = (void*)terrain_swrite;
}
