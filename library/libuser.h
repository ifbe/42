typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float vec1[1];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat1[1][1];
typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau PI*2
//
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#ifdef __ANDROID__
	#define GLSL_VERSION "#version 320 es\n"
#else
	#define GLSL_VERSION "#version 410 core\n"
#endif
//
#define _world3d_  hex64('3','d','w','o','r','l','d', 0)
#define _eeworld_  hex64('e','e','w','o','r','l','d', 0)
//
#define _hoffdata_ hex64('h','o','f','f','d','a','t','a')
//dxhelp
#define _dx11data_ hex64('d','x','1','1','d','a','t','a')
#define _dx11coop_ hex64('d','x','1','1','c','o','o','p')
#define _dx11view_ hex64('d','x','1','1','v','i','e','w')

#define _dx11wnd0_ hex64('d','x','1','1','w','n','d','0')
#define _dx11fbod_ hex64('d','x','1','1','f','b','o','d')
#define _dx11fboc_ hex64('d','x','1','1','f','b','o','c')
#define _dx11fbog_ hex64('d','x','1','1','f','b','o','g')
//
#define _gl41data_ hex64('g','l','4','1','d','a','t','a')
#define _gl41coop_ hex64('g','l','4','1','c','o','o','p')
#define _gl41view_ hex64('g','l','4','1','v','i','e','w')

#define _gl41wnd0_ hex64('g','l','4','1','w','n','d','0')
#define _gl41fbod_ hex64('g','l','4','1','f','b','o','d')
#define _gl41fboc_ hex64('g','l','4','1','f','b','o','c')
#define _gl41fbog_ hex64('g','l','4','1','f','b','o','g')
//
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
//tier
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)
//tier=_win_, type:
#define _ahrs_ hex32('a','h','r','s')
#define _bdc_  hex32('b','d','c',0)
#define _step_ hex32('s','t','e','p')
//
#define _mic_ hex32('m','i','c',0)
#define _spk_ hex32('s','p','k',0)
//
#define _cam_ hex32('c','a','m',0)
#define _cap_ hex32('c','a','p',0)
//
#define _wnd_ hex32('w','n','d',0)
#define _tray_ hex32('t','r','a','y')	//tray
#define _std_ hex32('s','t','d', 0)		//cmdline
//
#define _w2w_ hex32('w','2','w', 0)
#define _khala_ hex64('k','h','a','l','a', 0, 0, 0)
#define _loopback_ hex64('l','o','o','p','b', 'a', 'c', 'k')
//tier=_win_, type=_win_, rfmt:
#define _none_ hex32('n','o','n','e')
#define _easy_ hex32('e','a','s','y')
#define _full_ hex32('f','u','l','l')
#define _coop_ hex32('c','o','o','p')
//tier=_win_, type=_ctx_, rfmt:
#define _vbo_ hex32('v','b','o',0)
#define _rgba_ hex32('r','g','b','a')
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _func_ hex32('f','u','n','c')
#define _sch_  hex32('s','c','h',0)
#define _pcb_  hex32('p','c','b',0)
#define _html_ hex32('h','t','m','l')
#define _json_ hex32('j','s','o','n')
#define _xml_  hex32('x','m','l',0)
//tier=_win_, type=_ctx_, rfmt=vbo, vfmt:
#define _1d_ hex32('1','d',0,0)
#define _2d_ hex32('2','d',0,0)
#define _3d_ hex32('3','d',0,0)
#define _ui_ hex32('u','i',0,0)
//tier=_act_, type:
#define _baby_ hex32('b','a','b','y')
#define _test_ hex32('t','e','s','t')
//
#define _orig_ hex32('o','r','i','g')
#define _ORIG_ hex32('O','R','I','G')
#define _copy_ hex32('c','o','p','y')
#define _COPY_ hex32('C','O','P','Y')
//
#define _uart_ hex32('u','a','r','t')
#define _dbg_ hex32('d','b','g',0)
#define _8bit_ hex32('8','b','i','t')
//
#define _bus_  hex32('b','u','s',0)
#define _pin_  hex32('p','i','n',0)
//
#define _aid3d_ hex64('a','i','d','3','d', 0, 0, 0)
#define _mic3d_ hex64('m','i','c','3','d', 0, 0, 0)
#define _cam3d_ hex64('c','a','m','3','d', 0, 0, 0)
#define _lit3d_ hex64('l','i','t','3','d', 0, 0, 0)
#define _bg3d_ hex32('b','g','3','d')
#define _fg3d_ hex32('f','g','3','d')
#define _ui3d_ hex32('u','i','3','d')
#define _xx3d_ hex32('x','x','3','d')
//
#define _bg2d_ hex32('b','g','2','d')
#define _fg2d_ hex32('f','g','2','d')
#define _ui2d_ hex32('u','i','2','d')
#define _xx2d_ hex32('x','x','2','d')
//
#define _TCP_ hex32('T','C','P',0)
#define _tcp_ hex32('t','c','p',0)
#define _UDP_ hex32('U','D','P',0)
#define _udp_ hex32('u','d','p',0)
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
//
#define _drag_ hex32('d','r','a','g')
#define _joy_ hex32('j','o','y',0)
#define _kbd_ hex32('k','b','d',0)
#define _char_ hex32('c','h','a','r')
#define _i2c_ hex32('i','2','c',0)
//
#define touch0 0
#define touch1 1
#define touch2 2
#define touch3 3
#define mousel 10
#define mouser 11
#define gamepad0 12
#define gamepad1 13
#define gamepad2 14
#define gamepad3 15
//
#define joy_mask  0xff00ff
#define joy_event hex32('j', 0 , 0 , 0 )
#define joy_left  hex32('j', 0 ,'l', 0 )
#define joy_right hex32('j', 0 ,'r', 0 )
//
#define joyl_left    0x0001
#define joyl_right   0x0002
#define joyl_down    0x0004
#define joyl_up      0x0008
#define joyl_trigger 0x0010
#define joyl_bumper  0x0020
#define joyl_thumb   0x0040
#define joyl_select  0x0080
//
#define joyr_left    0x0001
#define joyr_right   0x0002
#define joyr_down    0x0004
#define joyr_up      0x0008
#define joyr_trigger 0x0010
#define joyr_bumper  0x0020
#define joyr_thumb   0x0040
#define joyr_start   0x0080
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
#define pcm2      0x2
#define pcm22     0x22
#define pcm222    0x222
#define pcm2222   0x2222
#define pcm22222  0x22222
#define pcm222222 0x222222
//solid
#define point3d  0
#define line3d   1
#define trigon3d 2
#define solidaid_max 4
//opaque
#define opaquepoint3d  0
#define opaqueline3d   1
#define opaquetrigon3d 2
#define font3d0 4
#define font3d1 5
#define font3d2 6
#define font3d3 7
#define opaqueaid_max 8
//#define point2d  3
//#define line2d   4
//#define trigon2d 5
//#define font2d0 6
//#define font2d1 7
//#define font2d2 8
//#define font2d3 9
//#define opaquetrigon2d 1




