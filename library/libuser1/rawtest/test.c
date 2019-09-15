#include "libuser.h"
#define s30 0.5
#define s45 0.7071067811865476
#define s60 0.8660254037844386
void fixmatrix(mat4 m, struct fstyle* sty);




static char vshader[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";
static char fshader[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";
static float vbuf[6*8] = {
	-1.0,-1.0,-1.0,		0.0, 0.0, 0.0,
	 1.0,-1.0,-1.0,		1.0, 0.0, 0.0,
	-1.0, 1.0,-1.0,		0.0, 1.0, 0.0,
	 1.0, 1.0,-1.0,		1.0, 1.0, 0.0,

	-1.0,-1.0, 1.0,		0.0, 0.0, 1.0,
	 1.0,-1.0, 1.0,		1.0, 0.0, 1.0,
	-1.0, 1.0, 1.0,		0.0, 1.0, 1.0,
	 1.0, 1.0, 1.0,		1.0, 1.0, 1.0
};
static u16 ibuf[3*2*6] = {
	2, 0, 6,	0, 4, 6,
	3, 1, 7,	1, 5, 7,

	0, 1, 5,	0, 5, 4,
	6, 3, 2,	6, 7, 3,

	0, 2, 1,	1, 2, 3,
	4, 5, 7,	7, 6, 4
};
static struct fstyle sty = {
	.vl = {-1.0, 0.0, 0.0,-1.0},
	.vr = { 1.0, 0.0, 0.0, 1.0},
	.vn = { 0.0,-s60, s30, 1.0},
	.vf = { 0.0, s60,-s30, 10000000.0},
	.vb = { 0.0,-s30,-s60,-1.0},
	.vt = { 0.0, s30, s60, 1.0},
	.vq = { 0.0, 0.0, 0.0, 0.0},
	.vc = { 0.0, -s60*4, s30*4, 0.0},
};
static mat4 cammvp;




int test_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int test_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int test_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int test_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int test_search(struct actor* act)
{
	return 0;
}
int test_modify(struct actor* act)
{
	return 0;
}
int test_delete(struct actor* act)
{
	return 0;
}
int test_create(struct actor* act)
{
	int j;
	u8* buf;
	struct datapair* pair;
	struct glsrc* src;

	act->buf = memorycreate(0x1000, 0);
	if(0 == act->buf)return 0;

	buf = act->buf;
	for(j=0;j<0x1000;j++)buf[j] = 0;

	pair = act->buf;
	src = &pair->src;

	src->vs = vshader;
	src->fs = fshader;
	src->shader_enq = 1;


	fixmatrix(cammvp, &sty);
	mat4_transpose(cammvp);
	src->arg_data[0] = cammvp;

	src->vbuf = vbuf;
	src->vbuf_len = 4*6*8;
	src->vbuf_enq = 1;

	src->ibuf = ibuf;
	src->ibuf_len = 2*3*2*6;
	src->ibuf_enq = 1;

	return 0;
}




void test_free()
{
}
void test_init(void* addr)
{
}
