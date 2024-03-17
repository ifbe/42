#ifndef _BIGNODE_H
#define _BIGNODE_H

//
#include "const/def.h"
#include "const/naming.h"
//
#include "my/audio.h"
#include "my/draw.h"
#include "my/carve.h"

#ifdef __cplusplus
extern "C" {
#endif



//--------------------------------relation--------------------------------
struct halfrel
{
	union{
		u64 chip;
		void* pchip;
	};
	union{
		u64 foot;
		void* pfoot;
	};

	u32 nodetype;
	u32 foottype;

	u32 prev;
	u32 next;
};
typedef struct halfrel _syn;

struct relation
{
	//[0x00,0x1f]
	union{
		u8 dst[0];
		u64 dstchip;
		void* pdstchip;
	};
	union{
		u64 dstfoot;
		void* pdstfoot;
	};
	u32 dstnodetype;
	u32 dstfoottype;
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	union{
		u8 src[0];
		u64 srcchip;
		void* psrcchip;
	};
	union{
		u64 srcfoot;
		void* psrcfoot;
	};
	u32 srcnodetype;
	u32 srcfoottype;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};
typedef struct relation _rel;




//--------------------------------public--------------------------------
struct sock_info{
	u8 self[0x20];
	u8 peer[0x20];
	void* parent;
	void* child;
	u64 fd;
};

struct file_info{
	u8 name[0x78];
	u64 fd;
};

struct whdf_xyzw{
	//[00,0f]
	int width;
	int height;
	int depth;
	int fourth;

	//[10,1f]
	int fbwidth;	//=stride=pitch=byte_per_line
	int fbheight;
	int fbdepth;
	int fbfourth;

	//[20,2f]
	union{
		int ix0;
		f32 fx0;
	};
	union{
		int iy0;
		f32 fy0;
	};
	union{
		int iz0;
		f32 fz0;
	};
	union{
		int iw0;
		f32 fw0;
	};

	//[30,3f]
	union{
		int ixn;
		f32 fxn;
	};
	union{
		int iyn;
		f32 fyn;
	};
	union{
		int izn;
		f32 fzn;
	};
	union{
		int iwn;
		f32 fwn;
	};
};
struct eight_four{
	float a0[4];
	float a1[4];
	float a2[4];
	float a3[4];
	float a4[4];
	float a5[4];
	float a6[4];
	float a7[4];
};




//--------------------------------private--------------------------------
//normal
struct listu32{
	u32 data0;
	u32 data1;
	u32 data2;
	u32 data3;
	u32 data4;
	u32 data5;
	u32 data6;
	u32 data7;
};
struct listu64{
	u64 data0;
	u64 data1;
	u64 data2;
	u64 data3;
	u64 data4;
	u64 data5;
	u64 data6;
	u64 data7;
};
struct listfloat{
	float f0;
	float f1;
	float f2;
	float f3;
	float f4;
	float f5;
	float f6;
	float f7;
};
struct listdouble{
	float d0;
	float d1;
	float d2;
	float d3;
	float d4;
	float d5;
	float d6;
	float d7;
};
struct listptr{
	void* buf0;
	void* buf1;
	void* buf2;
	void* buf3;
	void* buf4;
	void* buf5;
	void* buf6;
	void* buf7;
};
struct buflen{
	void* buf0;
	u64 len0;
	void* buf1;
	u64 len1;
	void* buf2;
	u64 len2;
	void* buf3;
	u64 len3;
};
//text
struct tuitext{
	void* buf;
	void* conv;
};
//html
struct htmltext{
	void* ctx;
	void* buf;
};
//json
struct jsontext{
	void* buf;
	int len;
};
//2d
struct rgbanode{
	void* buf;
	void* depthbuf;
};
struct rgbasdl{
	void* buf;
	void* depthbuf;

	void* sdlwnd;
	void* sdlren;
	void* sdltex;
};
struct rgbaxlib{
	void* buf;
	void* depthbuf;

	u64   xlibfd;
	void* xlibgc;
	void* ximage;

	void* save_stack;
	int save_sp;
};
struct rgbawinapi{
	void* buf;
	void* depthbuf;

	void*   hwnd;
	void*    hdc;
	void*  binfo;

	void* finger;

	void* save_stack;
	int save_sp;
};







//--------------------------------object--------------------------------
/*
struct item_common
{
	//[00,1f]: type
	u64 tier;
	u64 kind;
	u64 type;
	u64 vfmt;

	//[20,3f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[40,7f]: func
	union{
		int (*oncreate)(struct item* node, void* arg, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onreader)(struct item* node,void* foot, p64 arg,int cmd, void* buf,int len);
		char padding2[8];
	};
	union{
		int (*onwriter)(struct item* node,void* foot, p64 arg,int cmd, void* buf,int len);
		char padding3[8];
	};
	union{
		int (*onattach)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondetach)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*take)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*give)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		char padding7[8];
	};

	//[80,ff]: public
	union{
		u8 public_data[0x80];
		struct whdf_xyzw whdf;
		struct sock_info sockinfo;
		struct file_info fileinfo;
	};
}__attribute__((packed));
*/
struct item
{
	//[00,1f]: type
	u64 tier;
	u64 kind;
	u64 type;
	u64 vfmt;

	//[20,3f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[40,7f]: func
	union{
		int (*oncreate)(struct item* node, void* arg, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onreader)(struct item* node,void* foot, p64 arg,int cmd, void* buf,int len);
		char padding2[8];
	};
	union{
		int (*onwriter)(struct item* node,void* foot, p64 arg,int cmd, void* buf,int len);
		char padding3[8];
	};
	union{
		int (*onattach)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondetach)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*take_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*take_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, p64 arg,u32 cmd, void* buf,u32 len);
		int (*ontaking)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		int (*take)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*give_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*give_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, p64 arg,u32 cmd, void* buf,u32 len);
		int (*ongiving)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		int (*give)(struct item* node,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len);
		char padding7[8];
	};

	//[80,ff]: public
	union{
		u8 public_data[0x80];
		struct whdf_xyzw whdf;
		struct sock_info sockinfo;
		struct file_info fileinfo;
	};

	//[100,1ff]: private
	union{
		u8   priv_u8;
		u16   priv_u16;
		u32   priv_u32;
		u64   priv_u64;

		int   priv_fd;
		void* priv_ptr;
		u8    priv_256b[0x100];

		struct listu32 listu32;
		struct listu64 listu64;

		struct listfloat listf32;
		struct listdouble listf64;

		struct listptr listptr;
		struct str buflen;

		struct pcmeasy pcmeasy;
		struct appleaudioqueue appleaq;

		struct tuitext tuitext;
		struct htmltext html;
		struct jsontext json;

		struct rgbanode rgbanode;
		struct rgbasdl rgbasdl;
		struct rgbawinapi rgbawinapi;
		struct rgbaxlib rgbaxlib;

		struct gl41easy gl41easy;
		struct gl41list gl41list;
		struct gl41cmdq gl41cmdq;

		struct dx11list dx11list;

		struct mt20list mt20list;
	};
}__attribute__((packed));
typedef struct item _obj;




