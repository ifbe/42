#ifndef _DEFINE_H
#define _DEFINE_H

//unsigned
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

//signed
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

//pointer
//typedef ??? p8;
//typedef ??? p16;
typedef unsigned int       p32;		//on 32bit cpu, sizeof(void*)=4
typedef unsigned long long p64;		//on 64bit cpu, sizeof(void*)=8

//float
//typedef ??? f8;
//typedef ??? f8_s1e3m4;		//mini float8
//typedef ??? f16;
//typedef _Float16 f16_s1e5m10;	//ieee764 float16
//typedef __bf16 f16_s1e8m7;	//brain float16
typedef float f32;
//typedef float f32_s1e8m23;	//ieee754 float32
typedef double f64;
//typedef double f64_s1e11m52;	//ieee764 float64

//f32x4
typedef float vec1[1];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

//f32x4x4
typedef float mat1[1][1];
typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];

//
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))

#endif