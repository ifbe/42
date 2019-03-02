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
	#define GLSL_VERSION "#version 300 es\n"
#else
	#define GLSL_VERSION "#version 330 core\n"
	#define HLSL_VERSION "#version 330 core\n"
#endif
//
#define _orig_ hex32('o','r','i','g')
#define _ORIG_ hex32('O','R','I','G')
#define _copy_ hex32('c','o','p','y')
#define _COPY_ hex32('C','O','P','Y')
//
#define _perm_ hex32('p','e','r','m')
#define _node_ hex32('n','o','d','e')
#define _twig_ hex32('t','w','i','g')
//
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)
#define _1d_ hex32('1','d',0,0)
#define _2d_ hex32('2','d',0,0)
#define _3d_ hex32('3','d',0,0)
//
#define _coop_ hex32('c','o','o','p')
#define _uart_ hex32('u','a','r','t')
#define _dbg_ hex32('d','b','g',0)
#define _cam_ hex32('c','a','m',0)
#define _mic_ hex32('m','i','c',0)
//
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _func_ hex32('f','u','n','c')
#define _html_ hex32('h','t','m','l')
#define _json_ hex32('j','s','o','n')
#define _rgba_ hex32('r','g','b','a')
#define _pcb_  hex32('p','c','b',0)
#define _sch_  hex32('s','c','h',0)
#define _xml_  hex32('x','m','l',0)
#define _vbo_ hex32('v','b','o',0)
#define _8bit_ hex32('8','b','i','t')
//
#define _std_ hex32('s','t','d', 0)
#define _tray_ hex32('t','r','a','y')
#define _mic_  hex32('m','i','c',0)
#define _cam_  hex32('c','a','m',0)
//
#define _bg3d_ hex32('b','g','3','d')
#define _fg3d_ hex32('f','g','3','d')
#define _ev3d_ hex32('e','v','3','d')
#define _ui3d_ hex32('u','i','3','d')
//
#define _bg2d_ hex32('b','g','2','d')
#define _fg2d_ hex32('f','g','2','d')
#define _ev2d_ hex32('e','v','2','d')
#define _ui2d_ hex32('u','i','2','d')
//
#define _light_ hex32('l','i','g','h')
#define _mirror_ hex32('m','i','r','r')
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
#define joyl_stick   0x0040
#define joyl_select  0x0080
//
#define joyr_left    0x0001
#define joyr_right   0x0002
#define joyr_down    0x0004
#define joyr_up      0x0008
#define joyr_trigger 0x0010
#define joyr_bumper  0x0020
#define joyr_stick   0x0040
#define joyr_start   0x0080
//
#define vbuffmt_33 0x33
#define vbuffmt_333 0x333
#define pcm2 0x2
#define pcm22 0x22
//
#define font3d0 0
#define font3d1 1
#define font3d2 2
#define font3d3 3

#define font2d0 4
#define font2d1 5
#define font2d2 6
#define font2d3 7

#define point3d  8
#define line3d   9
#define trigon3d 10
#define light3d  11

#define point2d  12
#define line2d   13
#define trigon2d 14
#define light2d  15




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
	//[00,1f]shader
	void* vs;
	void* ts;
	void* gs;
	void* fs;

	//[20,6f]argument
	void* arg[4];
	u32 arg_fmt[4];
	u64 arg_data[4];

	//[70,bf]texture
	void* tex[4];
	u32 tex_fmt[4];
	u32 tex_w[4];
	u32 tex_h[4];

	//[c0,e7]vertex
	void* vbuf;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	void* ibuf;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;

	//[e8,eb]
	u8 method;		//'v'=glDrawArrays, 'i'=glDrawElements
	u8 geometry;	//1=point, 2=line, *=trigon
	u8 opaque;		//0=nothing, 1=blend
	u8 target;		//0=rtt, 1=background, 2=geometry, 3=alphatest, 4=transparent, 5=overlay

	//[ec,fa]enq
	u8 shader_enq[4];
	u8 arg_enq[4];
	u8 tex_enq[4];
	u8 ibuf_enq;
	u8 vbuf_enq;
};
struct gldst
{
	//shader
	u32 shader;

	//argument
	u32 arg[4];

	//texture
	u32 tex_len[4];
	u32 tex[4];

	//vertex
	u32 vbo;
	u32 vbo_len;
	u32 ibo;
	u32 ibo_len;

	//
	u32 vao;
	u32 fbo;

	//deq
	u8 shader_deq;
	u8 arg_deq[4];
	u8 tex_deq[4];
	u8 vbo_deq;
	u8 ibo_deq;
};
struct datapair
{
	//[000,0ff]
	struct glsrc src;
	u8 ipadd[0x100 - sizeof(struct glsrc)];

	//[100,1ff]
	struct gldst dst;
	u8 opadd[0x100 - sizeof(struct gldst)];
};




