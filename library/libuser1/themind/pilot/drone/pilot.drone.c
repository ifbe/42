#include "libuser.h"
#define REL_WORLD buf0
#define REL_DRONE buf1
void quaternion2axisangle(float* q, float* a);
void quaternion_multiplyfrom(float* o, float* l, float* r);




void flycon_pidloop_position(struct entity* ent, struct style* sty)
{
	float* expect = sty->expect.angular_x;
	expect[0] = 0.0;
	expect[1] = 0.0;
	expect[2] = 0.0;
	expect[3] = 1.0;
}
void flycon_pidloop_velocity(struct entity* ent, struct style* sty)
{
}
void flycon_pidloop_attitude(struct entity* ent, struct style* sty)
{
/*
	//Mx = M? * M0 -> M? = Mx * M0.transpose
	float n;
	mat3 m;
	struct fstyle* fs = &sty->fshape;
	n = 1.0 / vec3_getlen(fs->vr);
	m[0][0] = fs->vr[0] * n;
	m[1][0] = fs->vr[1] * n;
	m[2][0] = fs->vr[2] * n;
	n = 1.0 / vec3_getlen(fs->vf);
	m[0][1] = fs->vf[0] * n;
	m[1][1] = fs->vf[1] * n;
	m[2][1] = fs->vf[2] * n;
	n = 1.0 / vec3_getlen(fs->vt);
	m[0][2] = fs->vt[0] * n;
	m[1][2] = fs->vt[1] * n;
	m[2][2] = fs->vt[2] * n;
	mat3_transpose(m);
	//mat3_multiplyfrom(result, expect, m);
	matthree2axisangle(m, v);
*/
	float* expect = sty->expect.angular_x;
	float* actual = sty->actual.angular_x;
	vec4 inverse = {-actual[0],-actual[1],-actual[2],actual[3]};
	say("new: %f,%f,%f,%f\n", expect[0], expect[1], expect[2], expect[3]);
	say("now: %f,%f,%f,%f\n", actual[0], actual[1], actual[2], actual[3]);
	say("inv: %f,%f,%f,%f\n",inverse[0],inverse[1],inverse[2],inverse[3]);

	//expect = Q? * actual -> Q? = expect * actual.inverse
	vec4 q;
	quaternion_multiplyfrom(q, expect, inverse);
	say("q-+: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

	float* v = sty->expect.angular_v;
	quaternion2axisangle(q, v);
	say("att: %f,%f,%f\n", v[0],v[1],v[2]);
}
void flycon_pidloop_palstance(struct entity* ent, struct style* sty)
{
	vec3 e0;
	float* e1 = &ent->fx0;
	float* e2 = &ent->fxn;
	float* src = sty->fmotion.angular_v;
	float* dst = sty->expect.angular_v;
	float* dv = sty->expect.angular_a;
	e0[0] = dst[0] - src[0];
	e0[1] = dst[1] - src[1];
	e0[2] = dst[2] - src[2];
	say("v-+: %f,%f,%f\n",e0[0],e0[1],e0[2]);

#define Kp 1.0
#define Ki 0.000002
#define Kd 0.02
	dv[0] += Kp*(e0[0]-e1[0]) + Ki*e0[0] + Kd*(e0[0]+e2[0]-e1[0]*2);
	dv[1] += Kp*(e0[1]-e1[1]) + Ki*e0[1] + Kd*(e0[1]+e2[1]-e1[1]*2);
	dv[2] += Kp*(e0[2]-e1[2]) + Ki*e0[2] + Kd*(e0[2]+e2[2]-e1[2]*2);
	say("wor: %f,%f,%f\n",dv[0],dv[1],dv[2]);

	e2[0] = e1[0];
	e2[1] = e1[1];
	e2[2] = e1[2];
	e1[0] = e0[0];
	e1[1] = e0[1];
	e1[2] = e0[2];
}
void flycon_accel2force(float* ln, float* rn, float* lf, float* rf, struct style* sty)
{
	vec3 accel;
	float* accel_w = sty->expect.angular_a;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vt = sty->fs.vt;

	//world2local
	accel[0] = vr[0]*accel_w[0] + vr[1]*accel_w[1] + vr[2]*accel_w[2];
	accel[1] = vf[0]*accel_w[0] + vf[1]*accel_w[1] + vf[2]*accel_w[2];
	accel[2] = vt[0]*accel_w[0] + vt[1]*accel_w[1] + vt[2]*accel_w[2];	//ignore yaw_z
	say("loc: %f,%f,%f\n",accel[0],accel[1],accel[2]);

	*rn -= (accel[0]+accel[1]) * 0.001;
	*lf += (accel[0]+accel[1]) * 0.001;
	*ln -= (accel[0]-accel[1]) * 0.001;
	*rf += (accel[0]-accel[1]) * 0.001;
}
void flycon_applyforce(struct entity* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	flycon_pidloop_position(ent, sty);
	flycon_pidloop_attitude(ent, sty);
	flycon_pidloop_palstance(ent, sty);

	float ln = 2.5;
	float rn = 2.5;
	float lf = 2.5;
	float rf = 2.5;
	flycon_accel2force(&ln, &rn, &lf, &rf, sty);

	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vt = sty->fs.vt;
	sty->force[0][0] = vt[0] * ln;
	sty->force[0][1] = vt[1] * ln;
	sty->force[0][2] = vt[2] * ln;
	sty->force[1][0] = vt[0] * rn;
	sty->force[1][1] = vt[1] * rn;
	sty->force[1][2] = vt[2] * rn;
	sty->force[2][0] = vt[0] * lf;
	sty->force[2][1] = vt[1] * lf;
	sty->force[2][2] = vt[2] * lf;
	sty->force[3][0] = vt[0] * rf;
	sty->force[3][1] = vt[1] * rf;
	sty->force[3][2] = vt[2] * rf;

	sty->where[0][0] =-vr[0] -vf[0];
	sty->where[0][1] =-vr[1] -vf[1];
	sty->where[0][2] =-vr[2] -vf[2];
	sty->where[1][0] = vr[0] -vf[0];
	sty->where[1][1] = vr[1] -vf[1];
	sty->where[1][2] = vr[2] -vf[2];
	sty->where[2][0] =-vr[0] +vf[0];
	sty->where[2][1] =-vr[1] +vf[1];
	sty->where[2][2] =-vr[2] +vf[2];
	sty->where[3][0] = vr[0] +vf[0];
	sty->where[3][1] = vr[1] +vf[1];
	sty->where[3][2] = vr[2] +vf[2];
}
void flycon_checkplace(struct entity* ent)
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




int flycon_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@flycon_read:%.4s\n",&foot);
	return 0;
}
int flycon_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//say("@flycon_write:%.4s\n",&foot);
	if(_clk_ == foot){
		flycon_checkplace(ent);
		flycon_applyforce(ent);
	}
	return 0;
}
int flycon_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int flycon_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int flycon_search()
{
	return 0;
}
int flycon_modify()
{
	return 0;
}
int flycon_delete(struct entity* ent)
{
	return 0;
}
int flycon_create(struct entity* ent, void* str)
{
	say("@flycon_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;
	ent->fx0 = ent->fy0 = ent->fz0 = 0.0;
	ent->fxn = ent->fyn = ent->fzn = 0.0;
	return 0;
}
