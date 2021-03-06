#include "naming.h"
#include "struct.h"
#ifndef _BIGNODE_H
#define _BIGNODE_H

#ifdef __cplusplus
extern "C" {
#endif




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

	u32 type;
	u32 flag;

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
	u32 dsttype;
	u32 dstflag;
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
	u32 srctype;
	u32 srcflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};
struct foot{
	u64 type;
}__attribute__((packed));
typedef struct relation _rel;




struct item
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 hfmt;
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
		int (*oncreate)(struct item* node, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onsearch)(struct item* node, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(struct item* node, void* buf);
		char padding3[8];
	};
	union{
		int (*onlinkup)(struct item* node,void* foot, void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(struct item* node,void* foot, void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*take_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*take_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, void* arg,u32 cmd, void* buf,u32 len);
		int (*ontaking)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		int (*take)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*give_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*give_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, void* arg,u32 cmd, void* buf,u32 len);
		int (*ongiving)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		int (*give)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		char padding7[8];
	};

	//[80,ff]: public
	u8 public_data[0x80];

	//[100,1ff]: private
	union{
		int   priv_fd;
		void* priv_ptr;
		u8    priv_data[0x100];
	};
}__attribute__((packed));
typedef struct item _obj;




struct sysobj
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;

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
		int (*oncreate)(struct item* node, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onsearch)(struct item* node, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(struct item* node, void* buf);
		char padding3[8];
	};
	union{
		int (*onlinkup)(struct item* node,void* foot, void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(struct item* node,void* foot, void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[80,9f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[a0,bf]: memory
	union{
		u64 addr0;
		void* buf0;
	};
	union{
		u64 addr1;
		void* buf1;
	};
	union{
		u64 addr2;
		void* buf2;
	};
	union{
		u64 addr3;
		void* buf3;
	};

	//[0xc0,0xff]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x100,0x1ff]
	u8 data[0x100];
}__attribute__((packed));
typedef struct sysobj _sys;




struct artery
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

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
		int (*oncreate)(struct item* node, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onsearch)(struct item* node, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(struct item* node, void* buf);
		char padding3[8];
	};
	union{
		int (*onlinkup)(struct item* node,void* foot, void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(struct item* node,void* foot, void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[80,9f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[a0,bf]: memory
	union{
		u64 addr0;
		u64 data0;
		void* buf0;
	};
	union{
		u64 addr1;
		u64 data1;
		void* buf1;
	};
	union{
		u64 len;
		u64 addr2;
		u64 data2;
		void* buf2;
	};
	union{
		u64 addr3;
		u64 data3;
		void* buf3;
	};

	//[0xc0,0xff]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x100,0x1ff]
	u8 data[0x100];
}__attribute__((packed));
typedef struct artery _art;




struct supply
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 fmt;
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
		int (*oncreate)(void* entity, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* entity);
		char padding1[8];
	};
	union{
		int (*onsearch)(void* entity, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(void* entity, void* buf);
		char padding3[8];
	};
	union{
		int (*onlinkup)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct supply* sup,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct supply* sup,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[80,9f]: fd
	union{
		u64 padd0;
		void*  aqref;	//audio.aq
		u64   xlibfd;	//2d.xlib
		void*   hwnd;	//2d.winapi
		void* sdlwnd;	//2d.sdl
		void*  glwnd;	//3d.glfw
		u32      fbo;	//3d.fbo
		void* mtkwnd;	//3d.metal
	};
	union{
		u64 padd1;
		void*  aqctx;	//audio.aq
		void* xlibgc;	//2d.xlib
		void*    hdc;	//2d.winapi
		void* sdlren;	//2d.sdl
		u64   gltime;	//3d.glfw
		void* mtkview;	//3d.metal
	};
	union{
		u64 padd2;
		int    aqenq;	//audio.aq
		void* ximage;	//2d.xlib
		void*  binfo;	//2d.winapi
		void* sdltex;	//2d.sdl
		void* glevto;	//3d.glfw
		u32      rbo;	//3d.depth/stencil
		u32      dep;	//3d.depth/stencil
	};
	union{
		u64 padd3;
		int    aqdeq;	//audio.aq
		void* spsave;	//2d.*
		void* glsave;	//3d.glfw
		u32     tex0;	//3d.color
		u32*     tex;	//3d.gbuffer
	};

	//[a0,bf]: memory
	union{
		u64 data0;
		void* buf0;
		void* perwnd;
		struct pcmdata*  pcmeasy_mic;
		struct pcmdata** pcmfull_mic;
		struct dx11data*  dxeasy_camera;
		struct dx11data** dxfull_camera;
		struct gl41data*  gleasy_camera;
		struct gl41data** glfull_camera;
		u64* gl41cmdq_data;
		struct mt20data*  mteasy_camera;
		struct mt20data** mtfull_camera;
	};
	union{
		u64 data1;
		void* buf1;
		struct pcmdata*  pcmeasy_wall;
		struct pcmdata** pcmfull_wall;
		struct dx11data*  dxeasy_light;
		struct dx11data** dxfull_light;
		struct gl41data*  gleasy_light;
		struct gl41data** glfull_light;
		u64* gl41cmdq_code;
		struct mt20data*  mteasy_light;
		struct mt20data** mtfull_light;
	};
	union{
		u64 data2;
		void* buf2;
		void* rgbaalloc;
		void* depthbuf;
		void* htmlctx;	//html
		int jsonlen;	//json
		int textlen;	//text
		int rgbalen;	//rgba
		struct pcmdata*  pcmeasy_data;
		struct pcmdata** pcmfull_data;
		struct dx11data*  dxeasy_solid;
		struct dx11data** dxfull_solid;
		struct gl41data*  gleasy_solid;
		struct gl41data** glfull_solid;
		struct mt20data*  mteasy_solid;
		struct mt20data** mtfull_solid;
	};
	union{
		u64 data3;
		void* buf3;
		void* htmlbuf;	//html
		void* jsonbuf;	//json
		void* textbuf;	//text
		void* rgbabuf;	//rgba
		void* colorbuf;
		struct pcmdata*  pcmeasy_what;
		struct pcmdata** pcmfull_what;
		struct dx11data*  dxeasy_opaque;
		struct dx11data** dxfull_opaque;
		struct gl41data*  gleasy_opaque;
		struct gl41data** glfull_opaque;
		struct mt20data*  mteasy_opaque;
		struct mt20data** mtfull_opaque;
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int fourth;

	//[d0,df]
	int fbwidth;	//=stride=pitch=byte_per_line
	int fbheight;
	int fbdepth;
	int fbfourth;

	//[e0,ef]
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

	//[f0,ff]
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

	//[100,1ff]
	union{
		int   priv_fd;
		void* priv_ptr;
		u8    priv_data[0x100];
	};
}__attribute__((packed));
typedef struct supply _sup;


struct entity
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 fmt;
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
		int (*oncreate)(void* entity, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* entity);
		char padding1[8];
	};
	union{
		int (*onsearch)(void* entity, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(void* entity, void* buf);
		char padding3[8];
	};
	union{
		int (*onlinkup)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[80,9f]: fd
	union{
		u64 fd0;
		u64 fd;
		void* win;
		int vlen;
	};
	union{
		u64 fd1;
		u64 dc;
		u64 gc;
		void* er;
		void* mtkview;
		int ilen;
	};
	union{
		u64 fd2;
		int nlen;
	};
	union{
		u64 fd3;
		int wlen;
	};

	//[a0,bf]: buf
	union{
		u64 data0;
		float fdata0;
		double ddata0;
		void* buf0;
		void** dataasset;
	
		struct pcmdata*  pcmeasy_mic;
		struct pcmdata** pcmfull_mic;
		struct dx11data*  dxeasy_camera;
		struct dx11data** dxfull_camera;
		struct gl41data*  gleasy_camera;
		struct gl41data** glfull_camera;
		u64* gl41cmdq_data;
		struct mt20data*  mteasy_camera;
		struct mt20data** mtfull_camera;
	};
	union{
		u64 data1;
		float fdata1;
		double ddata1;
		void* buf1;
		void** timespace;

		struct pcmdata*  pcmeasy_wall;
		struct pcmdata** pcmfull_wall;
		struct dx11data*  dxeasy_light;
		struct dx11data** dxfull_light;
		struct gl41data*  gleasy_light;
		struct gl41data** glfull_light;
		u64* gl41cmdq_code;
		struct mt20data*  mteasy_light;
		struct mt20data** mtfull_light;
	};
	union{
		u64 data2;
		float fdata2;
		double ddata2;

		int jsonlen;	//json
		int textlen;	//text
		int rgbalen;	//rgba

		void* buf2;
		void* htmlctx;	//html
		void* depthbuf;

		struct pcmdata*  pcmeasy_data;
		struct pcmdata** pcmfull_data;
		struct dx11data*  dxeasy_solid;
		struct dx11data** dxfull_solid;
		struct gl41data*  gleasy_solid;
		struct gl41data** glfull_solid;
		struct mt20data*  mteasy_solid;
		struct mt20data** mtfull_solid;
	};
	union{
		u64 data3;
		float fdata3;
		double ddata3;

		void* buf3;
		void* htmlbuf;	//html
		void* jsonbuf;	//json
		void* textbuf;	//text
		void* rgbabuf;	//rgba
		void* colorbuf;

		struct pcmdata*  pcmeasy_what;
		struct pcmdata** pcmfull_what;
		struct dx11data*  dxeasy_opaque;
		struct dx11data** dxfull_opaque;
		struct gl41data*  gleasy_opaque;
		struct gl41data** glfull_opaque;
		struct mt20data*  mteasy_opaque;
		struct mt20data** mtfull_opaque;
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int fourth;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbfourth;

	//[e0,ef]
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

	//[f0,ff]
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

	//[100,1ff]
	union{
		int   priv_fd;
		void* priv_ptr;
		u8    priv_data[0x100];
	};
}__attribute__((packed));
typedef struct entity _ent;




int entity_take(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entity_give(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entitydiscon(struct halfrel* self, struct halfrel* peer);
int entitylinkup(struct halfrel* self, struct halfrel* peer);
int entitydelete(void*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supply_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supply_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supplydiscon(struct halfrel* self, struct halfrel* peer);
int supplylinkup(struct halfrel* self, struct halfrel* peer);
int supplydelete(struct supply*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int artery_take(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int artery_give(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int arterydiscon(struct halfrel* self, struct halfrel* peer);
int arterylinkup(struct halfrel* self, struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int system_take(_sys* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int system_give(_sys* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int systemdiscon(struct halfrel* self, struct halfrel* peer);
int systemlinkup(struct halfrel* self, struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driver_take(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driver_give(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driverdiscon(struct halfrel* self, struct halfrel* peer);
int driverlinkup(struct halfrel* self, struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int device_take(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int device_give(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int devicediscon(struct halfrel* self, struct halfrel* peer);
int devicelinkup(struct halfrel* self, struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int bootup_take(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int bootup_give(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int bootupdiscon(struct halfrel* self, struct halfrel* peer);
int bootuplinkup(struct halfrel* self, struct halfrel* peer);
int bootupdelete(void*);
void* bootupcreate(u64 type, void* addr, int argc, u8** argv);
//
int origin_take(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origin_give(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origindiscon(struct halfrel* self, struct halfrel* peer);
int originlinkup(struct halfrel* self, struct halfrel* peer);
int origindelete(void*);
void* origincreate(u64 type, void* addr, int argc, u8** argv);
//
int relationdiscon(struct halfrel* self, struct halfrel* peer);
int relationlinkup(struct halfrel* self, struct halfrel* peer);
int relationsearch(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationmodify(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationdelete(_rel* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
int take_data_from_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int give_data_into_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int take_data_from_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int give_data_into_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);




#ifdef __cplusplus
}
#endif

#endif