struct style
{
	//[00, 7f]
	vec4 vl;	//[00,0f]: left
	vec4 vr;	//[10,1f]: right
	vec4 vn;	//[20,2f]: near
	vec4 vf;	//[30,3f]: far
	vec4 vb;	//[40,4f]: bottom
	vec4 vu;	//[50,5f]: upper
	vec4 vv;	//[60,6f]: info
	vec4 vc;	//[70,7f]: center

	//[80, ...]
	u64 foot[0];
};
struct pinid
{
	//[00, 7f]
	int flag00;
	int flag01;
	int flag02;
	int flag03;

	int flag04;
	int flag05;
	int flag06;
	int flag07;

	int flag08;
	int flag09;
	int flag0a;
	int flag0b;

	int flag0c;
	int flag0d;
	int flag0e;
	int flag0f;

	int flag10;
	int flag11;
	int flag12;
	int flag13;

	int flag14;
	int flag15;
	int flag16;
	int flag17;

	int flag18;
	int flag19;
	int flag1a;
	int flag1b;

	int flag1c;
	int flag1d;
	int flag1e;
	int flag1f;

	//[80, ...]
	u64 foot[0];
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

	//[40,5f]: data
	union{
		u64 fd;
		void* win;
	};
	union{
		u64 dc;
		u64 gc;
		void* er;
		void* map;
	};
	union{
		u64 len;
		void* ctx;
		void* mod;
		void* ximage;
		void* texture;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[60,7f]
	u64 backdata;
	u64 foredata;
	u64 tempdata;
	u64 vkbddata;

	//[80,8f]: win geom
	short x0;
	short y0;
	short z0;
	short w0;
	short width;
	short height;
	short depth;
	short stride;

	//[90,9f]: fb geom
	short x1;
	short y1;
	short z1;
	short w1;
	short fbwidth;
	short fbheight;
	short fbdepth;
	short fbstride;

	//[a0,af]: near plane
	short x2;
	short y2;
	short z2;
	short w2;
	short nearwidth;
	short nearheight;
	short neardepth;
	short nearstride;

	//[b0,bf]: far plane
	short x3;
	short y3;
	short z3;
	short w3;
	short farwidth;
	short farheight;
	short fardepth;
	short farstride;

	//[c0,cf]: layer0: background
	short x4;
	short y4;
	short z4;
	short w4;
	short backx;
	short backy;
	short backz;
	short backw;

	//[d0,df]: layer1: foreground
	short x5;
	short y5;
	short z5;
	short w5;
	short forex;
	short forey;
	short forez;
	short forew;

	//[e0,ef]: layer2: popup
	short x6;
	short y6;
	short z6;
	short w6;
	short tempx;
	short tempy;
	short tempz;
	short tempw;

	//[f0,ff]: layer3: vkbd
	short x7;
	short y7;
	short z7;
	short w7;
	short vkbdx;
	short vkbdy;
	short vkbdz;
	short vkbdw;

	//[100,1ff]
	struct style target;
	struct style camera;

	//[200,3ff]
	struct xyzwpair input[32];
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
		void* idx;
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

	//[80,bf]vec
	union{
		int (*oncreate)(void* actor, void* buf);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* actor);
		char padding1[8];
	};
	union{
		int (*onstart)(void* dc,void* df,void* sc,void* sf);
		char padding2[8];
	};
	union{
		int (*onstop)(void* dc,void* df,void* sc,void* sf);
		char padding3[8];
	};
	union{
		int (*onget)(void* actor, void* pinid, void* buf, int len);
		char padding4[8];
	};
	union{
		int (*onpost)(void* actor, void* pinid, void* buf, int len);
		char padding5[8];
	};
	union{
		int (*onread)(void* dc,void* df,void* sc,void* sf);
		char padding6[8];
	};
	union{
		int (*onwrite)(void* dc,void* df,void* sc,void* sf,void* buf,int len);
		char padding7[8];
	};

	//[c0,ff]
	u64 onfunc0;
	u64 onfunc1;
	u64 onfunc2;
	u64 onfunc3;
	u64 onfunc4;
	u64 onfunc5;
	u64 onfunc6;
	u64 onfunc7;

	//[100,1ff]
	struct style target;
	struct style camera;
};


void* defaultstyle_vbo2d();


