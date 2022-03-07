#ifndef _STRUCT_H 
#define _STRUCT_H
typedef float f32;
typedef double f64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
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




struct str
{
	int len;
	u8 buf[0];
};
typedef struct str verylong;
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
typedef struct event event;




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




struct pcmdata
{
	int fmt;
	int chan;
	int rate;
	int count;
	short buf[0];
};




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
	vec4 displace_j;	//jerk
	vec4 displace_a;	//accel
	vec4 displace_v;	//velocity
	vec4 displace_x;	//displace
	vec4 angular_j;
	vec4 angular_a;
	vec4 angular_v;
	vec4 angular_x;
};
struct imotion{
	int ja[4];
	int jd[4];
	int aa[4];
	int ad[4];
	int va[4];
	int vd[4];
	int xa[4];
	int xd[4];
};
struct style
{
	//[00, 7f]: bounding volume
	union{
		//float
		struct fstyle fs;
		struct fstyle fshape;
		struct fstyle fbound;
		//int
		struct istyle is;
		struct istyle ishape;
		struct istyle ibound;
	};

	//[80, ff]: observe frustum
	union{
		struct fstyle frus;
		struct fstyle frustum;
	};

	//[100, 17f]: current motion
	union{
		struct fmotion actual;
		struct fmotion fm;
		struct fmotion fmotion;
		struct imotion im;
		struct imotion imotion;
	};

	//[180, 1ff]: future motion
	union{
		struct fmotion desire;
	};

	//[200, 27f]
	vec4 force[8];

	//[280, 2ff]
	vec4 where[8];

	//[300, 3ff]
	u8 padd[0x100];
};




#endif
