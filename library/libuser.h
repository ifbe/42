#include "naming.h"
#include "struct.h"
#include "operate.h"

#include "carve.h"
#include "draw.h"




//
#ifdef __ANDROID__
	#define GLSL_VERSION "#version 320 es\n"
	#define GLSL_PRECISION "precision highp float;\n"
#else
	#define GLSL_VERSION "#version 410 core\n"
	#define GLSL_PRECISION "precision mediump float;\n"
#endif
//mouse
#define point_onto hex16('p','+')
#define point_away hex16('p','-')
#define point_move hex16('p','@')
#define point_shut hex16('p','?')
//touch
#define touch_onto hex16('t','+')
#define touch_away hex16('t','-')
#define touch_move hex16('t','@')
#define touch_shut hex16('t','?')
//keyboard
#define kbd_up 0x48
#define kbd_down 0x50
#define kbd_left 0x4b
#define kbd_right 0x4d
//gamepad
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




#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------1d------------------------------------
void gentui_rect(  struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   struct entity* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  struct entity* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(struct entity* ctx, u32 rgb, int cx, int cy, int data);




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
int cmp(void*,void*);
int ncmp(void*,void*,int);

//
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);
void savefilefromtex(char* name, struct texture* tex);
void loadtexfromfile(struct texture* tex, char* name);
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
void vec3_normalizefrom(vec3 dst, vec3 src);
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
double getcos(double);
double getsin(double);
double gettan(double);
double arccos(double);
double arcsin(double);
double arctanyx(double, double);

double lg(double);
double ln(double);
//double log2(double);
double squareroot(double);
double power(double, double);




void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);

//
u32 getrandom();
//
int sleep_ms(int);
int sleep_us(int);
int sleep_ns(int);
//
u64 dateread();
u64 timeread();
//
int threaddelete(u64);
u64 threadcreate(void*, void*);
//
int memorydelete(void*);
void* memorycreate(int, int);
//
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
