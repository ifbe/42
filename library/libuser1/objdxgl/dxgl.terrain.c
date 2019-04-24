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
	"vec3 n = normalize(cross(a, b));\n"
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

"mediump vec3 dirsun0 = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 dirsun1 = vec3(-1.0, 0.0, 0.0);\n"

"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LD = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LS = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 sun0(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun0);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 25.0);\n"
"}\n"
"vec3 sun1(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun1);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 25.0);\n"
"}\n"
"void main(){\n"
	"mediump vec3 c = texture(tex0, texuvw.xy).bgr;\n"
	"c += sun0() / 6.0;\n"
	"c += sun1() / 6.0;\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




void terrain_generate(float (*vbuf)[6], u16* ibuf, struct actor* act)
{
	float f;
	int x,y,j;
	int x0,y0,x1,y1;
	u8* rgba = act->buf;
	int w = act->width;
	int h = act->height;
	float cx = (w-1) / 2.0;
	float cy = (h-1) / 2.0;

	for(y=0;y<256;y++)
	{
		for(x=0;x<256;x++)
		{
			x0 = x + act->x0;
			y0 = y + act->y0;

			//pixel ->  local xyz (->     world xyz)
			//    0 ->       -1.0 (-> -127.5*1000.0)
			//  127 -> -0.5/127.5 (->   -0.5*1000.0)
			//  128 ->  0.5/127.5 (->    0.5*1000.0)
			//  255 ->        1.0 (->  127.5*1000.0)
			vbuf[y*256+x][0] = x/127.5 - 1.0;
			vbuf[y*256+x][1] = y/127.5 - 1.0;

			//local ->        world ->               uv
			//    0 ->   0-127.5+cx -> (cx-127.5)/(w-1)
			//  127 -> 127-127.5+cx -> (cx  -0.5)/(w-1)
			//  128 -> 128-127.5+cx -> (cx  +0.5)/(w-1)
			//  255 -> 255-127.5+cx -> (cx+127.5)/(w-1)
			x1 = x0 - 128 + w/2;
			y1 = y0 - 128 + h/2;
			y1 = h-1 - y1;
			if((x1 < 0) | (x1 >= w) | (y1 < 0) | (y1 >= h))f = 0.0;
			else f = rgba[(w*y1 + x1) * 4] / 256.0;
			vbuf[y*256+x][2] = f;

			//local ->        world ->               uv
			//    0 ->   0-127.5+cx -> (cx-127.5)/(w-1)
			//  127 -> 127-127.5+cx -> (cx  -0.5)/(w-1)
			//  128 -> 128-127.5+cx -> (cx  +0.5)/(w-1)
			//  255 -> 255-127.5+cx -> (cx+127.5)/(w-1)
			vbuf[y*256+x][3] =       (x0-127.5+cx) / (w-1);
			vbuf[y*256+x][4] = 1.0 - (y0-127.5+cy) / (h-1);
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
	//geometry
	int w = act->width;
	int h = act->height;

	//center
	float sx = (w-1) / 2.0;
	float sy = (h-1) / 2.0;

	//position
	float fx = sx + v[0] / 1000.0;
	float fy = sy - v[1] / 1000.0;

	//
	int x0;
	int x1;
	if(fx > 0){x0 = (int)fx;x1 = x0 + 1;}
	else{x1 = -(int)(-fx);x0 = x1 - 1;}

	//
	int y0;
	int y1;
	if(fx > 0){y0 = (int)fy;y1 = y0 + 1;}
	else{y1 = -(int)(-fy);y0 = y1 - 1;}

	//
	//say("%d,%d,%d,%d\n",x0,x1,y0,y1);
	if(x0 < 0)goto edge;
	if(x1 >= w)goto edge;
	if(y0 < 0)goto edge;
	if(y1 >= h)goto edge;

	//
	float val;
	float max;
	u8* rgba = act->buf;
	max = rgba[(w*y0 + x0)*4];
	val = rgba[(w*y0 + x1)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x0)*4];
	if(val > max)max = val;
	val = rgba[(w*y1 + x1)*4];
	if(val > max)max = val;
	v[2] = max * 10000.0 / 256;
	return;

edge:
	v[2] = 0.0;
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
{/*
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
*/
	struct glsrc* src = (void*)(pin->foot[0]);
	if(0 == src)return;

	float* mat = src->arg_data[0];
	if(0 == mat)return;

	void* vbuf = src->vbuf;
	void* ibuf = src->ibuf;
	if(0 == vbuf)return;
	if(0 == ibuf)return;

	if(0 == act->w0){
		act->w0 = 42;

		terrain_generate(vbuf, ibuf, act);
		src->vbuf_enq += 1;

		mat[ 0] = 127.5*1000.0;
		mat[ 1] = 0.0;
		mat[ 2] = 0.0;
		mat[ 3] = 0.0;
		mat[ 4] = 0.0;
		mat[ 5] = 127.5*1000.0;
		mat[ 6] = 0.0;
		mat[ 7] = 0.0;
		mat[ 8] = 0.0;
		mat[ 9] = 0.0;
		mat[10] = 10.0*1000.0;
		mat[11] = 0.0;
		mat[12] = act->x0 * 1000.0;
		mat[13] = act->y0 * 1000.0;
		mat[14] = 0.0;
		mat[15] = 1.0;
	}
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
	float x,y;
	int w = act->width;
	int h = act->height;
	float* v = (void*)buf;

	terrain_locate(v, act);
	say("%f,%f,%f\n", v[0], v[1], v[2]);

	x = v[0] / 1000.0;
	act->xn = (int)x;
	while(act->xn <= act->x0-64){
		act->x0 -= 64;
		act->w0 = 0;
	}
	while(act->xn >= act->x0+64){
		act->x0 += 64;
		act->w0 = 0;
	}

	y = v[1] / 1000.0;
	act->yn = (int)y;
	while(act->yn <= act->y0-64){
		act->y0 -= 64;
		act->w0 = 0;
	}
	while(act->yn >= act->y0+64){
		act->y0 += 64;
		act->w0 = 0;
	}

	say("%d,%d,%d,%d\n", act->x0, act->y0, act->xn, act->yn);
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
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	if(0 == lf)return;

	pair = alloc_winobj(root);
	src = &pair->src;
	dst = &pair->dst;
	lf->foot[0] = (u64)src;
	tf->foot[0] = (u64)dst;

	//
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 0;

	//shader
	src->vs = terrain_glsl_v;
	src->gs = terrain_glsl_g;
	src->fs = terrain_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 256*255;
	src->vbuf_len = (src->vbuf_w) * 256*256;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_enq = 42;

	//index
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 254*254*2;
	src->ibuf_len = (src->ibuf_w) * 256*256*2;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_enq = 42;

	//argument
	src->arg_name[0] = "objmat";
	src->arg_data[0] = memorycreate(4*4*4);
	src->arg_fmt[0] = 'm';

	//texture
	src->tex_name[0] = "tex0";
	src->tex_data[0] = leaf->buf;
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_w[0] = leaf->width;
	src->tex_h[0] = leaf->height;
	src->tex_enq[0] = 42;
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
	u8* rgba;
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
				rgba[4*(y*2048 + x)+0] = getrandom()%256;
				rgba[4*(y*2048 + x)+1] = getrandom()%256;
				rgba[4*(y*2048 + x)+2] = getrandom()%256;
			}
		}
	}

	act->x0 = 0;
	act->y0 = 0;
	act->w0 = 0;
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