struct str
{
	int len;
	u8 buf[0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




struct halfrel
{
	u64 chip;
	u64 foot;
	u32 type;
	u32 flag;
};
struct relation
{
	//[0x00,0x1f]
	u64 dstchip;
	u64 dstfoot;
	u32 dsttype;
	u32 dstflag;
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	u64 srcchip;
	u64 srcfoot;
	u32 srctype;
	u32 srcflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};




struct xyzw
{
	u16 x;
	u16 y;
	u16 z;
	u16 id;
};
struct xyzwpair
{
	//touchdown, pointdown, gamepadleft
	u16 x0;
	u16 y0;
	u16 z0;
	u16 w0;

	//touchmove, pointmove, gamepadright
	u16 xn;
	u16 yn;
	u16 zn;
	u16 wn;
};




struct glsrc
{
	//[00,24)shader
	void* vs;
	void* tc;
	void* te;
	void* gs;
	void* fs;
	u8 shader_enq;

	//[88,fc)texture
	char* tex_name[4];
	void* tex_data[4];
	u32 tex_w[4];
	u32 tex_h[4];
	u32 tex_fmt[4];
	u8 tex_enq[4];

	//[24,88)argument
	char* arg_name[8];
	void* arg_data[8];
	u32 arg_fmt[8];

	//[c0,e7]vertex
	void* vbuf;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;
	u8 vbuf_enq;

	void* ibuf;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;
	u8 ibuf_enq;

	//
	int ifirst;
	int icount;

	//[e8,eb]
	u8 method;		//'v'=glDrawArrays, 'i'=glDrawElements
	u8 geometry;	//1=point, 2=line, *=trigon
	u8 target;		//0=rtt, 1=background, 2=geometry, 3=alphatest, 4=transparent, 5=overlay
};
struct gldst
{
	//shader
	u32 shader;
	u8 shader_deq;

	//texture
	u32 tex[4];
	u8 tex_deq[4];

	//vertex
	u32 vbo;
	u8 vbo_deq;
	u32 ibo;
	u8 ibo_deq;

	//
	u32 vao;
};
struct datapair
{
	//[000,1bf]
	struct glsrc src;
	u8 ipadd[0x1c0 - sizeof(struct glsrc)];

	//[1c0,1ff]
	struct gldst dst;
	u8 opadd[0x40 - sizeof(struct gldst)];
};/*
struct glctx
{
	//[00,24)shader
	u32 shader;
	u8 shader_enq;

	//[88,fc)texture
	char* tex_name[4];
	u32 tex[4];
	u32 tex_w[4];
	u32 tex_h[4];
	u32 tex_fmt[4];
	u8 tex_deq[4];

	//[24,88)argument
	char* arg_name[8];
	void* arg_data[8];
	u32 arg_fmt[8];

	//[c0,e7]vertex
	u32 vbo;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;
	u8 vbuf_enq;

	u32 ibo;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;
	u8 ibuf_enq;

	//
	u32 vao;
	int ifirst;
	int icount;

	//[e8,eb]
	u8 method;		//'v'=glDrawArrays, 'i'=glDrawElements
	u8 geometry;	//1=point, 2=line, *=trigon
	u8 target;		//0=rtt, 1=background, 2=geometry, 3=alphatest, 4=transparent, 5=overlay
};*/




struct current{
	float value;
	float ampli;
	float phase;
	float freq;
};
struct circstat{
	//[00, 7f]
	struct current c;
};
struct kalman{
	u8 padd[0x80];
};
struct pidval{
	float kp;
	float ki;
	float kd;
	float kk;

	float tp;	//enow - e_1
	float ti;	//enow
	float td;	//(enow - e_1) - (e_1 - e_2)
	float tt;

	float e_3;
	float e_2;
	float e_1;
	float enow;

	float o_3;
	float o_2;
	float o_1;
	float onow;		//onow = o_1 + kp*tp + ki*ti + kd*kd

	u8 padd[0x40];
};




struct fstyle{
	vec4 vl;	//[00,0f]: left
	vec4 vr;	//[10,1f]: right
	vec4 vn;	//[20,2f]: near
	vec4 vf;	//[30,3f]: far
	vec4 vb;	//[40,4f]: bottom
	vec4 vt;	//[50,5f]: upper
	vec4 vq;	//[60,6f]: info
	vec4 vc;	//[70,7f]: center
};
struct istyle{
	int ul[4];
	int ur[4];
	int un[4];
	int uf[4];
	int ub[4];
	int ut[4];
	int uq[4];
	int uc[4];
};
struct fmotion{
	vec4 j_angular;		//q
	vec4 j_displace;	//v		jerk
	vec4 a_angular;		//q
	vec4 a_displace;	//v		accel
	vec4 v_angular;		//q
	vec4 v_displace;	//v		velocity
	vec4 x_angular;		//q
	vec4 x_displace;	//v		displace
};
struct imotion{
	int ul[4];
	int ur[4];
	int un[4];
	int uf[4];
	int ub[4];
	int ut[4];
	int uq[4];
	int uc[4];
};
struct style
{
	//[00, 7f]: actual, css shape
	union{
		//struct fstyle actualshape;
		//struct fmotion actualmotion;
		struct fstyle f;
		struct fstyle fs;
		struct fstyle fshape;
		struct istyle i;
		struct istyle is;
		struct istyle ishape;
	};

	//[80, ff]: actual, css shape
	union{
		//struct fstyle actualshape;
		//struct fmotion actualmotion;
		struct fmotion fm;
		struct fmotion fmotion;
		struct imotion im;
		struct imotion imotion;
	};

	//[100, 1ff]
	u64 data[16];
/*
	//[80, ff]: expect, motion state
	union{
		struct fstyle expectshape;
		struct fmotion expectmotion;
	};

	//[00, 7f]: origin, css shape
	union{
		struct fstyle originshape;
		struct fmotion originmotion;
	};

	//[80, ff]: target, motion state
	union{
		struct fstyle targetshape;
		struct fmotion targetmotion;
	};

	//[200, 2ff]
	struct kalman kal;
	struct pidval pid;
*/
};




struct item
{
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

	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;
};
struct object
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;

	//[40,5f]: data
	u64 selffd;
	u64 thatfd;
	u64 dc;
	union{
		u64 addr;
		void* buf;
	};

	//[60,7f]: prop
	int width;
	int height;
	int depth;
	int stride;
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[0x80,0xbf]
	u8 self[0x20];
	u8 peer[0x20];

	//[0xc0,0xff]
	u8 data[0x40];
};
struct element
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: data
	union{
		u64 fd;
		void* win;
	};
	union{
		u64 dc;
		u64 gc;
		void* er;
	};
	union{
		u64 len;
		void* hp;
		void* mod;
		void* ximage;
		void* texture;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[60,7f]: prop
	int width;
	int height;
	int depth;
	int stride;
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[80,ff]
	u8 data[0x80];
};
struct arena
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 vfmt;

