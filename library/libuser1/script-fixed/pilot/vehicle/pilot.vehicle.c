#include "libuser.h"
#define REL_WORLD listptr.buf0
#define REL_DRONE listptr.buf1
//pid
#define e0 whdf.fx0
#define e1 whdf.fy0
#define e2 whdf.fz0
#define integral whdf.fw0
//expect
#define expect_x whdf.fxn
void quaternion2eulerian(float* q, float* eulerian);




#define Ap 10.0
float carcon_pidloop_angle(_obj* ent, vec3 front)
{
	vec3 vf;
	vec3_normalizefrom(vf, front);
	float actual =-arctanyx(vf[0], vf[1]);
	float differ = ent->expect_x - actual;
	if(differ > PI)differ -= tau;
	if(differ <-PI)differ += tau;
logtoall("x_desire = %f\n", ent->expect_x);
logtoall("x_actual = %f\n", actual);
logtoall("x_differ = %f\n", differ);

	return Ap*differ;
}
#define Kp 10.0
#define Ki 0.1
#define Kd 0.1
float carcon_pidloop_speed(_obj* ent, struct style* sty, float expect)
{
	float* v = sty->actual.angular_v;
	float actual = v[3];
	if(v[2] < 0.0)actual = -actual;

	ent->e2 = ent->e1;
	ent->e1 = ent->e0;
	ent->e0 = expect - actual;

	ent->integral *= 0.99;
	ent->integral += ent->e0;
	if(ent->integral <-Kp*100.0)ent->integral =-Kp*100.0;
	if(ent->integral > Kp*100.0)ent->integral = Kp*100.0;
logtoall("v_desire = %f\n", expect);
logtoall("v_actual = %f\n", actual);
logtoall("v_differ = %f\n", ent->e0);

	return Kp*ent->e0 + Ki*ent->integral + Kd*(ent->e0 - ent->e1);
}
void carcon_applyforce(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vt = sty->fs.vt;
	float v = carcon_pidloop_angle(ent, vf);
	logtoall("x_pidout = %f\n",v);
	float a = carcon_pidloop_speed(ent, sty, v);
	logtoall("v_pidout = %f\n",a);

	float ln = 0.1 - a;
	float rn = 0.1 + a;
	float lf = 0.0 - a;
	float rf = 0.0 + a;
#define MAXVAL 1e+6
	if(ln > MAXVAL)ln = MAXVAL;
	if(ln <-MAXVAL)ln =-MAXVAL;
	if(rn > MAXVAL)rn = MAXVAL;
	if(rn <-MAXVAL)rn =-MAXVAL;
	if(lf > MAXVAL)lf = MAXVAL;
	if(lf <-MAXVAL)lf =-MAXVAL;
	if(rf > MAXVAL)rf = MAXVAL;
	if(rf <-MAXVAL)rf =-MAXVAL;
	logtoall("ln,rn,lf,rf = %f,%f,%f,%f\n",ln,rn,lf,rf);

	struct forceinfo* fi = &sty->forceinfo;

	fi->force[0][0] = vf[0] * ln;
	fi->force[0][1] = vf[1] * ln;
	fi->force[0][2] = vf[2] * ln;
	fi->force[1][0] = vf[0] * rn;
	fi->force[1][1] = vf[1] * rn;
	fi->force[1][2] = vf[2] * rn;
	fi->force[2][0] = vf[0] * lf;
	fi->force[2][1] = vf[1] * lf;
	fi->force[2][2] = vf[2] * lf;
	fi->force[3][0] = vf[0] * rf;
	fi->force[3][1] = vf[1] * rf;
	fi->force[3][2] = vf[2] * rf;

	fi->where[0][0] =-vr[0] -vf[0];
	fi->where[0][1] =-vr[1] -vf[1];
	fi->where[0][2] =-vr[2] -vf[2];
	fi->where[1][0] = vr[0] -vf[0];
	fi->where[1][1] = vr[1] -vf[1];
	fi->where[1][2] = vr[2] -vf[2];
	fi->where[2][0] =-vr[0] +vf[0];
	fi->where[2][1] =-vr[1] +vf[1];
	fi->where[2][2] =-vr[2] +vf[2];
	fi->where[3][0] = vr[0] +vf[0];
	fi->where[3][1] = vr[1] +vf[1];
	fi->where[3][2] = vr[2] +vf[2];

	fi->cnt = 4;
}
void carcon_checkplace(_obj* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _dst_, &tmp[0], &tmp[1]);
	if(ret <= 0)return;

	_obj* drone = tmp[1]->pchip;
	if(0 == drone)return;

	_obj* world;
	struct relation* rel = drone->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->type) | (_scene3d_ != world->type)){
			ent->REL_WORLD = rel->src;
			ent->REL_DRONE = rel->dst;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}




int carcon_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@carcon_read:%p,%p\n",ent,foot);
	return 0;
}
int carcon_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//logtoall("@carcon_write:%.4s\n",&foot);
	float angle;
	switch(stack[sp=1].foottype){
	case _clk_:
		carcon_checkplace(ent);
		carcon_applyforce(ent);
		break;
	case _ioby_:
		decstr2float(buf, &angle);
		ent->expect_x = angle*PI/180;
		logtoall("input:%f\n",ent->expect_x);
		break;
	case _evby_:
		ent->expect_x = buf[0]*PI/50 - PI;
		logtoall("input:%f\n",ent->expect_x);
		break;
	}
	return 0;
}
int carcon_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int carcon_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int carcon_search()
{
	return 0;
}
int carcon_modify()
{
	return 0;
}
int carcon_delete(_obj* ent)
{
	return 0;
}
int carcon_create(_obj* ent, void* str)
{
	logtoall("@carcon_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;

	ent->e0 = 0.0;
	ent->e1 = 0.0;
	ent->e2 = 0.0;
	ent->integral = 0.0;
	return 0;
}
