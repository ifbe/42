#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define _act_ hex32('a','c','t',0)
#define _buf_ hex32('b','u','f',0)
#define _win_ hex32('w','i','n',0)
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
	u64 destchip;
	u64 destfoot;
	u32 desttype;
	u32 destflag;
	u32 samepinprevchip;
	u32 samepinnextchip;

	//0x20,0x3f
	u64 selfchip;
	u64 selffoot;
	u32 selftype;
	u32 selfflag;
	u32 samechipprevpin;
	u32 samechipnextpin;
};




struct arena
{
	//[00,1f]
	u64 type;
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
		u64 len;
		u64 dc;
		u64 gc;
		void* er;
	};
	union{
		u64 ximage;
		void* texture;
	};
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]vec
	int cx;
	int cy;
	int cz;
	int flag0;

	union{
		int rx;
		int w;
	};
	int ry;
	int rz;
	int flag1;

	int fx;
	union{
		int fy;
		int h;
	};
	int fz;
	int flag2;

	int ux;
	int uy;
	union{
		int uz;
		int d;
	};
	int flag3;

	//[80,ff]
	union{
		u64 info[16];
		struct point touch[16];
		struct
		{
			u64 vertexcount;
			u64 normalcount;
			u64 colorcount;
			u64 texturecount;
			u64 pointcount;
			u64 linecount;
			u64 tricount;
			u64 rectcount;
		};
	};
};
struct actor
{
	//[0,1f]
	u64 type;
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
	u64 vao;
	u64 vbo;
	u64 inf;
	union{
		u64 addr;
		void* buf;
	};

	//[40,7f]vec
	int cx;
	int cy;
	int cz;
	int cw;

	int rx;
	int ry;
	int rz;
	int rw;

	int fx;
	int fy;
	int fz;
	int fw;

	int ux;
	int uy;
	int uz;
	int uw;

	//[80,bf]
	union{
		int (*oncreate)();
		char padding0[8];
	};
	union{
		int (*ondelete)();
		char padding1[8];
	};
	union{
		int (*onstart)(void* act, void* pl);
		char padding2[8];
	};
	union{
		int (*onstop)();
		char padding3[8];
	};
	union{
		int (*onlist)();
		char padding4[8];
	};
	union{
		int (*onchoose)();
		char padding5[8];
	};
	union{
		int (*onread)(void* win, void* act, void* style, void* player);
		char padding6[8];
	};
	union{
		int (*onwrite)(void* event);
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
};
struct style
{
	int flag0;
	int flag1;
	int flag2;
	int flag3;

	int flag4;
	int flag5;
	int flag6;
	int flag7;

	int flag8;
	int flag9;
	int flaga;
	int flagb;

	int flagc;
	int flagd;
	int flage;
	int flagf;

	float i_cx;
	float i_cy;
	float i_cz;
	float i_cw;

	float i_rx;
	float i_ry;
	float i_rz;
	float i_rw;

	float i_fx;
	float i_fy;
	float i_fz;
	float i_fw;

	float i_ux;
	float i_uy;
	float i_uz;
	float i_uw;
};
struct compo
{
	int flag0;
	int flag1;
	int flag2;
	int flag3;

	int flag4;
	int flag5;
	int flag6;
	int flag7;

	int flag8;
	int flag9;
	int flaga;
	int flagb;

	int flagc;
	int flagd;
	int flage;
	int flagf;

	float o_cx;
	float o_cy;
	float o_cz;
	float o_cw;

	float o_rx;
	float o_ry;
	float o_rz;
	float o_rw;

	float o_fx;
	float o_fy;
	float o_fz;
	float o_fw;

	float o_ux;
	float o_uy;
	float o_uz;
	float o_uw;
};




void select_1d(struct arena* win, struct style* sty);
void select_2d(struct arena* win, struct style* sty);
void select_3d(struct arena* win, struct style* sty);
void background(struct arena* win);
void foreground(struct arena* win);

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
void drawvt100(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len
);
void drawicon_1(struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len
);

void drawline(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2
);		//left-upper, right-bottom
void drawline_bezier(struct arena* win, u32 rgb,
	int ax, int ay, int bx, int by, int cx, int cy
);		//corner1, corner2, control


void drawline_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3
);
void drawline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2
);		//left-upper, right-bottom
void drawline_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius
);
void drawline_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy
);		//center, right, upper
void drawline_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end
);		//center, radius, angle1, angle2

void drawsolid_triangle(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2, int x3, int y3
);
void drawsolid_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2
);		//left-upper, right-bottom
void drawsolid_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius
);
void drawsolid_oval(struct arena* win, u32 rgb,
	int cx, int cy, int rx, int ry, int fx, int fy
);		//center, right, upper
void drawsolid_sector(struct arena* win, u32 rgb,
	int cx, int cy, int radius, int start, int end
);		//center, radius, angle1, angle2

void drawhyaline_rect(struct arena* win, u32 rgb,
	int x1, int y1, int x2, int y2
);
void drawhyaline_circle(struct arena* win, u32 rgb,
	int cx, int cy, int radius
);

void carveascii(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8 dat
);
void carvedecimal(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u32 dat
);
void carvestring(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* str, int len
);
void carveunicode(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u32 uni
);
void carveutf8(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	u8* buf, int len
);

void carvepoint(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz
);
void carvepoint_bezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc
);
void carvepoint_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3
);
void carvepoint_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvepoint_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvepoint_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvepoint_cask(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvepoint_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvepoint_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carvepoint_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carvepoint_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);

void carveline(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2
);
void carveline_bezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc
);
void carveline_special(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz,
	float sa, float da
);
void carveline_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3
);
void carveline_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz
);
void carveline_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carveline_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carveline_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carveline_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carveline_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carveline_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carveline_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);

void carvesolid_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3
);
void carvesolid_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz
);
void carvesolid_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvesolid_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvesolid_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carvesolid_cask(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvesolid_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);
void carvesolid_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carvesolid_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
);
void carvesolid_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz
);

//libsoft1
int double2decstr(double, void*);
int double2hexstr(double, void*);

int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);

int datastr2decstr(void* dst, void* src, int len);
int datastr2hexstr(void* dst, void* src, int len);

int decstr2data(void* str, u64 data);
int decstr2datastr(void* dst, void* src, int len);
int decstr2double(void* dst, void* src, int len);

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

//libsoft0
u32 getrandom();
u64 gettime();
void* startmemory(int);
void stopmemory();
int readfile(void*, void*, u64, u64);
int writefile(void*, void*, u64, u64);

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

//libboot1
void eventwrite(u64,u64,u64,u64);
int mysnprintf(void*, int, void*, ...);
int printmemory(void*, int);
int say(void*, ...);
