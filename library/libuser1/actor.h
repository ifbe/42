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
//
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define _orig_ hex32('o','r','i','g')
#define _ORIG_ hex32('O','R','I','G')
#define _copy_ hex32('c','o','p','y')
#define _COPY_ hex32('C','O','P','Y')
//
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)
//
#define _drop_ hex32('d','r','o','p')
#define _joy_ hex32('j','o','y',0)
#define _kbd_ hex32('k','b','d',0)
#define _char_ hex32('c','h','a','r')
#define _i2c_ hex32('i','2','c',0)
#define _uart_ hex32('u','a','r','t')
//
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _html_ hex32('h','t','m','l')
#define _json_ hex32('j','s','o','n')
#define _vbo_ hex32('v','b','o',0)
#define _8bit_ hex32('8','b','i','t')
//
#define _dl_ hex16('d','l')
#define _dr_ hex16('d','r')
#define _dn_ hex16('d','n')
#define _df_ hex16('d','f')
#define _ka_ hex16('k','a')
#define _kb_ hex16('k','b')
#define _kx_ hex16('k','x')
#define _ky_ hex16('k','y')
#define _lt_ hex16('l','t')
#define _rt_ hex16('r','t')
#define _lb_ hex16('l','b')
#define _rb_ hex16('r','b')
#define _ls_ hex16('l','s')
#define _rs_ hex16('r','s')
#define _ll_ hex16('l','l')
#define _rr_ hex16('r','r')




struct point
{
	u16 x;
	u16 y;
	u16 z;
	u16 id;
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




struct mystring
{
	int len;
	u8 buf[0];
};
struct htmlpiece
{
	int len;
	void* buf;
};
struct texandobj
{
	u32 program;
	u32 vao;

	u32 tex;
	u32 tlen;
	void* tbuf;

	u32 ibo;
	u32 ilen;
	void* ibuf;

	u32 vbo;
	u32 vlen;
	void* vbuf;
};
struct style
{
	//[00,0f]:
	u64 type;
	u64 name;

	//[10,1f]: center
	float cx;
	float cy;
	float cz;
	float cw;

	//[20,2f]: right
	float rx;
	float ry;
	float rz;
	float rw;

	//[30,3f]: far
	float fx;
	float fy;
	float fz;
	float fw;

	//[40,4f]: upper
	float ux;
	float uy;
	float uz;
	float uw;

	//[50,5f]: left
	float lx;
	float ly;
	float lz;
	float lw;

	//[60,6f]: near
	float nx;
	float ny;
	float nz;
	float nw;

	//[70,7f]: bottom
	float bx;
	float by;
	float bz;
	float bw;
};
struct pinid
{
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
};




struct object
{
	//[0x00,0x0f]
	u32 tier;
	u32 type;	//raw, bt, udp, tcp?
	u64 name;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[0x20,0x3f]
	u64 fd;
	u64 flag;
	u64 len;
	union{
		u64 addr;
		void* buf;
	};

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
struct element
{
	//[00,20]
	u32 tier;
	u32 type;	//http, tls, ssh
	u64 stage1;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[20,3f]
	u64 fd;
	u64 flag;
	u64 len;
	union{
		u64 addr;
		void* buf;
	};

	//[40,ff]
	u8 data[0xc0];
};
struct arena
{
	//[00,1f]
	u32 tier;
	u32 type;
	u64 fmt;
	union{
		struct relation* irel;
		char pad0[8];
	};
	union{
		struct relation* orel;
		char pad1[8];
	};

	//[20,3f]data
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

	//[40,7f]func
	u64 oncreate;
	u64 ondelete;
	u64 onstart;
	u64 onstop;
	u64 onread;
	u64 onwrite;
	u64 onlist;
	u64 onchoose;

	//[80,ff]
	int width;
	int height;
	int depth;
	int stride;

	//[90,9f]
	int fwidth;
	int fheight;
	int fdepth;
	int fstride;

	//[a0,af]
	int flag20;
	int flag21;
	int flag22;
	int enq;

	//[b0,bf]
	int flag30;
	int flag31;
	int flag32;
	int deq;

	//[c0,cf]
	int flag40;
	int flag41;
	int vkbddata;
	int vkbdtype;

	//[d0,df]
	int flag50;
	int flag51;
	int menudata;
	int menutype;

	//[e0,ef]
	int flag60;
	int flag61;
	int flag62;
	int flag6;

	//[f0,ff]
	int flag70;
	int flag71;
	int flag72;
	int flag7;

	//[100,1ff]
	struct style target;
	struct style camera;

	//[200,2ff]
	struct point touchdown[16];
	struct point touchmove[16];

	//[300,3ff]
	u8 detail[0x100];
};
struct actor
{
	//[0,1f]
	u32 tier;
	u32 type;
	u64 name;
	union{
		struct relation* irel;
		char pad0[8];
	};
	union{
		struct relation* orel;
		char pad1[8];
	};

	//[20,3f]
	union{
		u64 fd;
		void* what0;
	};
	union{
		u64 abc;
		void* what1;
	};
	union{
		u64 len;
		void* unique;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]vec
	union{
		int (*oncreate)(void* actor, void* buf);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* actor, void* buf);
		char padding1[8];
	};
	union{
		int (*onstart)(void* actor, void* pinid);
		char padding2[8];
	};
	union{
		int (*onstop)(void* actor, void* pinid);
		char padding3[8];
	};
	union{
		int (*onlist)(void* actor, void* pinid, void* buf, int len);
		char padding4[8];
	};
	union{
		int (*onchoose)(void* actor, void* pinid, void* buf, int len);
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

