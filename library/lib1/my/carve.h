#include "const/def.h"
#include "data/node-outer.h"
//
#ifdef __ANDROID__
	#define GLSL_VERSION "#version 320 es\n"
	#define GLSL_PRECISION "precision highp float;\n"
#elif __APPLE__
	#define GLSL_VERSION "#version 410 core\n"
	#define GLSL_PRECISION "precision mediump float;\n"
#else
	#define GLSL_VERSION "#version 430 core\n"
	#define GLSL_PRECISION "precision mediump float;\n"
#endif

//solid
#define point3d  0
#define line3d   1
#define trigon3d 2
#define voxel3d  3
#define solidaid_max 4
//opaque
#define font3d0 0
#define font3d1 1
#define font3d2 2
#define font3d3 3
#define opaquepoint3d  4
#define opaqueline3d   5
#define opaquetrigon3d 6
#define opaquevoxel3d  7
#define opaqueaid_max 8
//
#define vbuffmt_3      0x3
#define vbuffmt_33     0x33
#define vbuffmt_333    0x333
#define vbuffmt_3333   0x3333
#define vbuffmt_33333  0x33333
#define vbuffmt_333333 0x333333
#define vbuffmt_4      0x4
#define vbuffmt_44     0x44
#define vbuffmt_444    0x444
#define vbuffmt_4444   0x4444
#define vbuffmt_44444  0x44444
#define vbuffmt_444444 0x444444
//
#define _pbrcolor_ 10
#define _dirlight_ 20
#define _spotlight_ 21
#define _projector_ 22
#define _pointlight_ 23
#define _imagelight_ 24




struct shader{
    void* vs;
    void* ts;
    void* gs;
    void* fs;
};
struct uniform{
	void* buf;
	int len;
};
struct texture{
	union{
		void* data;
		u32 glfd;
	};
	u32 w;
	u32 h;
	u32 fmt;
};
struct vertex{
	void* vbuf;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;

	void* ibuf;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;

	//[e8,eb]
	u8 geometry;	//0=default, 1=point, 2=line, 3=trigon, 4=volume
	u8 fill;		//0=default, 1=point, 2=line, 3=trigon, 4=volume
	u8 opaque;		//0=solid, n=opaque
	u8 flag;		//4b align
};
struct drawqueue{
	u32 type;		//clean, +tex, +vtx, tex0=tex[0] ...
	u32 typesub;
	u32 dst;
	u32 src;
};
struct per3d{
    //cpu data
    struct shader* sha;
	struct uniform* uni;
    struct texture* tex;
    struct vertex* vtx;
	struct drawqueue* queue;

    //dx
	void* dx_texture[7];
	void* dx_resource[7];
	void* dx_sampler[7];
	void* dx_constant;
	void* dx_vbuf;
	void* dx_ibuf;
	void* dx_layout;

    //gl
    u64* gl_sbo;
    u64* gl_ubo;
    u64* gl_tbo;
    u64* gl_vbo;
    u64* gl_fbo;
    u64* gl_vao;

	//mt
	void* mt_fbo;
	void* mt_pipeline;
	void* mt_depthstencil;
	void* mt_blend;
	void* mt_shader;
	void* mt_vert;
	void* mt_frag;
	void* mt_texture[7];
	void* mt_sampler[7];
	void* mt_uniform;
	void* mt_vbuf;
	void* mt_ibuf;
};




struct mysrc
{
	void* owner;
	u32 type;

	//shader
	void* vs;
	void* gs;
	void* fs;

	//texture in/out
	struct texture tex[8];

	//uniform
	struct uniform uni[2];

	//vertex
	struct vertex vtx[2];

	//enqueue
	u8 target_enq;
	u8 shader_enq;
	u8 tex_enq[8];
	u8 uni_enq[2];
	u8 vbuf_enq;
	u8 ibuf_enq;
};
struct my3ddata
{
	//[000,1ff]
	struct mysrc src;
	u8 ipadd[0x200 - sizeof(struct mysrc)];

	//[200,3ff]
	u8 opadd[0x200];
};




struct dxdst
{
	//framebuffer
	void* owner;
	void* fbo;

	//shader
	void* vsblob;
	void* psblob;
	void* vsprog;
	void* psprog;

	//texture
	void* texture[7];
	void* resource[7];
	void* sampler[7];

	//constant
	void* constant;

	//vertex
	void* vbuf;
	void* ibuf;
	void* layout;

	//dequeue
	u8 target_deq;
	u8 shader_deq;
	u8 tex_deq[7];
	u8 arg_deq;
	u8 vbo_deq;
	u8 ibo_deq;
};
struct dx11data
{
	//[000,1ff]
	struct mysrc src;
	u8 ipadd[0x200 - sizeof(struct mysrc)];

	//[200,3ff]
	struct dxdst dst;
	u8 opadd[0x200 - sizeof(struct dxdst)];
};




struct gldst
{
/*
	//crop
	u32 x0;
	u32 y0;
	u32 dx;
	u32 dy;

	//clear
	u32 clear;		//color,depth
	u32 color;
	u32 depth;
*/
	//shader
	int lighttype;

	//uniform
	struct arg{
		char* name;
		void* data;
		u32 fmt;
	}arg[8];

	//texture
	char* texname[8];

	//android will distroy context, need check and recreate
	u32 ctxage;