	//[40,7f]: func
	union{
		int (*oncreate)(void* actor, void* buf);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* actor);
		char padding1[8];
	};
	union{
		int (*onstart)(void* self, void* peer);
		char padding2[8];
	};
	union{
		int (*onstop)(void* self, void* peer);
		char padding3[8];
	};
	union{
		int (*oncread)(void* self, void* peer, void* buf, int len);
		char padding4[8];
	};
	union{
		int (*oncwrite)(void* self, void* peer, void* buf, int len);
		char padding5[8];
	};
	union{
		int (*onsread)(void* self, void* peer, void* buf, int len);
		char padding6[8];
	};
	union{
		int (*onswrite)(void* self, void* peer, void* buf, int len);
		char padding7[8];
	};

	//[80,bf]: data
	union{
		u64 padd0;
		u64 fd;
		void* win;
	};
	union{
		u64 padd1;
		u64 dc;
		u64 gc;
		void* er;
	};
	union{
		u64 padd2;
		u64 len;
		void* ximage;
		void* texture;
	};
	union{
		u64 padd3;
		u64 addr;
		void* buf;
	};
	union{
		u64 padd4;
		u32 fbo;
	};
	union{
		u64 padd5;
		u32 rbo;
	};
	union{
		u64 padd6;
		u32 tex_depth;
	};
	union{
		u64 padd7;
		u32 tex_color;
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int stride;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[e0,ef]
	int x0;
	int y0;
	int z0;
	int w0;

	//[f0,ff]
	int xn;
	int yn;
	int zn;
	int wn;

	//[100,1ff]
	struct xyzwpair touch[8];
	struct xyzwpair mouse[8];
};
struct actor
{
	//[00,1f]: wire
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

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 name0;