//----------------------------------1d------------------------------------
void gentui_rect(  struct arena* win, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  struct arena* win, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   struct arena* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  struct arena* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(struct arena* win, u32 rgb, int cx, int cy, int data);




//----------------------------------2d------------------------------------
void drawaxis(          struct arena* win);
void select_2d(         struct arena* win, u32 rgb, struct style* sty, u32 flag);

void drawline(          struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawbezier(        struct arena* win, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( struct arena* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  struct arena* win, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   struct arena* win, u32 rgb, int cx, int cy, int r);
void drawline_oval(     struct arena* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   struct arena* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  struct arena* win, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    struct arena* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  struct arena* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawhyaline_rect(  struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawhyaline_circle(struct arena* win, u32 rgb, int cx, int cy, int r);

void drawascii(         struct arena* win, u32 rgb, int cx, int cy, u8 data);
void drawbyte(          struct arena* win, u32 rgb, int cx, int cy, u8 data);
void drawunicode(       struct arena* win, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          struct arena* win, u32 rgb, int cx, int cy, u8* buf, int len);
void drawstring(        struct arena* win, u32 rgb, int cx, int cy, u8* buf, int len);
void drawdecimal(       struct arena* win, u32 rgb, int cx, int cy, int data);
void drawhexadecimal(   struct arena* win, u32 rgb, int cx, int cy, u64 data);
void drawdouble(        struct arena* win, u32 rgb, int cx, int cy, double z);

void drawascii_fit(     struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u64);

void drawtext(          struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawtext_reverse(  struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawvt100(         struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawicon_1(        struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);




//-----------------------------fake2d--------------------------
void carvepoint2d(           struct arena* win, u32 rgb, vec3 vc);
void carvepoint2d_bezier(    struct arena* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carvepoint2d_triangle(  struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvepoint2d_rect(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint2d_circle(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void carveline2d(            struct arena* win, u32 rgb, vec3 va, vec3 vb);
void carveline2d_arrow(      struct arena* win, u32 rgb, vec3 va, vec3 vb);
void carveline2d_bezier(     struct arena* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline2d_yshape(     struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline2d_triangle(   struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline2d_rect(       struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline2d_circle(     struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void carvesolid2d_triangle(  struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid2d_rect(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid2d_circle(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid2d_prism4(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid2d_sphere(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carve2d_ascii(          struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carve2d_decimal(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carve2d_hexadecimal(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carve2d_unicode(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carve2d_utf8(           struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carve2d_string(         struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvestring2d_center(   struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);

void carvetext2d(            struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext2d_reverse(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);




//-----------------------------3d--------------------------
void carveaxis(              struct arena* win);
void carvecamera(            struct arena* win, struct arena* w);
void select_3d(              struct arena* win, u32 rgb, struct style* sty, u32 flag);

void carvepoint(             struct arena* win, u32 rgb, vec3 vc);
void carvepoint_bezier(      struct arena* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carvepoint_triangle(    struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvepoint_rect(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_circle(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_cone(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cask(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cylinder(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_dodecahedron(struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_icosahedron( struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_sphere(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveline(              struct arena* win, u32 rgb, vec3 va, vec3 vb);
void carveline_bezier(       struct arena* win, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline_special(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu, float sa, float da);
void carveline_yshape(       struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_triangle(     struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_rect(         struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_hexagon(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_circle(       struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_cone(         struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_prism4(       struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_cylinder(     struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_dodecahedron( struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_icosahedron(  struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_sphere(       struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carvesolid_triangle(    struct arena* win, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid_rect(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_circle(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_cone(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvesolid_prism4(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cask(        struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cylinder(    struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_dodecahedron(struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_icosahedron( struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_sphere(      struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveascii(             struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carvedecimal(           struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehexadecimal(       struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carveunicode(           struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveutf8(              struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carvestring(            struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvestring_center(     struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);

void* alloc_winobj(struct arena*);




//libsoft1
int double2decstr(double, void*);
int double2hexstr(double, void*);

int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);

int datastr2decstr(void* dst, void* src, int len);
int datastr2hexstr(void* dst, void* src, int len);

int decstr2data(void* str, u64 data);
int decstr2datastr(void* src, void* dst);
int decstr2double(void* src, void* dst);

int hexstr2data(void* str, u64 data);
int hexstr2datastr(void* dst, void* src, int len);
int hexstr2double(void* dst, void* src, int len);
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
void vec3_cross(vec3 d, vec3 s);
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
int actorread_all(struct arena*);
int actorevent(struct event*);
int arenaevent(struct event*);

int actor_leafread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_leafwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_rootread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actor_rootwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int actordelete(struct actor*);
void* actorcreate(u64, void*);
//
int arena_leafread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_leafwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_rootread(  void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arena_rootwrite( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arenadelete(struct arena*);
void* arenacreate(u64, void*);
//
int artery_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int artery_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int arterydelete(void*);
void* arterycreate(u64, void*);
//
int system_leafread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootread( void* dc,void* df,void* sc,void* sf,void* buf,int len);
int system_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len);
int systemdelete(void*);
void* systemcreate(u64, void*);
//
int nodetree_rootread(void*, void*, void*, int);
int nodetree_rootwrite(void*, void*, void*, int);
int threaddelete(u64);
u64 threadcreate(void*, void*);
int memorydelete(void*);
void* memorycreate(int);
int relationdelete(void*);
void* relationcreate(void*,void*,int,void*,void*,int);
//
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int htmlprintf(struct arena*, int, char*, ...);
int mysnprintf(void*, int, void*, ...);
int myvsnprintf(void*, int, void*, __builtin_va_list);
int printmemory(void*, int);
int say(void*, ...);

#ifdef __cplusplus
}
#endif
