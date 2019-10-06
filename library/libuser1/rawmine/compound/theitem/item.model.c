#include "libuser.h"
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void parsevertfromstl(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len);



/*
char* model_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 normal;\n"
"uniform mat4 objmat;\n"
"void main()\n"
"{\n"
	"normal = n;\n"
	"vertex = vec3(objmat * vec4(v, 1.0));\n"
	"gl_Position = objmat * vec4(v, 1.0);\n"
"}\n";
*/
char* model_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 normal;\n"
"uniform mat4 objmat;\n"
"uniform mat4 cammvp;\n"
"void main()\n"
"{\n"
	"normal = n;\n"
	"vertex = vec3(objmat * vec4(v, 1.0));\n"
	"gl_Position = cammvp * objmat * vec4(v, 1.0);\n"
"}\n";

char* model_glsl_f =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 normal;\n"
"out mediump vec4 FragColor;\n"
"uniform mediump vec3 camxyz;\n"
"uniform mediump vec3 sunxyz;\n"

//"mediump vec3 dirsun0 = vec3(1.0, 1.0, 1.0);\n"
//"mediump vec3 dirsun1 = vec3(-1.0, 0.0, 0.0);\n"

"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LD = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LS = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 sun0(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(sunxyz - vertex);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 25.0);\n"
"}\n"
/*
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
*/
//"float shadow(){\n"
	//"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	//"return 1.0;\n"
//"}\n"
"void main(){\n"
	"mediump vec3 light = LA*KA;\n"
	"light += sun0() / 2.0;\n"
	//"light += sun1() / 2.0;\n"
	"light = vec3(clamp(light.x, 0.0, 1.0), clamp(light.y, 0.0, 1.0), clamp(light.z, 0.0, 1.0));\n"
	"FragColor = vec4(light, 1.0);\n"
"}\n";




void sty_sty_mat(struct fstyle* src, struct fstyle* dst, mat4 mat)
{
	float x,y,z,max;
	float nr, nf, nu;
	mat4 tmp;

	//move: (0,0,0) -> dst
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = dst->vc[0];
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = dst->vc[1];
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = dst->vc[2];
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	//rotate: 1 -> dst

	//scale: 1 -> dst
	x = dst->vr[0];
	y = dst->vr[1];
	z = dst->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = dst->vf[0];
	y = dst->vf[1];
	z = dst->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = dst->vt[0];
	y = dst->vt[1];
	z = dst->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//scale: src -> 1
	x = src->vr[0];
	y = src->vr[1];
	z = src->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = src->vf[0];
	y = src->vf[1];
	z = src->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = src->vt[0];
	y = src->vt[1];
	z = src->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = 1.0 / max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0 / max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0 / max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//rotate: src -> 1

	//move: src -> (0,0,0)
	tmp[0][0] = 1.0;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = - src->vc[0];
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0;
	tmp[1][2] = 0.0;
	tmp[1][3] = - src->vc[1];
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0;
	tmp[2][3] = - src->vc[2];
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	mat4_transpose(mat);
}



static void model_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
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
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}/*
static void model_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	if(act->buf == 0)return;
	if(0 == sty)sty = defaultstyle_vbo2d();

	struct glsrc* src = (void*)(pin->foot[0]);
	sty_sty_mat(&act->target, &sty->f, (void*)src->arg_data[0]);
}*/
static void model_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	if(0 == act)return;
	if(act->buf == 0)return;

	struct glsrc* src = act->buf;
	sty_sty_mat(&pin->fs, &sty->fs, (void*)src->arg_data[0]);
}
static void model_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void model_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void model_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void model_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("model(%x,%x,%x)\n",win,act,sty);
}
static void model_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)model_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)model_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)model_draw_html(act, pin, win, sty);
	else if(fmt == _json_)model_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)model_draw_vbo2d(act, pin, win, sty);
		//else model_draw_vbo3d(act, pin, win, sty);
	}
	else model_draw_pixel(act, pin, win, sty);
}
static void model_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(_drag_ == type)
	{
		char buffer[0x1000];
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		//actorcreatefromfile(act, buffer);
	}
}




static void model_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	if(ctx){
		if(_gl41data_ == ctx->type)model_draw_vbo3d(act,pin,ctx,sty);
	}
	//model_draw(act, pin, win, sty);
}
static void model_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//model_event(act, pin, win, sty, ev, 0);
}
static void model_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void model_start(struct halfrel* self, struct halfrel* peer)
{
	struct glsrc* src;
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	//vertex
	src = act->buf;
	parsevertfromstl(src, &pin->fs, src->vbuf, src->vbuf_len);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_enq = 42;

	pin->data[0] = (u64)(act->buf);
	say("@texball_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




static void model_search(struct actor* act)
{
}
static void model_modify(struct actor* act)
{
}
static void model_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void model_create(struct actor* act, void* str)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = model_glsl_v;
	src->fs = model_glsl_f;
	src->shader_enq = 42;

	//argument
	src->arg_name[0] = "objmat";
	src->arg_data[0] = memorycreate(4*4*4, 0);
	src->arg_fmt[0] = 'm';

	//vertex
	src->vbuf_len = 0x1000000;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	if(0 == str)str = "datafile/stl/bunny-lowpoly.stl";
	j = openreadclose(str, 0, src->vbuf, src->vbuf_len);
}




void model_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 'd', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)model_create;
	p->ondelete = (void*)model_delete;
	p->onsearch = (void*)model_search;
	p->onmodify = (void*)model_modify;

	p->onstart = (void*)model_start;
	p->onstop  = (void*)model_stop;
	p->onread  = (void*)model_read;
	p->onwrite = (void*)model_write;
}
