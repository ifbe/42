#include "libuser.h"
#define REL_WORLD buf0
#define REL_DRONE buf1
//pid
#define e0 fx0
#define e1 fy0
#define e2 fz0
#define integral fw0
//expect
#define expect_x fxn
void quaternion2eulerian(float* q, float* eulerian);




float carcon_pidloop_angle(struct entity* ent, vec3 front)
{
	vec3 vf;
	vec3_normalizefrom(vf, front);

	float a = arctan2(vf[0], vf[1]) - ent->expect_x;
	if(a > PI)a -= PI*2;
	else if(a <-PI)a += PI*2;

	ent->e2 = ent->e1;
	ent->e1 = ent->e0;
	ent->e0 = a;
	ent->integral += ent->e0;

#define Kp 0.05
#define Ki 0.000001
#define Kd 1.0
	return Kp*ent->e0 + Ki*ent->integral + Kd*(ent->e0 - ent->e1);
}
void carcon_applyforce(struct entity* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	struct fstyle* fs = &sty->fs;
	struct ftest* ft = &sty->ft;

	float a = carcon_pidloop_angle(ent, fs->vf);
	float ln =-a;
	float rn = a;
	float lf =-a;
	float rf = a;

	ft->force[0][0] = fs->vf[0] * ln;
	ft->force[0][1] = fs->vf[1] * ln;
	ft->force[0][2] = fs->vf[2] * ln;
	ft->force[1][0] = fs->vf[0] * rn;
	ft->force[1][1] = fs->vf[1] * rn;
	ft->force[1][2] = fs->vf[2] * rn;
	ft->force[2][0] = fs->vf[0] * lf;
	ft->force[2][1] = fs->vf[1] * lf;
	ft->force[2][2] = fs->vf[2] * lf;
	ft->force[3][0] = fs->vf[0] * rf;
	ft->force[3][1] = fs->vf[1] * rf;
	ft->force[3][2] = fs->vf[2] * rf;

	ft->where[0][0] =-fs->vr[0] -fs->vf[0];
	ft->where[0][1] =-fs->vr[1] -fs->vf[1];
	ft->where[0][2] =-fs->vr[2] -fs->vf[2];
	ft->where[1][0] = fs->vr[0] -fs->vf[0];
	ft->where[1][1] = fs->vr[1] -fs->vf[1];
	ft->where[1][2] = fs->vr[2] -fs->vf[2];
	ft->where[2][0] =-fs->vr[0] +fs->vf[0];
	ft->where[2][1] =-fs->vr[1] +fs->vf[1];
	ft->where[2][2] =-fs->vr[2] +fs->vf[2];
	ft->where[3][0] = fs->vr[0] +fs->vf[0];
	ft->where[3][1] = fs->vr[1] +fs->vf[1];
	ft->where[3][2] = fs->vr[2] +fs->vf[2];
}
void carcon_checkplace(struct entity* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _dst_, &tmp[0], &tmp[1]);
	if(ret <= 0)return;

	struct entity* drone = tmp[1]->pchip;
	if(0 == drone)return;

	struct entity* world;
	struct relation* rel = drone->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->fmt) | (_scene3d_ != world->fmt)){
			ent->REL_WORLD = rel->src;
			ent->REL_DRONE = rel->dst;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}




int carcon_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@carcon_read:%.4s\n",&foot);
	return 0;
}
int carcon_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//say("@carcon_write:%.4s\n",&foot);
	if(_clk_ == foot){
		carcon_checkplace(ent);
		carcon_applyforce(ent);
	}
	if(_ioby_ == foot){
		float angle;
		decstr2float(buf, &angle);
		ent->expect_x = angle*PI/180;
		say("input:%f\n",ent->expect_x);
	}
	if(_evby_ == foot){
		ent->expect_x = buf[0]*PI/50 - PI;
		say("input:%f\n",ent->expect_x);
	}
	return 0;
}
int carcon_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int carcon_linkup(struct halfrel* self, struct halfrel* peer)
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
int carcon_delete(struct entity* ent)
{
	return 0;
}
int carcon_create(struct entity* ent, void* str)
{
	say("@carcon_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;

	ent->e0 = 0.0;
	ent->e1 = 0.0;
	ent->e2 = 0.0;
	ent->integral = 0.0;
	return 0;
}