	//[40,7f]: func
	union{
		int (*oncreate)(void* actor, void* buf);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* actor);
		char padding1[8];
	};
	union{
		int (*onsearch)(void* actor, void* buf);
		char padding2[8];
	};
	union{
		int (*onmodify)(void* actor, void* buf);
		char padding3[8];
	};
	union{
		int (*onstart)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*onstop)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*onread)(void* self, void* peer, void* buf, int len);
		char padding6[8];
	};
	union{
		int (*onwrite)(void* self, void* peer, void* buf, int len);
		char padding7[8];
	};

	//[80,9f]: buf
	union{
		u64 fd;
		void* win;
		void* ibuf;		//indx buf
		void* gl_camera;
		void* ctx;
	};
	union{
		u64 dc;
		u64 gc;
		void* er;
		void* vbuf;		//vert buf
		void* gl_light;
	};
	union{
		u64 len;
		void* idx;
		void* wbuf;		//wire list
		void* gl_solid;
	};
	union{
		u64 addr;
		void* buf;
		void* nbuf;		//node list
		void* gl_opaque;
	};

	//[a0,bf]: len
	union{
		u64 data0;
		int ilen;
	};
	union{
		u64 data1;
		int vlen;
	};
	union{
		u64 data2;
		int wlen;
	};
	union{
		u64 data3;
		int nlen;
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int stride;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[e0,ef]
	int x0;
	int y0;
	int z0;
	int w0;

	//[f0,ff]
	int xn;
	int yn;
	int zn;
	int wn;

	//[100,1ff]
	struct fstyle target;
	struct fstyle camera;
};


void* defaultstyle_vbo2d();