//--------------------------------function node--------------------------------
//entity
void* entity_alloc();
void* entity_alloc_fromtype(u64 type);
void* entity_alloc_frompath(u64 type, u8* path);
void* entity_alloc_fromfd(u64 type, int fd);
//
int entity_create(_obj*, void* addr, int argc, u8** argv);
int entity_delete(_obj*);
int entity_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int entity_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int entity_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int entity_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int entity_takeby(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int entity_giveby(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//supply
void* supply_alloc();
void* supply_alloc_fromtype(u64 type);
void* supply_alloc_frompath(u64 type, u8* path);
void* supply_alloc_fromfd(u64 type, int fd);
void* supply_findtype(u64 type);
//
int supply_create(_obj*, void* addr, int argc, u8** argv);
int supply_delete(_obj*);
int supply_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int supply_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int supply_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int supply_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int supply_takeby(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int supply_giveby(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//artery
void* artery_alloc();
void* artery_alloc_fromtype(u64 type);
void* artery_alloc_frompath(u64 type, u8* path);
void* artery_alloc_fromfd(u64 type, int fd);
//
int artery_create(_obj*, void* addr, int argc, u8** argv);
int artery_delete(_obj*);
int artery_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int artery_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int artery_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int artery_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int artery_takeby(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int artery_giveby(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//system
void* system_alloc();
void* system_alloc_fromtype(u64 type);
void* system_alloc_frompath(u64 type, u8* path);
void* system_alloc_fromfd(u64 type, int fd);
//
int system_create(_obj* type, void* addr, int argc, u8** argv);
int system_delete(_obj*);
int system_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int system_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int system_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer);
int system_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer);
int system_takeby(_obj* obj,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int system_giveby(_obj* obj,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//driver
void* driver_alloc();
void* driver_alloc_fromtype(u64 type);
void* driver_alloc_frompath(u64 type, u8* path);
void* driver_alloc_fromfd(u64 type, int fd);
//
int driver_create(_obj* obj, void* addr, int argc, u8** argv);
int driver_delete(_obj*);
int driver_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int driver_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int driver_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer);
int driver_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer);
int driver_takeby(_obj* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int driver_giveby(_obj* dri,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//device
void* device_alloc();
void* device_alloc_fromtype(u64 type);
void* device_alloc_frompath(u64 type, u8* path);
void* device_alloc_fromfd(u64 type, int fd);
//
int device_create(_obj* obj, void* addr, int argc, u8** argv);
int device_delete(_obj*);
int device_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int device_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int device_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int device_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int device_takeby(_obj* dev,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int device_giveby(_obj* dev,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//bootup
void* bootup_alloc();
void* bootup_alloc_fromtype(u64 type);
void* bootup_alloc_frompath(u64 type, u8* path);
void* bootup_alloc_fromfd(u64 type, int fd);
//
int bootup_create(_obj* ent, void* addr, int argc, u8** argv);
int bootup_delete(_obj*);
int bootup_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int bootup_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int bootup_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int bootup_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int bootup_takeby(struct item* wrk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int bootup_giveby(struct item* wrk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

//origin
void* origin_alloc();
void* origin_alloc_fromarg(u64 type, void* func, int argc, u8** argv);
void* origin_alloc_fromtype(u64 type);
void* origin_alloc_frompath(u64 type, u8* path);
void* origin_alloc_fromfd(u64 type, int fd);
//
int origin_create(_obj* ent, void* addr, int argc, u8** argv);
int origin_delete(_obj*);
int origin_reader(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
int origin_writer(_obj* ent,void* foot, p64 arg,int idx, void* buf,int len);
//
int origin_attach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int origin_detach(_obj* obj,void* foot, struct halfrel* self, struct halfrel* peer);
int origin_takeby(struct item* ori,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int origin_giveby(struct item* ori,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);




//--------------------------------function relation--------------------------------
void* relation_alloc();
void* relation_alloc_fromptr(void*,void*,u32,u32,void*,void*,u32,u32);
//
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
int relationdelete(_rel* rel);
//
int relationdetach(struct halfrel* self, struct halfrel* peer);
int relationattach(struct halfrel* self, struct halfrel* peer);
//
int relationsearch(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationmodify(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
//
int reading_data_from_peer(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
int writing_data_into_peer(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
int reading_data_from_them(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
int writing_data_into_them(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
//
int take_data_from_peer_temp_stack(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
int give_data_into_peer_temp_stack(void* chip,int ftype, p64 arg,int idx, void* buf,int len);
int take_data_from_them_temp_stack(void* item,int ftype, p64 arg,int key, void* buf,int len);
int give_data_into_them_temp_stack(void* item,int ftype, p64 arg,int key, void* buf,int len);
//
int take_data_from_peer(void* chip,int ftype, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int give_data_into_peer(void* chip,int ftype, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);
int take_data_from_them(void* item,int ftype, _syn* stack,int sp, p64 arg,int key, void* buf,int len);
int give_data_into_them(void* item,int ftype, _syn* stack,int sp, p64 arg,int key, void* buf,int len);




//--------------------------------function audio--------------------------------
//void playpcm(_obj* ctx, int fmt, void* buf, int len);




//--------------------------------function draw--------------------------------
void drawline(          _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_bezier(   _obj* ctx, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_choose(   _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   _obj* ctx, u32 rgb, int cx, int cy, int r);
void drawline_oval(     _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   _obj* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(_obj* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  _obj* ctx, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  _obj* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawicon_1(        _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void drawopaque_rect(   _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawopaque_circle( _obj* ctx, u32 rgb, int cx, int cy, int r);

void drawascii(         _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawstring(        _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawfloat(         _obj* ctx, u32 rgb, int cx, int cy, float z);
void drawdouble(        _obj* ctx, u32 rgb, int cx, int cy, double z);
void drawhex8(          _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawhex32(         _obj* ctx, u32 rgb, int cx, int cy, u32 data);
void drawhexdump(       _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawhexadecimal(   _obj* ctx, u32 rgb, int cx, int cy, u64 data);
void drawdec8(          _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawdecimal(       _obj* ctx, u32 rgb, int cx, int cy, int data);
void drawunicode(       _obj* ctx, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);

void drawascii_fit(     _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u64);

int drawtext(           _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawtext_reverse(   _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawvt100(          _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);




//--------------------------------function carve--------------------------------
//directx
void dx11data_before(_obj* wnd);
void dx11data_after(_obj* wnd);
void dx11data_nocam(_obj* wnd);
void dx11data_01cam(_obj* wnd);
int dx11data_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

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
int mt20data_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);




//opengl
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_nocam(_obj* wnd);
void gl41data_01cam(_obj* wnd);
int gl41data_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len);

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
void gl41frustum_color(       _obj* ctx, struct fstyle* sty, u32 color);
void gl41boundingvolume(      _obj* ctx, u32 rgb, struct fstyle* sty, u32 flag);




void mt20data_before(_obj* wnd);
void mt20data_after(_obj* wnd);
void mt20data_nocam(_obj* wnd);
void mt20data_01cam(_obj* wnd);
//void mt20solid_rect(         _obj* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);




#ifdef __cplusplus
}
#endif

#endif
