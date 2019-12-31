#include "struct.h"
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
#define _event3rd_ hex64('e','v','e','n','t','3','r','d')
//
#define _reality_ hex64('r','e','a','l','i','t','y', 0)
#define _eeworld_ hex64('e','e','w','o','r','l','d', 0)
//
#define _world3d_ hex64('3','d','w','o','r','l','d', 0)
#define _scene3d_ hex64('3','d','s','c','e','n','e', 0)
//
#define _hoffdata_ hex64('h','o','f','f','d','a','t','a')
//dxhelp
#define _dx11data_ hex64('d','x','1','1','d','a','t','a')
#define _dx11coop_ hex64('d','x','1','1','c','o','o','p')

#define _dx11wnd0_ hex64('d','x','1','1','w','n','d','0')
#define _dx11fbod_ hex64('d','x','1','1','f','b','o','d')
#define _dx11fboc_ hex64('d','x','1','1','f','b','o','c')
#define _dx11fbog_ hex64('d','x','1','1','f','b','o','g')
//
#define _gl41data_ hex64('g','l','4','1','d','a','t','a')
#define _gl41coop_ hex64('g','l','4','1','c','o','o','p')

#define _gl41wnd0_ hex64('g','l','4','1','w','n','d','0')
#define _gl41fbod_ hex64('g','l','4','1','f','b','o','d')
#define _gl41fboc_ hex64('g','l','4','1','f','b','o','c')
#define _gl41fbog_ hex64('g','l','4','1','f','b','o','g')
//
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _clk_ hex32('c','l','k',0)
#define  _ev_ hex32('e','v', 0, 0)
//tier
#define _ent_ hex32('e','n','t',0)
#define _sup_ hex32('s','u','p',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
//physic
#define _phys_ hex32('p','h','y','s')
#define _gravity_ hex64('g','r','a','v','i','t','y',0)
#define _collide_ hex64('c','o','l','l','i','d','e',0)
//sensor
#define _sensor_ hex64('s','e','n','s','o','r',0,0)
#define _lidar_ hex64('l','i','d','a','r',0,0,0)
#define _radar_ hex64('r','a','d','a','r',0,0,0)
#define _ahrs_ hex32('a','h','r','s')
#define _slam_ hex32('s','l','a','m')
//motor
#define _car_ hex32('c','a','r',0)
#define _bdc_ hex32('b','d','c',0)
#define _step_ hex32('s','t','e','p')
//sound
#define _mic_ hex32('m','i','c',0)
#define _spk_ hex32('s','p','k',0)
//light
#define _cam_ hex32('c','a','m',0)
#define _wnd_ hex32('w','n','d',0)
//
#define _cap_ hex32('c','a','p',0)
#define _holo_ hex32('h','o','l','o')
//
#define _tray_ hex32('t','r','a','y')	//tray
#define _std_ hex32('s','t','d', 0)		//cmdline
//
#define _w2w_ hex32('w','2','w', 0)
#define _khala_ hex64('k','h','a','l','a', 0, 0, 0)
#define _loopback_ hex64('l','o','o','p','b', 'a', 'c', 'k')
//tier=_sup_, type=_sup_, rfmt:
#define _none_ hex32('n','o','n','e')
#define _easy_ hex32('e','a','s','y')
#define _full_ hex32('f','u','l','l')
#define _coop_ hex32('c','o','o','p')
//tier=_sup_, type=_ctx_, rfmt:
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
//tier=_ent_, type:
#define _baby_ hex32('b','a','b','y')
#define _test_ hex32('t','e','s','t')
#define _pcm_ hex32('p','c','m',0)
#define _ctx_ hex32('c','t','x',0)
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
#define _TCP_ hex32('T','C','P',0)
#define _tcp_ hex32('t','c','p',0)
#define _UDP_ hex32('U','D','P',0)
#define _udp_ hex32('u','d','p',0)
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
//
#define _drag_ hex32('d','r','a','g')
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
#define _kbd_ hex32('k','b','d',0)
#define kbd_up 0x48
#define kbd_down 0x50
#define kbd_left 0x4b
#define kbd_right 0x4d
//
#define _joy_ hex32('j','o','y',0)
#define joy_mask  0xff00ff
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
#define solidaid_max 3
//opaque
#define font3d0 0
#define font3d1 1
#define font3d2 2
#define font3d3 3
#define opaquepoint3d  4
#define opaqueline3d   5
#define opaquetrigon3d 6
#define opaqueaid_max 7
//#define point2d  3
//#define line2d   4
//#define trigon2d 5
//#define font2d0 6
//#define font2d1 7
//#define font2d2 8
//#define font2d3 9
//#define opaquetrigon2d 1




void* defaultstyle_vbo2d();


