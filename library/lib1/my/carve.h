#include "const/def.h"

#ifndef _MYCARVE_H
#define _MYCARVE_H

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
//camtype
#define _wvp_proj_ 10
#define _equi_rect_proj_ 20
//lighttype
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
/*
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
void carvetrigonindex_triangle_v3n3x3(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0,vec3 v1,vec3 v2);
void carvetrigonindex_rect_v3n3x3(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_circle_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_cask_v3n3x3(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_prism4_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_sphere_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
*/



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




//directx11
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
struct dx11easy{
	struct dx11data* camera;
	struct dx11data* light;
	struct dx11data* solid;
	struct dx11data* opaque;
};
struct dx11world{
	struct dx11data** camera;
	struct dx11data** light;
	struct dx11data** solid;
	struct dx11data** opaque;
};
struct dx11list{
	struct dx11world world[2];
	void* hwnd;
	void* spsave;
};
struct dx11cmdq{
	u64* data;
	u64* code;
};




//opengl
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
	int camtype;
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
	u32 glctxage;

	//--------area to clean when ctx gone--------
	u8 gpudata_head[0];

	//render target
	u32 fbo;
	u32 rbo;
	u32 ppll_atomic;	//per pixel link list: counter
	u32 ppll_head;		//per pixel link list: head
	u32 ppll_data;		//per pixel link list: data

	//shader
	u32 shader;

	//data
	u32 ubo;

	//vertex
	u32 vbo;
	u32 ibo;
	u32 vao;

	//texture
	u32 tex[8];

	//dequeue
	u8 target_deq;
	u8 shader_deq;
	u8 tex_deq[8];
	u8 vbo_deq;
	u8 ibo_deq;

	//--------area to clean when ctx gone--------
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

//1
struct gl41easy{
	struct gl41data*  camera;
	struct gl41data*  light;
	struct gl41data*  solid;
	struct gl41data*  opaque;
};

//2
struct gl41world{
	struct gl41data** camera;
	struct gl41data** light;
	struct gl41data** solid;
	struct gl41data** opaque;
};
struct gl41list{
	struct gl41world world[2];
	u32 rendermode;

	//context
	void* glwnd;
	u32 glctxage;
	u64 gltime;

	//event
	void* glevto;
	void* glsave;
};
//3
struct gl41cmdq{
	u64* data;
	u64* code;
};




//vulkan
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




//metal
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
struct mt20easy{
	struct mt20data* camera;
	struct mt20data* light;
	struct mt20data* solid;
	struct mt20data* opaque;
};
struct mt20world{
	struct mt20data** camera;
	struct mt20data** light;
	struct mt20data** solid;
	struct mt20data** opaque;
};
struct mt20list{
	struct mt20world world[2];

	void* delegate;
	void* view;
};
struct mt20cmdq{
	u64* data;
	u64* code;
};








#endif
