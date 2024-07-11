#include "const/def.h"

#ifndef _STRUCT_H
#define _STRUCT_H



struct str
{
	int len;
	int end;
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




struct kv44{
	u32 key;
	u32 val;
};
struct kv88{
	u64 key;
	u64 val;
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


struct physprop{
	mat4 inertiatensor;
	mat4 inertiainverse;
};


struct forceinfo{
	//[0,0x6f]
	vec4 force[7];

	//[0x70,0xdf]
	vec4 where[7];

	//[0xe0,0xff]
	int cnt;
	int padd[7];
};


struct style
{
//--------[00, ff]: renderer--------
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

//--------[100, 1ff]: flight control--------
	//[100, 17f]: current motion
	union{
		struct fmotion estimate;
	};

	//[180, 1ff]: future motion
	union{
		struct fmotion desire;
	};

//--------[200, 2ff]: graphic engine--------
	//[200, 27f]: actual motion
	union{
		struct fmotion actual;
		struct fmotion fm;
		struct fmotion fmotion;
		struct imotion im;
		struct imotion imotion;
	};

	//[280, 2ff]: actual motion
	union{
		struct physprop physic;
	};

//--------[300, 3ff]: force position--------
	struct forceinfo forceinfo;
};




#endif
