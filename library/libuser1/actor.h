#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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
#define _kbd_ hex32('k','b','d',0)
#define _char_ hex32('c','h','a','r')
#define _i2c_ hex32('i','2','c',0)
#define _uart_ hex32('u','a','r','t')
//
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _vbo_ hex32('v','b','o',0)
#define _html_ hex32('h','t','m','l')
#define _8bit_ hex32('8','b','i','t')




struct mystring
{
	int len;
	u8 buf[0];
};
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

	//[80,ff]: w,h,d,s
	int width;
	int height;
	int depth;
	int stride;

	//[90,9f]: center
	float cx;
	float cy;
	float cz;
	int dirty;

	//[a0,af]: right
	float rx;
	float ry;
	float rz;
	int vkbd;

	//[b0,bf]: far
	float fx;
	float fy;
	float fz;
	int theone;

	//[c0,cf]: upper
	float ux;
	float uy;
	float uz;
	int edit;

	//[d0,df]
	int dirtyx0;
	int dirtyy0;
	int dirtyz0;
	int flag0;

	//[e0,ef]
	int dirtyx1;
	int dirtyy1;
	int dirtyz1;
	int enq;

	//[f0,ff]
	int dirtyx2;
	int dirtyy2;
	int dirtyz2;
	int deq;

	//[100,1ff]
	struct point touchdown[16];
	struct point touchmove[16];
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
	int width;
	int height;
	int depth;
	int stride;

	//[+10,+1f]: center
	int cx;
	int cy;
	int cz;
	int dirty;

	//[+20,+2f]: right
	int rx;
	int ry;
	int rz;
	int rw;

	//[+30,+3f]: far
	int fx;
	int fy;
	int fz;
	int fw;

	//[+40,+4f]: upper
	int ux;
	int uy;
	int uz;
	int uw;

	//[+50,+5f]: left
	int lx;
	int ly;
	int lz;
	int ltype;

	//[+60,+6f]: near
	int nx;
	int ny;
	int nz;
	int ntype;

	//[+70,+7f]: bottom
	int bx;
	int by;
	int bz;
	int btype;

	//[100,1ff]
	u8 detail[0x100];
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
	float rtype;

	//[30,3f]: far
	float fx;
	float fy;
	float fz;
	float ftype;

	//[40,4f]: upper
	float ux;
	float uy;
	float uz;
	float utype;

	//[50,5f]: left
	float lx;
	float ly;
	float lz;
	float ltype;

	//[60,6f]: near
	float nx;
	float ny;
	float nz;
	float ntype;

	//[70,7f]: bottom
	float bx;
	float by;
	float bz;
	float btype;
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




void gentui_rect(struct arena* win, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(struct arena* win, u32 rgb, int x, int y, u8* buf, int len);
void gentui_str(struct arena* win, u32 rgb, int x, int y, u8* str, int len);
void gentui_text(struct arena* win, u32 rgb, int x, int y, u8* str, int len);
void gentui_decstr(struct arena* win, u32 rgb, int x, int y, int data);

void drawascii(struct arena* win, u32 rgb, int x, int y, u8 data);
void drawunicode(struct arena* win, u32 rgb, int x, int y, u32 unicode);
void drawutf8(struct arena* win, u32 rgb, int x, int y, u8* buf, int len);
void drawstring(struct arena*, u32 rgb, int x, int y, u8* buf, int len);
void drawbyte(struct arena* win, u32 rgb, int x, int y, u8 data);
void drawdecimal(struct arena* win, u32 rgb, int x, int y, int data);
void drawhexadecimal(struct arena* win, u32 rgb, int x, int y, u64 data);
void drawdouble(struct arena*, u32 rgb, int x, int y, double z);
void drawtext(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len
);
void drawvt100(struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawicon_1(struct arena* win, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawline(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawbezier(struct arena* win, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);

void drawline_triangle(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(struct arena* win, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(struct arena* win, u32 rgb, int cx, int cy, int r);
void drawline_oval(struct arena* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(struct arena* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(struct arena* win, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(struct arena* win, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(struct arena* win, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawhyaline_rect(struct arena* win, u32 rgb, int x1, int y1, int x2, int y2);
void drawhyaline_circle(struct arena* win, u32 rgb, int cx, int cy, int r);

void carveascii(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8 dat);
void carvedecimal(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carvehexadecimal(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 dat);
void carveunicode(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u32 uni);
void carveutf8(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, u8* buf, int len);
void carvestring(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz,float fx, float fy, float fz, u8* str, int len);

void carvepoint(struct arena* win, u32 rgb, float cx, float cy, float cz);
void carvepoint_bezier(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carvepoint_triangle(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvepoint_rect(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_circle(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_cone(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_cask(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_cylinder(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvepoint_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint_icosahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvepoint_sphere(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);

void carveline(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2);
void carveline_bezier(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float xc, float yc, float zc);
void carveline_special(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz, float sa, float da);
void carveline_yshape(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline_triangle(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carveline_rect(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carveline_hexagon(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_circle(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_cone(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_prism4(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_cylinder(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carveline_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_icosahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carveline_sphere(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);

void carvesolid_triangle(struct arena* win, u32 rgb, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void carvesolid_rect(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz);
void carvesolid_circle(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_cone(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_prism4(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_cask(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_cylinder(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);
void carvesolid_dodecahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_icosahedron(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float fx, float fy, float fz, float ux, float uy, float uz);
void carvesolid_sphere(struct arena* win, u32 rgb, float cx, float cy, float cz, float rx, float ry, float rz, float ux, float uy, float uz);

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
void* relationcreate(void*,void*,u64,void*,void*,u64);
int relationdelete(void*);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int mysnprintf(void*, int, void*, ...);
int printmemory(void*, int);
int say(void*, ...);