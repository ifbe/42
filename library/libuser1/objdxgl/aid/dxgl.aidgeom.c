#include "libuser.h"
void actorcreatefromfile(struct actor* act, char* name);




char easy2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char easy2d_frag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




char simplevert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char simplefrag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




char glsl2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 normal;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"colour = c;\n"
	"normal = vec3(0.0, 0.0, -1.0);\n"
	"gl_Position = cammvp * vec4(vertex,1.0);\n"
"}\n";

char glsl2d_frag[] =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"

"mediump vec3 campos = vec3(0.0, 0.0, -1.0);\n"
"mediump vec3 dirsun0 = vec3(1.0, 1.0, 1.0);\n"

"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LD = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LS = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 blinnphong(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun0);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(campos - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
"void main(){\n"
	"mediump vec3 c = colour + blinnphong();\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




char prettyvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec3 n;\n"
"out mediump vec3 normal;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"normal = n;\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(vertex,1.0);\n"
"}\n";

char prettyfrag[] =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
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
	"return ret + LS*KS*pow(NH, 89.6);\n"
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
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
//"float shadow(){\n"
	//"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	//"return 1.0;\n"
//"}\n"
"void main(){\n"
	"mediump vec3 c = colour;\n"
	"c += sun0() / 2.0;\n"
	"c += sun1() / 2.0;\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




char opaque2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec4 c;\n"
"out mediump vec4 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char opaque2d_frag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




char opaquevert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"layout(location = 2)in mediump vec4 c;\n"
"out mediump vec4 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char opaquefrag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




void initshader12341234(struct arena* win)  
{
/*
	int j;
	GLuint tmp;
	struct datapair* mod = win->mod;

	//1.check version
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor: %s\n", vendor);
	printf("GL Renderer: %s\n", renderer);
	printf("GL Version (string): %s\n", version);
	printf("GLSL Version: %s\n", glslVersion);
	printf("GL Version (integer): %x.%x\n", major, minor);
*/
}




static void aidgeom_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void aidgeom_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void aidgeom_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void aidgeom_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void aidgeom_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void aidgeom_start(struct halfrel* self, struct halfrel* peer)
{
	struct arena* win;
	struct datapair* mod;
	struct glsrc* src;

	win = (void*)(peer->chip);
	mod = win->gl_solid;
	say("@aidgeom_start\n");

//--------------------3d-------------------
	//drawarray.point3d
	src = &mod[point3d].src;
	src->method = 'v';
	src->geometry = 1;

	src->vs = simplevert;
	src->fs = simplefrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_enq = 1;


	//drawelement.line3d
	src = &mod[line3d].src;
	src->method = 'i';
	src->geometry = 2;

	src->vs = simplevert;
	src->fs = simplefrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*2;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x22;
	src->ibuf_enq = 1;


	//drawelement.trigon3d
	src = &mod[trigon3d].src;
	src->method = 'i';
	src->geometry = 3;

	src->vs = prettyvert;
	src->fs = prettyfrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x1000000;
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




//----------------------2d--------------------
	//drawarray.point2d
	src = &mod[point2d].src;
	src->method = 'v';
	src->geometry = 1;

	src->vs = easy2d_vert;
	src->fs = easy2d_frag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_enq = 1;


	//drawelement.line2d
	src = &mod[line2d].src;
	src->method = 'i';
	src->geometry = 2;

	src->vs = easy2d_vert;
	src->fs = easy2d_frag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*2;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x22;
	src->ibuf_enq = 1;


	//drawelement.trigon2d
	src = &mod[trigon2d].src;
	src->method = 'i';
	src->geometry = 3;

	src->vs = glsl2d_vert;
	src->fs = glsl2d_frag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*3*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;




	mod = win->gl_opaque;

//----------------------2d--------------------
	//opaque3d
	src = &mod[opaquetrigon3d].src;
	src->method = 'i';
	src->geometry = 3;

	src->vs = opaquevert;
	src->fs = opaquefrag;
	src->shader_enq = 1;

	src->vbuf_len = 0x1000000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*4*3;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_444;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;


	//opaque2d
	src = &mod[opaquetrigon2d].src;
	src->method = 'i';
	src->geometry = 3;

	src->vs = opaque2d_vert;
	src->fs = opaque2d_frag;
	src->shader_enq = 1;

	src->vbuf_len = 0x100000;
	src->vbuf = memorycreate(src->vbuf_len);
	src->vbuf_w = 4*4*2;
	src->vbuf_h = (src->vbuf_len) / (src->vbuf_w);
	src->vbuf_fmt = vbuffmt_44;
	src->vbuf_enq = 1;

	src->ibuf_len = 0x100000;
	src->ibuf = memorycreate(src->ibuf_len);
	src->ibuf_w = 2*3;
	src->ibuf_h = (src->ibuf_len) / (src->ibuf_w);
	src->ibuf_fmt = 0x222;
	src->ibuf_enq = 1;
}
static void aidgeom_delete(struct actor* act)
{
	if(0 == act)return;
	if(0 == act->buf){
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void aidgeom_create(struct actor* act, void* str)
{
	if(0 == act)return;

}




void aidgeom_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('a', 'i', 'd', 'g', 'e', 'o', 'm', 0);

	p->oncreate = (void*)aidgeom_create;
	p->ondelete = (void*)aidgeom_delete;
	p->onstart  = (void*)aidgeom_start;
	p->onstop   = (void*)aidgeom_stop;
	p->oncread  = (void*)aidgeom_cread;
	p->oncwrite = (void*)aidgeom_cwrite;
	p->onsread  = (void*)aidgeom_sread;
	p->onswrite = (void*)aidgeom_swrite;
}
