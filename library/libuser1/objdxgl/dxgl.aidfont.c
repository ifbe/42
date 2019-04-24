#include "libuser.h"
void actorcreatefromfile(struct actor* act, char* name);
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);




#ifdef __ANDROID__
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).aaaa;\n"
		"}\n"
	};
#else
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).rrrr;\n"
		"}\n"
	};
#endif

char font3dvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec2 t;\n"
"out mediump vec3 colour;\n"
"out mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"texuvw = t;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char font2dvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec2 t;\n"
"out mediump vec3 colour;\n"
"out mediump vec2 texuvw;\n"
"void main(){\n"
	"colour = c;\n"
	"texuvw = t;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";




static void aidfont_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	struct datapair* mod = win->mod;
	if(_vbo_ != win->fmt)return;

	//reuse shader
	mod[font3d1].dst.shader = mod[font3d0].dst.shader;
	mod[font3d2].dst.shader = mod[font3d0].dst.shader;
	mod[font3d3].dst.shader = mod[font3d0].dst.shader;

	mod[font2d1].dst.shader = mod[font2d0].dst.shader;
	mod[font2d2].dst.shader = mod[font2d0].dst.shader;
	mod[font2d3].dst.shader = mod[font2d0].dst.shader;

	//reuse tex
	mod[font2d0].dst.tex[0] = mod[font3d0].dst.tex[0];
	mod[font2d0].src.tex_name[0] = mod[font3d0].src.tex_name[0];
	mod[font2d1].dst.tex[0] = mod[font3d1].dst.tex[0];
	mod[font2d1].src.tex_name[0] = mod[font3d1].src.tex_name[0];
	mod[font2d2].dst.tex[0] = mod[font3d2].dst.tex[0];
	mod[font2d2].src.tex_name[0] = mod[font3d2].src.tex_name[0];
	mod[font2d3].dst.tex[0] = mod[font3d3].dst.tex[0];
	mod[font2d3].src.tex_name[0] = mod[font3d3].src.tex_name[0];
}
static void aidfont_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void aidfont_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void aidfont_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void aidfont_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void aidfont_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	u8* buf;
	struct glsrc* src;
	struct datapair* mod;

	buf = leaf->buf;
	mod = root->mod;

//--------------------font3d-------------------
	//[0000,3fff]
	src = &mod[font3d0].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vs = font3dvert;
	src->fs = fontfrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;

	src->tex_name[0] = "tex0";
	src->tex_data[0] = buf + 0;
	src->tex_w[0] = 2048;
	src->tex_h[0] = 2048;
	src->tex_fmt[0] = hex32('o','n','e', 0);
	src->tex_enq[0] = 1;


	//[4000,7fff]
	src = &mod[font3d1].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;

	src->tex_name[0] = "tex0";
	src->tex_data[0] = buf + 0x400000;
	src->tex_w[0] = 2048;
	src->tex_h[0] = 2048;
	src->tex_fmt[0] = hex32('o','n','e', 0);
	src->tex_enq[0] = 1;


	//[8000,bfff]
	src = &mod[font3d2].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;

	src->tex_name[0] = "tex0";
	src->tex_data[0] = buf + 0x800000;
	src->tex_w[0] = 2048;
	src->tex_h[0] = 2048;
	src->tex_fmt[0] = hex32('o','n','e', 0);
	src->tex_enq[0] = 1;


	//[c000,ffff]
	src = &mod[font3d3].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;

	src->tex_name[0] = "tex0";
	src->tex_data[0] = buf + 0xc00000;
	src->tex_w[0] = 2048;
	src->tex_h[0] = 2048;
	src->tex_fmt[0] = hex32('o','n','e', 0);
	src->tex_enq[0] = 1;


//--------------------font2d-------------------
	//[0000,3fff]
	src = &mod[font2d0].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vs = font2dvert;
	src->fs = fontfrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;


	//[4000,7fff]
	src = &mod[font2d1].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;


	//[8000,bfff]
	src = &mod[font2d2].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;


	//[c000,ffff]
	src = &mod[font2d3].src;
	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_len = 0x200000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;
}
static void aidfont_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void aidfont_create(struct actor* act, void* str)
{
	int j;
	u8* buf;
	if(0 == act)return;

	//
	act->buf = buf = memorycreate(2048*2048*4);
	if(0 == buf)return;

	//clean
	for(j=0;j<2048*2048*4;j++)buf[j] = 0;

	//utf8
	for(j=0x80;j<0x4000;j++){
		drawunicode_alpha(buf+0, 2048, 2048, 
			(j&0x7f)<<4, (j&0xff80)>>3, j
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0x400000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0x800000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}
	for(j=0;j<0x4000;j++){
		drawunicode_alpha(buf+0xc00000, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	//failsafe
	for(j=0x20;j<0x80;j++){
		drawascii_alpha(buf+0, 2048, 2048,
			j<<4, 0, j
		);
	}
}




void aidfont_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('a', 'i', 'd', 'f', 'o', 'n', 't', 0);

	p->oncreate = (void*)aidfont_create;
	p->ondelete = (void*)aidfont_delete;
	p->onstart  = (void*)aidfont_start;
	p->onstop   = (void*)aidfont_stop;
	p->oncread  = (void*)aidfont_cread;
	p->oncwrite = (void*)aidfont_cwrite;
	p->onsread  = (void*)aidfont_sread;
	p->onswrite = (void*)aidfont_swrite;
}