//----------------------------------1d------------------------------------
void gentui_rect(  struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   struct entity* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  struct entity* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(struct entity* ctx, u32 rgb, int cx, int cy, int data);




//----------------------------------2d------------------------------------
void drawaxis(          struct entity* ctx);
void select_2d(         struct entity* ctx, u32 rgb, struct fstyle* sty, u32 flag);

void drawline(          struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawbezier(        struct entity* ctx, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   struct entity* ctx, u32 rgb, int cx, int cy, int r);
void drawline_oval(     struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   struct entity* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  struct entity* ctx, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  struct entity* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawicon_1(        struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void drawhyaline_rect(  struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawhyaline_circle(struct entity* ctx, u32 rgb, int cx, int cy, int r);

void drawascii(         struct entity* ctx, u32 rgb, int cx, int cy, u8 data);
void drawbyte(          struct entity* ctx, u32 rgb, int cx, int cy, u8 data);
void drawunicode(       struct entity* ctx, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawstring(        struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawdecimal(       struct entity* ctx, u32 rgb, int cx, int cy, int data);
void drawhexadecimal(   struct entity* ctx, u32 rgb, int cx, int cy, u64 data);
void drawdouble(        struct entity* ctx, u32 rgb, int cx, int cy, double z);

void drawascii_fit(     struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u64);

void drawtext(          struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawtext_reverse(  struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawvt100(         struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);




//-----------------------------3d--------------------------
void carveaxis(               struct entity* ctx);
void carvefrustum(            struct entity* ctx, struct fstyle* sty);
void select_3d(               struct entity* ctx, u32 rgb, struct fstyle* sty, u32 flag);

void carvepoint(              struct entity* ctx, u32 rgb, vec3 vc);
void carvepoint_bezier(       struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carvepoint_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvepoint_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_circle(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvepoint_cone(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cask(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvepoint_dodecahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_icosahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvepoint_sphere(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveline(               struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void carveline_shorter(       struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void carveline_arrow(         struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vn);
void carveline_bezier(        struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline_special(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu, float sa, float da);
void carveline_yshape(        struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_triangle(      struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_rect(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_rectselect(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_hexagon(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_circle(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_cone(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_prism4(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_cylinder(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_dodecahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_icosahedron(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_sphere(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carvesolid_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_circle(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_cone(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvesolid_prism4(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cask(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_dodecahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_icosahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_sphere(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_propeller(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

void carveopaque_triangle(    struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveopaque_rect(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque_circle(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveopaque_cone(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveopaque_prism4(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_cask(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_cylinder(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_dodecahedron(struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_icosahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveopaque_sphere(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveascii(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carveascii_center(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carveunicode(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveunicode_center(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveutf8(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carveutf8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void carvedecimal(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehexadecimal(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehex8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvestring(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvestring_center(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext_reverse(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvefloat(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void carvedouble(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);

void* alloc_winobj(struct entity*, int type);




//libsoft1
int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);
int float2decstr(float, void*);
int float2hexstr(float, void*);
int double2decstr(double, void*);
int double2hexstr(double, void*);

int decstr2u16(void* str, u16* data);
int decstr2u32(void* str, u32* data);
int decstr2u64(void* str, u64* data);
int decstr2data(void* str, u64* data);
int decstr2float(void* src, void* dst);
int decstr2double(void* src, void* dst);

int hexstr2u16(void* str, u16* data);
int hexstr2u32(void* str, u32* data);
int hexstr2u64(void* str, u64* data);
int hexstr2data(void* str, u64* data);
int hexstr2float(void* src, void* dst);
int hexstr2double(void* src, void* dst);

int datastr2decstr(void* dst, void* src, int len);
int decstr2datastr(void* dst, void* src, int len);
int hexstr2datastr(void* dst, void* src, int len);
int datastr2hexstr(void* dst, void* src, int len);

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
float vec3_getlen(vec3 v);
void vec3_setlen(vec3 v, float len);
void vec3_normalize(vec3 v);
void vec3_normalizeto(vec3 src, vec3 dst);
void vec3_cross(vec3 v, vec3 a, vec3 b);
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
int entityread(  struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len);
int entitywrite( struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len);
int entitystop(  struct halfrel* self, struct halfrel* peer);
int entitystart( struct halfrel* self, struct halfrel* peer);
int entitydelete(struct entity*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supplyread(  struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len);
int supplywrite( struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len);
int supplystop(  struct halfrel* self, struct halfrel* peer);
int supplystart( struct halfrel* self, struct halfrel* peer);
int supplydelete(struct supply*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int arteryread( struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int arterywrite(struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int arterystop( struct halfrel* self, struct halfrel* peer);
int arterystart(struct halfrel* self, struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int systemread( struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int systemwrite(struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int systemstop( struct halfrel* self, struct halfrel* peer);
int systemstart(struct halfrel* self, struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driverread( struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int driverwrite(struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int driverstop( struct halfrel* self, struct halfrel* peer);
int driverstart(struct halfrel* self, struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int deviceread( struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int devicewrite(struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int devicestop( struct halfrel* self, struct halfrel* peer);
int devicestart(struct halfrel* self, struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int workerread( struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int workerwrite(struct halfrel* self, struct halfrel* peer,void* arg, int idx, void* buf,int len);
int workerstop( struct halfrel* self, struct halfrel* peer);
int workerstart(struct halfrel* self, struct halfrel* peer);
int workerdelete(void*);
void* workercreate(u64 type, void* addr, int argc, u8** argv);
//
int relationread( void* item, int foot, void* arg, int idx, void* buf, int len);
int relationwrite(void* item, int foot, void* arg, int idx, void* buf, int len);
int relationstop( struct halfrel* self, struct halfrel* peer);
int relationstart(struct halfrel* self, struct halfrel* peer);
int relationdelete(void*);
void* relationcreate(void*,void*,int,int,void*,void*,int,int);
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
int threaddelete(u64);
u64 threadcreate(void*, void*);
int memorydelete(void*);
void* memorycreate(int, int);
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int htmlprintf(struct entity*, int, char*, ...);
int mysnprintf(void*, int, void*, ...);
int myvsnprintf(void*, int, void*, __builtin_va_list);
int printmemory(void*, int);
int say(void*, ...);

#ifdef __cplusplus
}
#endif