//----------------------------------1d------------------------------------
void gentui_rect(  struct actor* win, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  struct actor* win, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   struct actor* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  struct actor* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(struct actor* win, u32 rgb, int cx, int cy, int data);




//----------------------------------2d------------------------------------
void drawaxis(          struct actor* win);
void select_2d(         struct actor* win, u32 rgb, struct fstyle* sty, u32 flag);

void drawline(          struct actor* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    struct actor* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawbezier(        struct actor* win, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( struct actor* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     struct actor* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  struct actor* win, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   struct actor* win, u32 rgb, int cx, int cy, int r);
void drawline_oval(     struct actor* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   struct actor* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(struct actor* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    struct actor* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  struct actor* win, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    struct actor* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  struct actor* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawicon_1(        struct actor* win, u32 rgb, int x0, int y0, int x1, int y1);
void drawhyaline_rect(  struct actor* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawhyaline_circle(struct actor* win, u32 rgb, int cx, int cy, int r);

void drawascii(         struct actor* win, u32 rgb, int cx, int cy, u8 data);
void drawbyte(          struct actor* win, u32 rgb, int cx, int cy, u8 data);
void drawunicode(       struct actor* win, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          struct actor* win, u32 rgb, int cx, int cy, u8* buf, int len);
void drawstring(        struct actor* win, u32 rgb, int cx, int cy, u8* buf, int len);
void drawdecimal(       struct actor* win, u32 rgb, int cx, int cy, int data);
void drawhexadecimal(   struct actor* win, u32 rgb, int cx, int cy, u64 data);
void drawdouble(        struct actor* win, u32 rgb, int cx, int cy, double z);

void drawascii_fit(     struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u64);

void drawtext(          struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawtext_reverse(  struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawvt100(         struct actor* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);




//-----------------------------2in3--------------------------
/*
void carvepoint2d(           struct actor* win, u32 rgb, vec3 vc);
void carvepoint2d_bezier(    struct actor* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carvepoint2d_triangle(  struct actor* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvepoint2d_rect(      struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint2d_circle(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void carveline2d(            struct actor* win, u32 rgb, vec3 va, vec3 vb);
void carveline2d_arrow(      struct actor* win, u32 rgb, vec3 va, vec3 vb);
void carveline2d_bezier(     struct actor* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline2d_yshape(     struct actor* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline2d_triangle(   struct actor* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline2d_rect(       struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline2d_circle(     struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void carvesolid2d_triangle(  struct actor* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid2d_rect(      struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid2d_circle(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid2d_prism4(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid2d_sphere(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveopaque2d_bezier(   struct actor* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveopaque2d_triangle( struct actor* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveopaque2d_rect(     struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque2d_circle(   struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque2d_prism4(   struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque2d_sphere(   struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carve2d_ascii(          struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carve2d_decimal(        struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carve2d_hexadecimal(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carve2d_unicode(        struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carve2d_utf8(           struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carve2d_string(         struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carve2d_double(         struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);
void carve2d_vec4(           struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, char* s, vec3 v);

void carvestring2d_center(   struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext2d(            struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext2d_reverse(    struct actor* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
*/



//-----------------------------3d--------------------------
void carveaxis(               struct actor* ctx);
void carvefrustum(            struct actor* ctx, struct fstyle* sty);
void select_3d(               struct actor* ctx, u32 rgb, struct fstyle* sty, u32 flag);

void carvepoint(              struct actor* ctx, u32 rgb, vec3 vc);
void carvepoint_bezier(       struct actor* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carvepoint_triangle(     struct actor* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvepoint_rect(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_circle(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_cone(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cask(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cylinder(     struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_dodecahedron( struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_icosahedron(  struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_sphere(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveline(               struct actor* ctx, u32 rgb, vec3 va, vec3 vb);
void carveline_arrow(         struct actor* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vn);
void carveline_bezier(        struct actor* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline_special(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu, float sa, float da);
void carveline_yshape(        struct actor* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_triangle(      struct actor* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_rect(          struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_rectselect(    struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_hexagon(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_circle(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_cone(          struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_prism4(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_cylinder(      struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_dodecahedron(  struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_icosahedron(   struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_sphere(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carvesolid_triangle(     struct actor* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid_rect(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_circle(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_cone(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvesolid_prism4(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cask(         struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cylinder(     struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_dodecahedron( struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_icosahedron(  struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_sphere(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveopaque_triangle(    struct actor* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveopaque_rect(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque_circle(      struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque_cone(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveopaque_prism4(      struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_cask(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_cylinder(    struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_dodecahedron(struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_icosahedron( struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_sphere(      struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveascii(              struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carvedecimal(            struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehexadecimal(        struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carveunicode(            struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveutf8(               struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carvestring(             struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvestring_center(      struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext_reverse(       struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvefloat(              struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void carvedouble(             struct actor* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);

void* alloc_winobj(struct actor*, int type);




//libsoft1
int float2decstr(float, void*);
int decstr2float(void* src, void* dst);

int float2hexstr(float, void*);
int hexstr2float(void* src, void* dst);

int double2decstr(double, void*);
int decstr2double(void* src, void* dst);

int double2hexstr(double, void*);
int hexstr2double(void* src, void* dst);

int data2decstr(u64 data,u8* str);
int decstr2data(void* str, u64 data);

int decstr2datastr(void* src, void* dst);
int datastr2decstr(void* dst, void* src, int len);

int data2hexstr(u64 data,u8* str);
int hexstr2data(void* str, u64 data);

int datastr2hexstr(void* dst, void* src, int len);
int hexstr2datastr(void* dst, void* src, int len);
//
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);
//
int cmp(void*,void*);
int ncmp(void*,void*,int);
//
int md5sum(void*, void*, int);
int sha1sum(void*, void*, int);
int sha256sum(void*, void*, int);
int sha384sum(void*, void*, int);
int sha512sum(void*, void*, int);
//
void vec2_normalize(vec2 v);
void vec2_cross(vec2 d, vec2 s);
float vec2_dot(vec2, vec2);
float vec2_cosine(vec2, vec2);
//
void vec3_normalize(vec3 v);
void vec3_normalizeto(vec3 src, vec3 dst);
void vec3_cross(vec3 d, vec3 s);
float vec3_len(vec3 v);
float vec3_dot(vec3, vec3);
float vec3_cosine(vec3, vec3);
//
void quaternion_normalize(vec4 q);
void quaternion_multiply(vec4 l, vec4 r);
void quaternion_rotate(vec3 v, vec4 q);
void quaternion_operation(vec3 v, vec3 a, float delta);
//
void mat2_transpose(mat2 u);
void mat2_multiply(mat2 d, mat2 s);
int mat2_inverse(mat2 d, mat2 s);
//
void mat3_transpose(mat3 u);
void mat3_multiply(mat3 d, mat3 s);
int mat3_inverse(mat3 d, mat3 s);
//
void mat4_transpose(mat4 u);
void mat4_multiply(mat4 d, mat4 s);
int mat4_inverse(mat4 d, mat4 s);

//libhard1
double cosine(double);
double sine(double);
double tangent(double);
double arccos(double);
double arcsin(double);
double arctan2(double, double);

double lg(double);
double ln(double);
double log2(double);
double squareroot(double);
double power(double, double);

//
u32 getrandom();
int sleep_us(int);
u64 dateread();
u64 timeread();




#ifdef __cplusplus
extern "C" {
#endif
int actorevent(struct event*);
int actorread_all();
int arenaevent(struct event*);
int arenaread_all();
//
int system_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
//
int actorread(  struct halfrel* self,struct halfrel* peer,void* buf,int len);
int actorwrite( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int actorstop(  struct halfrel* self,struct halfrel* peer);
int actorstart( struct halfrel* self,struct halfrel* peer);
int actordelete(struct actor*);
void* actorcreate(u64, void*);
//
int arenaread(  struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arenawrite( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arenastop(  struct halfrel* self,struct halfrel* peer);
int arenastart( struct halfrel* self,struct halfrel* peer);
int arenadelete(struct arena*);
void* arenacreate(u64, void*);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* buf,int len);
int arterystop( struct halfrel* self,struct halfrel* peer);
int arterystart(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64, void*);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* buf,int len);
int systemstop( struct halfrel* self,struct halfrel* peer);
int systemstart(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64, void*);
//
int relationread( void* item, int foot, void* buf, int len);
int relationwrite(void* item, int foot, void* buf, int len);
int relationstop( struct halfrel* self,struct halfrel* peer);
int relationstart(struct halfrel* self,struct halfrel* peer);
int relationdelete(void*);
void* relationcreate(void*,void*,int,int,void*,void*,int,int);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
int nodetree_rootread(void*, void*, void*, int);
int nodetree_rootwrite(void*, void*, void*, int);
int threaddelete(u64);
u64 threadcreate(void*, void*);
int memorydelete(void*);
void* memorycreate(int, int);
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int htmlprintf(struct actor*, int, char*, ...);
int mysnprintf(void*, int, void*, ...);
int myvsnprintf(void*, int, void*, __builtin_va_list);
int printmemory(void*, int);
int say(void*, ...);

#ifdef __cplusplus
}
#endif