	//[+00,+0f]: center
	float width;
	float height;
	float depth;
	int stride;

	//[+10,+1f]: center
	float cx;
	float cy;
	float cz;
	int dirty;

	//[+20,+2f]: right
	float rx;
	float ry;
	float rz;
	int rw;

	//[+30,+3f]: far
	float fx;
	float fy;
	float fz;
	int fw;

	//[+40,+4f]: upper
	float ux;
	float uy;
	float uz;
	int uw;

	//[+50,+5f]: left
	float lx;
	float ly;
	float lz;
	int ltype;

	//[+60,+6f]: near
	float nx;
	float ny;
	float nz;
	int ntype;

	//[+70,+7f]: bottom
	float bx;
	float by;
	float bz;
	int btype;

	//[100,1ff]
	u8 detail[0x100];
};




//----------------------------------1d------------------------------------
void gentui_rect(  struct arena* win, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  struct arena* win, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   struct arena* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  struct arena* win, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(struct arena* win, u32 rgb, int cx, int cy, int data);




//----------------------------------2d------------------------------------
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
void drawvt100(         struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawicon_1(        struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);

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




//-----------------------------fake2d--------------------------
void carve2d_ascii(            struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8 dat);
void carve2d_decimal(          struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carve2d_hexadecimal(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carve2d_unicode(          struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 uni);
void carve2d_utf8(             struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* buf, int len);
void carve2d_string(           struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* str, int len);
void carvestring2d_center(     struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* str, int len);

void carvepoint2d(             struct arena* win, u32 rgb, float cx, float cy, float cz);
void carvepoint2d_bezier(      struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carvepoint2d_triangle(    struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvepoint2d_rect(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint2d_circle(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvepoint2d_cone(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint2d_cask(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint2d_cylinder(    struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint2d_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint2d_icosahedron( struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint2d_sphere(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);

void carveline2d(              struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2);
void carveline2d_arrow(        struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2);
void carveline2d_bezier(       struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carveline2d_special(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz, float sa, float da);
void carveline2d_yshape(       struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline2d_triangle(     struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline2d_rect(         struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carveline2d_hexagon(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline2d_circle(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carveline2d_cone(         struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline2d_prism4(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline2d_cylinder(     struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline2d_dodecahedron( struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline2d_icosahedron(  struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline2d_sphere(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy);

void carvesolid2d_triangle(    struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvesolid2d_rect(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvesolid2d_circle(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);



//-----------------------------3d--------------------------
void carveascii(             struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8 dat);
void carvedecimal(           struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carvehexadecimal(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carveunicode(           struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 uni);
void carveutf8(              struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* buf, int len);
void carvestring(            struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* str, int len);
void carvestring_center(     struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* str, int len);

void carvepoint(             struct arena* win, u32 rgb, float cx, float cy, float cz);
void carvepoint_bezier(      struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carvepoint_triangle(    struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvepoint_rect(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_circle(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvepoint_cone(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_cask(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_cylinder(    struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint_icosahedron( struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint_sphere(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);

void carveline(              struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2);
void carveline_bezier(       struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carveline_special(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz, float sa, float da);
void carveline_yshape(       struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline_triangle(     struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline_rect(         struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carveline_hexagon(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_circle(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carveline_cone(         struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_prism4(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_cylinder(     struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_dodecahedron( struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_icosahedron(  struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_sphere(       struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);

void carvesolid_triangle(    struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvesolid_rect(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvesolid_circle(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvesolid_cone(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_prism4(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_cask(        struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_cylinder(    struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_icosahedron( struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_sphere(      struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);




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

int cmp(void*,void*);
int ncmp(void*,void*,int);

int md5sum(void*, void*, int);
int sha1sum(void*, void*, int);
int sha256sum(void*, void*, int);
int sha384sum(void*, void*, int);
int sha512sum(void*, void*, int);

void matrixtranspose_2(float* u);
void matrixtranspose_3(float* u);
void matrixtranspose_4(float* u);
void matrixmultiply_2(float* d, float* s);
void matrixmultiply_3(float* d, float* s);
void matrixmultiply_4(float* d, float* s);
int matrixinverse_2(float* d, float* s);
int matrixinverse_3(float* d, float* s);
int matrixinverse_4(float* d, float* s);

void quaternionnormalize(float* q);
void quaternionmultiply(float* l, float* r);
void quaternionrotate(float* v, float* q);

void vectornormalize(float* v);
void vectorcross(float* d, float* s);
float vectordot(float*, float*);
float vectorcosine(float*, float*);

//
int actorread(void* dc,void* df,void* sc,void* sf);
int actorwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arenaread(void* dc,void* df,void* sc,void* sf);
int arenawrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int arteryread(void* dc,void* df,void* sc,void* sf);
int arterywrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);
int systemread(void* dc,void* df,void* sc,void* sf);
int systemwrite(void* dc,void* df,void* sc,void* sf,void* buf, int len);

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
u64 getdate();
u64 gettime();
//
u64 threadcreate(void*, void*);
void threaddelete(u64);
//
void* memorycreate(int);
int memorydelete(void*);
//
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
void* relationcreate(void*,void*,int,void*,void*,int);
int relationdelete(void*);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int htmlprintf(struct arena*, int, char*, ...);
int mysnprintf(void*, int, void*, ...);
int myvsnprintf(void*, int, void*, __builtin_va_list);
int printmemory(void*, int);
int say(void*, ...);
