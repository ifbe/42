#include "libuser.h"
void loadtexfromfile(struct glsrc* src, int idx, char* name);



/*
char* ground_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec2 texuvw;\n"
"void main(){\n"
	"texuvw = t;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";
*/
char* ground_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec2 t;\n"
"out mediump vec3 vertex;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char* ground_glsl_f =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"uniform sampler2D suntex;\n"
"uniform mat4 sunmvp;\n"
"float shadow(){\n"
	"mediump vec4 tmp = sunmvp * vec4(vertex, 1.0);\n"
	"tmp /= tmp.w;\n"
	"tmp = (tmp+1.0)*0.5;\n"
	"if(tmp.x < 0.0)return 0.5;\n"
	"if(tmp.x > 1.0)return 0.5;\n"
	"if(tmp.y < 0.0)return 0.5;\n"
	"if(tmp.y > 1.0)return 0.5;\n"
	"if(tmp.z - texture(suntex, tmp.xy).r > 0.001)return 0.5;\n"
	"return 1.0;\n"
"}\n"
"void main(){\n"
	"mediump float x = mod(abs(vertex.x), 1000.0) / 1000.0;\n"
	"mediump float y = mod(abs(vertex.y), 1000.0) / 1000.0;\n"
	"FragColor = vec4(shadow() * texture(tex0, vec2(x,y)).bgr, 1.0);\n"
"}\n";

//"layout(location = 0)out vec3 color;\n"
//"layout(location = 1)out vec3 hahah;\n"
	//"mediump float x = mod(abs(vertex.x), 1000.0) - 500.0;\n"
	//"mediump float y = mod(abs(vertex.y), 1000.0) - 500.0;\n"
	//"if(x>-490.0 && x<490.0 && y>-490.0 && y<490.0)discard;\n"
	//"color = vec3(1.0, 1.0, 0.0);\n"
	//"hahah = vec3(x, y, 0.0);\n"




static void ground_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void ground_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	struct datapair* dst;
	struct glsrc* src;
	float (*vbuf)[6];
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	dst = (void*)(sty->data[0]);
	if(0 == dst)return;
	src = (void*)(pin->data[0]);
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2] - 1.0;
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2] - 1.0;
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2] - 1.0;
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2] - 1.0;
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void ground_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ground_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ground_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ground_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void ground_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)ground_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)ground_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)ground_draw_html(act, pin, win, sty);
	else if(fmt == _json_)ground_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)ground_draw_vbo(act, pin, win, sty);
	else ground_draw_pixel(act, pin, win, sty);
}




static void ground_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@ground_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)ground_draw_vbo(act,pin,ctx,sty);
	}
}
static void ground_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void ground_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void ground_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->buf);
	say("@ground_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




static void ground_search(struct actor* act)
{
}
static void ground_modify(struct actor* act)
{
}
static void ground_delete(struct actor* act)
{
}
static void ground_create(struct actor* act, void* str)
{
	int j;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = ground_glsl_v;
	src->fs = ground_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_fmt[0] = hex32('r','g','b','a');
	src->tex_data[0] = memorycreate(2048*2048*4, 0);
	if(0 == str)str = "datafile/jpg/wall.jpg";
	loadtexfromfile(src, 0, str);
	src->tex_enq[0] = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




void ground_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'o', 'u', 'n', 'd', 0, 0);

	p->oncreate = (void*)ground_create;
	p->ondelete = (void*)ground_delete;
	p->onsearch = (void*)ground_search;
	p->onmodify = (void*)ground_modify;

	p->onstart = (void*)ground_start;
	p->onstop  = (void*)ground_stop;
	p->onread  = (void*)ground_read;
	p->onwrite = (void*)ground_write;
}