	//
	u8 gpudata_head[0];
	u32 fbo;
	u32 rbo;
	u32 ppll_atomic;	//per pixel link list: counter
	u32 ppll_head;		//per pixel link list: head
	u32 ppll_data;		//per pixel link list: data

	u32 shader;
	u32 ubo;
	u32 tex[8];
	u32 vbo;
	u32 ibo;
	u32 vao;

	//dequeue
	u8 target_deq;
	u8 shader_deq;
	u8 tex_deq[8];
	u8 vbo_deq;
	u8 ibo_deq;
	u8 gpudata_tail[0];
};
struct gl41data
{
	//[000,1ff]
	struct mysrc src;
	u8 ipadd[0x200 - sizeof(struct mysrc)];

	//[200,3ff]
	struct gldst dst;
	u8 opadd[0x200 - sizeof(struct gldst)];
};




struct vk12dst
{
	void* owner;
};
struct vk12data
{
	//[000,1ff]
	struct mysrc src;
	u8 ipadd[0x200 - sizeof(struct mysrc)];

	//[200,3ff]
	struct vk12dst dst;
	u8 opadd[0x200 - sizeof(struct gldst)];
};




struct mtdst
{
	void* owner;

	//framebuffer
	void* fbo;
	void* pipeline;
	void* depthstencil;
	void* blend;

	//shader
	void* shader;
	void* vert;
	void* frag;

	//texture
	void* texture[7];
	void* sampler[7];

	//constant
	void* uniform;

	//vertex
	void* vbuf;
	void* ibuf;

	//dequeue
	u8 target_deq;
	u8 shader_deq;
	u8 tex_deq[7];
	u8 arg_deq;
	u8 vbo_deq;
	u8 ibo_deq;
};
struct mt20data
{
	//[000,1ff]
	struct mysrc src;
	u8 ipadd[0x200 - sizeof(struct mysrc)];

	//[200,3ff]
	struct mtdst dst;
	u8 opadd[0x200 - sizeof(struct mtdst)];
};




//directx
void dx11data_before(_obj* wnd);
void dx11data_after(_obj* wnd);
void dx11data_nocam(_obj* wnd);
void dx11data_01cam(_obj* wnd);
int dx11data_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);

void dx11solid_rect(_obj* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
/*
void dx11point(              _obj* ctx, u32 rgb, vec3 vc);

void dx11line(               _obj* ctx, u32 rgb, vec3 va, vec3 vb);
void dx11line_rect(          _obj* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void dx11solid_triangle(     _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void dx11solid_rect(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void dx11solid_prism4(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11solid_cylinder(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void dx11opaque_rect(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void dx11opaque_prism4(      _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void dx11ascii(              _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void dx11ascii_center(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void dx11unicode(            _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void dx11unicode_center(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void dx11utf8(               _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void dx11utf8_center(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void dx11string(              _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void dx11string_center(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void dx11decimal(             _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void dx11hexadecimal(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
*/




//metal
void mt20data_before(_obj* wnd);
void mt20data_after(_obj* wnd);
int mt20data_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);




//opengl
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_nocam(_obj* wnd);
void gl41data_01cam(_obj* wnd);
int gl41data_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);

void gl41point(              _obj* ctx, u32 rgb, vec3 vc);
void gl41point_bezier(       _obj* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void gl41point_triangle(     _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41point_rect(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41point_circle(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41point_cone(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_cask(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_cylinder(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_dodecahedron( _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41point_icosahedron(  _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41point_sphere(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void gl41line(               _obj* ctx, u32 rgb, vec3 va, vec3 vb);
void gl41line_shorter(       _obj* ctx, u32 rgb, vec3 va, vec3 vb);
void gl41line_arrow(         _obj* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vn);
void gl41line_bezier(        _obj* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void gl41line_spring(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_yshape(        _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41line_triangle(      _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41line_rect(          _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_rectround(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_rectselect(    _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_hexagon(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_circle(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_gear(          _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth);
void gl41line_rotategear(    _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth, float a);
void gl41line_cone(          _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_prism4(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_cylinder(      _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_dodecahedron(  _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_icosahedron(   _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_sphere(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void gl41solid_triangle(     _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41solid_rect(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41solid_circle(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41solid_cone(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41solid_prism4(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_cask(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_cylinder(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_gear(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth);
void gl41solid_rotategear(   _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth, float a);
void gl41solid_dodecahedron( _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_icosahedron(  _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_sphere(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_propeller(    _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

void gl41opaque_triangle(    _obj* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41opaque_rect(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41opaque_circle(      _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41opaque_cone(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41opaque_prism4(      _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_cask(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_cylinder(    _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_dodecahedron(_obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_icosahedron( _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_sphere(      _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_propeller(   _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

void gl41ascii(              _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void gl41ascii_center(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void gl41unicode(            _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void gl41unicode_center(     _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void gl41utf8(               _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void gl41utf8_center(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void gl41decimal(             _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41hexadecimal(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41hex8_center(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41string(              _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41string_center(       _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41text(                _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41text_reverse(        _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41float(               _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void gl41double(              _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);

void gl41axis(                _obj* ctx);
void gl41frustum(             _obj* ctx, struct fstyle* sty);
void gl41boundingvolume(      _obj* ctx, u32 rgb, struct fstyle* sty, u32 flag);




void mt20data_before(_obj* wnd);
void mt20data_after(_obj* wnd);
void mt20data_nocam(_obj* wnd);
void mt20data_01cam(_obj* wnd);
//void mt20solid_rect(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
