#include "const/def.h"
#include "const/math.h"
#include "const/naming.h"
#include "const/event.h"
#include "data/node-inner.h"
#include "data/node-outer.h"
#include "data/wire-inner.h"
#include "data/wire-outer.h"
#include "my/draw.h"
#include "my/carve.h"




//audio
#define pcm2      0x2
#define pcm22     0x22
#define pcm222    0x222
#define pcm2222   0x2222
#define pcm22222  0x22222
#define pcm222222 0x222222




#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------1d------------------------------------
void gentui_rect(  _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void gentui_utf8(  _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void gentui_str(   _obj* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_text(  _obj* ctx, u32 rgb, int cx, int cy, u8* str, int len);
void gentui_decstr(_obj* ctx, u32 rgb, int cx, int cy, int data);




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
_obj* file_create(void*, int);
int file_delete(_obj*);
int file_reader(_obj*, int, p64, int, void*, int);
int file_writer(_obj*, int, p64, int, void*, int);
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);
//
int savefilefromtex(char* name, struct texture* tex);
int loadtexfromfile(struct texture* tex, char* name);
int loadglslfromfile(void* url, int max, void* buf, int len);
int loadhlslfromfile(void* url, int max, void* buf, int len);
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




//
u32 random_read();
//
int sleep_ms(int);
int sleep_us(int);
int sleep_ns(int);
//
u64 timeread_ms();
u64 timeread_us();
u64 timeread_ns();
//
u64 dateread();
//
int threaddelete(u64);
u64 threadcreate(void*, void*);
//
int memoryfree(void*);
void* memoryalloc(int, int);
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int htmlprintf(_obj*, int, char*, ...);
int mysnprintf(void*, int, void*, ...);
int myvsnprintf(void*, int, void*, __builtin_va_list);
//
int printmemory(void*, int);
int printmmio(void*, int);
int logtoall(void*, ...);

#ifdef __cplusplus
}
#endif
