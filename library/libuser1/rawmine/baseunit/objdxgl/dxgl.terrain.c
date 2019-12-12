#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void loadtexfromfile(struct glsrc* src, int idx, char* name);




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




void terrain_generate(float (*vbuf)[6], u16* ibuf, struct entity* act, struct glsrc* src)
{
	float f;
	int x,y,j;
	int x0,y0,x1,y1;
	u8* rgba = src->tex[0].data;
	int w = src->tex[0].w;
	int h = src->tex[0].h;
	float cx = (w-1) / 2.0;
	float cy = (h-1) / 2.0;

	for(y=0;y<256;y++)
	{
		for(x=0;x<256;x++)
		{
			x0 = x + act->fx0;
			y0 = y + act->fy0;

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
			else f = rgba[(w*y1 + x1) * 4] / 255.0;
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
void terrain_locate(vec4 v, struct entity* act)
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
	v[2] = max * 10000.0 / 255;
	return;

edge:
	v[2] = 0.0;
}




static void terrain_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
}
static void terrain_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
	struct glsrc* src = act->buf;
	if(0 == src)return;

	float* mat = src->arg[0].data;
	if(0 == mat)return;

	void* vbuf = src->vbuf;
	void* ibuf = src->ibuf;
	if(0 == vbuf)return;
	if(0 == ibuf)return;

	if(0 == act->iw0){
		act->iw0 = 42;

		terrain_generate(vbuf, ibuf, act, src);
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
		mat[12] = act->fx0 * 1000.0;
		mat[13] = act->fy0 * 1000.0;
		mat[14] = -10000.0;
		mat[15] = 1.0;
	}
}
static void terrain_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terrain_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terrain_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)terrain_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)terrain_draw_html(act, pin, win, sty);
	else if(fmt == _json_)terrain_draw_json(act, pin, win, sty);
	else terrain_draw_pixel(act, pin, win, sty);
}
static void terrain_ask(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	float x,y;
	struct entity* act = (void*)(self->chip);
	int w = act->width;
	int h = act->height;
	float* v = (void*)buf;

	terrain_locate(v, act);
	say("%f,%f,%f\n", v[0], v[1], v[2]);

	x = v[0] / 1000.0;
	act->fxn = (int)x;
	while(act->fxn <= act->fx0-64){
		act->fx0 -= 64;
		act->iw0 = 0;
	}
	while(act->fxn >= act->fx0+64){
		act->fx0 += 64;
		act->iw0 = 0;
	}

	y = v[1] / 1000.0;
	act->fyn = (int)y;
	while(act->fyn <= act->fy0-64){
		act->fy0 -= 64;
		act->iw0 = 0;
	}
	while(act->fyn >= act->fy0+64){
		act->fy0 += 64;
		act->iw0 = 0;
	}

	say("%f,%f,%f,%f\n", act->fx0, act->fy0, act->fxn, act->fyn);
}




static void terrain_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> terrain
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len){
			terrain_draw_vbo(act,part, win,geom, wrd,camg, wnd, area);
		}
	}
}
static void terrain_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void terrain_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void terrain_start(struct halfrel* self, struct halfrel* peer)
{
}




static void terrain_search(struct entity* act)
{
}
static void terrain_modify(struct entity* act)
{
}
static void terrain_delete(struct entity* act)
{
	if(0 == act)return;
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void terrain_create(struct entity* act, void* str)
{
	int j;
	int x,y,c;
	u8* buf;
	u8* rgba;
	struct glsrc* src;
	if(0 == act)return;

	act->fx0 = 0.0;
	act->fy0 = 0.0;
	act->iw0 = 0.0;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->method = 'i';
	src->geometry = 3;

	//shader
	src->vs = terrain_glsl_v;
	src->gs = terrain_glsl_g;
	src->fs = terrain_glsl_f;
	src->shader_enq = 42;

	//argument
	src->arg[0].name = "objmat";
	src->arg[0].data = memorycreate(4*4*4, 0);
	src->arg[0].fmt = 'm';

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	loadtexfromfile(src, 0, str);
	if((0 == src->tex[0].w) | (0 == src->tex[0].h))
	{
		src->tex[0].w = 2048;
		src->tex[0].h = 2048;

		rgba = src->tex[0].data;
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
	src->tex[0].enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_h = 256*255;
	src->vbuf_len = (src->vbuf_w) * 256*256;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 42;

	//index
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 254*254*2;
	src->ibuf_len = (src->ibuf_w) * 256*256*2;
	src->ibuf = memorycreate(src->ibuf_len, 0);
	src->ibuf_enq = 42;
}




void terrain_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'r', 'r', 'a', 'i', 'n', 0);

	p->oncreate = (void*)terrain_create;
	p->ondelete = (void*)terrain_delete;
	p->onsearch = (void*)terrain_search;
	p->onmodify = (void*)terrain_modify;

	p->onstart = (void*)terrain_start;
	p->onstop  = (void*)terrain_stop;
	p->onread  = (void*)terrain_read;
	p->onwrite = (void*)terrain_write;
}
