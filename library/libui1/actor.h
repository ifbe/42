#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
//
#define __kbd__ hex32('k','b','d',0)
#define __char__ hex32('c','h','a','r')
#define __i2c__ hex32('i','2','c',0)
#define __uart__ hex32('u','a','r','t')
//
#define __cli__ hex32('c','l','i',0)
#define __tui__ hex32('t','u','i',0)
#define __vbo__ hex32('v','b','o',0)
#define __html__ hex32('h','t','m','l')
#define __8bit__ hex32('8','b','i','t')




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
	//local,cli,voice,vnc,rdp,...
	//rgba8888,vt100...
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

	//data
	union{
		void* buf;
		u64 fd;
	};
	union{
		u64 len;
		u64 dc;
	};
	u64 ximage;
	u64 thread;

	//where
	u64 cx;
	u64 cy;
	u64 cz;
	u64 cw;

	//size
	u64 w;
	u64 h;
	u64 d;
	u64 t;

	//
	union{
		u64 info[16];
		struct point touch[16];
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
	u64 buf;
	u64 len;
	u64 info;
	u64 flag;

	//[40,77]
	union{
		int (*create)();
		char padding0[8];
	};
	union{
		int (*delete)();
		char padding1[8];
	};
	union{
		int (*start)();
		char padding2[8];
	};
	union{
		int (*stop)();
		char padding3[8];
	};
	union{
		int (*list)();
		char padding4[8];
	};
	union{
		int (*choose)();
		char padding5[8];
	};
	union{
		int (*read)(void* win, void* act, void* style, void* player);
		char padding6[8];
	};
	union{
		int (*write)(void* event);
		char padding7[8];
	};

	//
	u8 priv[0x80];
};
struct style
{
	//[00,1f]:cartesian coordinate
	u64 cx;
	u64 cy;
	u64 cz;
	u64 dim;

	//[20,3f]:eulerian angle
	u64 pitch;
	u64 yaw;
	u64 roll;
	u64 aaa;

	//[40,5f]:total size(base 0x10000)
	u64 wantw;
	u64 wanth;
	u64 wantd;
	u64 hehe;

	//[60,7f]:show area(base 0x100000)
	u64 realw;
	u64 realh;
	u64 reald;
	u64 haha;
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
void drawbyte(struct arena* win, u32 rgb, int x, int y, u8 data);
void drawdecimal(struct arena* win, u32 rgb, int x, int y, int data);
void drawstring(struct arena*, u32 rgb, int x, int y, u8* buf, int len);
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
