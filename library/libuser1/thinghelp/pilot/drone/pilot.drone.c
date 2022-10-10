#include "libuser.h"
#define REL_WORLD listptr.buf0
#define REL_DRONE listptr.buf1
#define ENT_DBGUI listptr.buf2
void quaternion2axismulangle(float* q, float* a);
void axismulangle2quaternion(float* a, float* q);
void quaternion_multiplyfrom(float* o, float* l, float* r);


#define dronelog if(0)say


#define POS_p 1.0
void flycon_pos2att_position2velocity(_obj* ent, struct style* sty)
{
	float* desire = sty->desire.displace_x;
	float* actual = sty->actual.displace_x;

	vec4 differ;
	differ[0] = desire[0] - actual[0];
	differ[1] = desire[1] - actual[1];
	differ[2] = desire[2] - actual[2];

	float* pidout = sty->desire.displace_v;
	pidout[0] = POS_p * differ[0];
	pidout[1] = POS_p * differ[1];
	pidout[2] = POS_p * differ[2];

	dronelog("X_x_desire: %f,%f,%f\n", desire[0], desire[1], desire[2]);
	dronelog("X_x_actual: %f,%f,%f\n", actual[0], actual[1], actual[2]);
	dronelog("X_x_differ: %f,%f,%f\n", differ[0], differ[1], differ[2]);
	dronelog("X_x_pidout: %f,%f,%f\n", pidout[0], pidout[1], pidout[2]);
}
#define VEL_p 1.0
void flycon_pos2att_velocity2attitude(_obj* ent, struct style* sty)
{
	float* desire = sty->desire.displace_v;
	float* actual = sty->actual.displace_v;
	dronelog("X_v_desire: %f,%f,%f\n", desire[0], desire[1], desire[2]);
	dronelog("X_v_actual: %f,%f,%f\n", actual[0], actual[1], actual[2]);

	//differ = desire - actual
	vec4 differ;
	differ[0] = desire[0] - actual[0];
	differ[1] = desire[1] - actual[1];
	differ[2] = desire[2] - actual[2];
	dronelog("X_v_differ: %f,%f,%f\n", differ[0], differ[1], differ[2]);

	if(differ[2] < 0.0)differ[2] = 0;

	float len = vec3_getlen(differ);
	float angle = (len > 20) ? 20 : len;
	angle *= PI/120;

	float c = getcos(angle);
	float s = getsin(angle) / len;

	float* q = sty->desire.angular_x;
	q[0] =-differ[1]*s;
	q[1] = differ[0]*s;
	q[2] = 0.0;
	q[3] = c;
}
#define ATT_p 1.0
void flycon_att2aacc_attitude2palstance(_obj* ent, struct style* sty)
{
	float* expect = sty->desire.angular_x;
	float* actual = sty->actual.angular_x;
	dronelog("A_x_desire: %f,%f,%f,%f\n", expect[0], expect[1], expect[2], expect[3]);
	dronelog("A_x_actual: %f,%f,%f,%f\n", actual[0], actual[1], actual[2], actual[3]);
/*
	//if ass to sky, keep speed
	if(actual[0]*actual[0] + actual[1]*actual[1] > 0.5){
		sty->desire.angular_v[0] = sty->actual.angular_v[0];
		sty->desire.angular_v[1] = sty->actual.angular_v[1];
		sty->desire.angular_v[2] = sty->actual.angular_v[2];
		return;
	}
*/
	//expect = Q? * actual -> Q? = expect * actual.inverse
	vec4 q;
	vec4 inverse = {-actual[0],-actual[1],-actual[2],actual[3]};
	quaternion_multiplyfrom(q, expect, inverse);
	dronelog("A_x_differ: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

	//only P, no I,D
	float* v = sty->desire.angular_v;
	quaternion2axismulangle(q, v);
	v[0] *= ATT_p;
	v[1] *= ATT_p;
	v[2] *= ATT_p;
	dronelog("A_x_pidout: %f,%f,%f\n", v[0],v[1],v[2]);
}
#define Kp 0.5
#define Ki 0.0001
#define Kd 0.1
void flycon_att2aacc_palstance2aacc(_obj* ent, struct style* sty)
{
	float nxn,tmp;
	vec4 src,dst;
	float* top = sty->fshape.vt;
	nxn = 1.0 / (top[0]*top[0] + top[1]*top[1] + top[2]*top[2]);

	//actual: seprate pitch_x,roll_y and yaw_z
	float* now = sty->actual.angular_v;
	tmp = 0.8 * (top[0]*now[0] + top[1]*now[1] + top[2]*now[2]) * nxn;
	src[0] = now[0] - top[0] * tmp;
	src[1] = now[1] - top[1] * tmp;
	src[2] = now[2] - top[2] * tmp;

	//desire: seprate pitch_x,roll_y and yaw_z
	float* new = sty->desire.angular_v;
	tmp = 0.8 * (top[0]*new[0] + top[1]*new[1] + top[2]*new[2]) * nxn;
	dst[0] = new[0] - top[0] * tmp;
	dst[1] = new[1] - top[1] * tmp;
	dst[2] = new[2] - top[2] * tmp;

	//differ: desire - actual
	vec4 e0;
	e0[0] = dst[0] - src[0];
	e0[1] = dst[1] - src[1];
	e0[2] = dst[2] - src[2];
	dronelog("A_v_desire: %f,%f,%f,%f\n",dst[0],dst[1],dst[2],dst[3]);
	dronelog("A_v_actual: %f,%f,%f,%f\n",src[0],src[1],src[2],src[3]);
	dronelog("A_v_differ: %f,%f,%f\n",e0[0],e0[1],e0[2]);

	//pidout: compute
	float* e1 = &ent->whdf.fx0;
	float* e2 = &ent->whdf.fxn;
	float* out = sty->desire.angular_a;
	out[0] += Kp*(e0[0]-e1[0]) + Ki*e0[0] + Kd*(e0[0]+e2[0]-e1[0]*2);
	out[1] += Kp*(e0[1]-e1[1]) + Ki*e0[1] + Kd*(e0[1]+e2[1]-e1[1]*2);
	out[2] += Kp*(e0[2]-e1[2]) + Ki*e0[2] + Kd*(e0[2]+e2[2]-e1[2]*2);
	dronelog("A_v_pidout: %f,%f,%f\n",out[0],out[1],out[2]);

	//pidval: update
	e2[0] = e1[0];
	e2[1] = e1[1];
	e2[2] = e1[2];
	e1[0] = e0[0];
	e1[1] = e0[1];
	e1[2] = e0[2];
}
void flycon_pos2xacc(_obj* ent, struct style* sty)
{
	float* desire = sty->desire.displace_v;
	float* actual = sty->actual.displace_v;
	float* acc = sty->desire.displace_a;
	acc[2] = desire[2] - actual[2];
}
void flycon_accel2force(float* ln, float* rn, float* lf, float* rf, struct style* sty)
{
	vec3 vr,vf,vt;
	float* q = sty->actual.angular_x;
	vr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	vr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	vr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);

	vf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	vf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	vf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);

	vt[0] = 2.0 * (q[0]*q[2] + q[1]*q[3]);
	vt[1] = 2.0 * (q[1]*q[2] - q[0]*q[3]);
	vt[2] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;


//------------thrust---------------
	float* acc = sty->desire.displace_a;
	float thrust = 9*(acc[2]) / (vt[2]);
	if(thrust < 1)thrust = 1;
	if(thrust > 4)thrust = 4;
	*ln = thrust;
	*rn = thrust;
	*lf = thrust;
	*rf = thrust;
	dronelog("thrust = %f\n",thrust);


//------------rotate---------------
	//world2local
	vec3 l_acc;
	float* w_acc = sty->desire.angular_a;
	l_acc[0] = vr[0]*w_acc[0] + vr[1]*w_acc[1] + vr[2]*w_acc[2];
	l_acc[1] = vf[0]*w_acc[0] + vf[1]*w_acc[1] + vf[2]*w_acc[2];
	l_acc[2] = vt[0]*w_acc[0] + vt[1]*w_acc[1] + vt[2]*w_acc[2];	//ignore yaw_z
	dronelog("loc: %f,%f,%f\n",l_acc[0],l_acc[1],l_acc[2]);

	//rotate
	*rn +=-l_acc[0] - l_acc[1] + l_acc[2]*0.1;
	*lf += l_acc[0] + l_acc[1] + l_acc[2]*0.1;
	*ln +=-l_acc[0] + l_acc[1] - l_acc[2]*0.1;
	*rf += l_acc[0] - l_acc[1] - l_acc[2]*0.1;
}
void flycon_force2motor(float ln, float rn, float lf, float rf, struct style* sty)
{
	//basis
	vec3 vr,vf,vt;
	float* q = sty->actual.angular_x;
	vr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	vr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	vr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);
	vf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	vf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	vf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
	vt[0] = 2.0 * (q[0]*q[2] + q[1]*q[3]);
	vt[1] = 2.0 * (q[1]*q[2] - q[0]*q[3]);
	vt[2] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;

	//force
	vec3 yaw;
	yaw[0] = (-vr[0]+vf[0])*0.2;
	yaw[1] = (-vr[1]+vf[1])*0.2;
	yaw[2] = (-vr[2]+vf[2])*0.2;
	sty->force[0][0] = (vt[0]+yaw[0]) * ln;
	sty->force[0][1] = (vt[1]+yaw[1]) * ln;
	sty->force[0][2] = (vt[2]+yaw[2]) * ln;
	yaw[0] = (vr[0]+vf[0])*0.2;
	yaw[1] = (vr[1]+vf[1])*0.2;
	yaw[2] = (vr[2]+vf[2])*0.2;
	sty->force[1][0] = (vt[0]+yaw[0]) * rn;
	sty->force[1][1] = (vt[1]+yaw[1]) * rn;
	sty->force[1][2] = (vt[2]+yaw[2]) * rn;
	yaw[0] = (-vr[0]-vf[0])*0.2;
	yaw[1] = (-vr[1]-vf[1])*0.2;
	yaw[2] = (-vr[2]-vf[2])*0.2;
	sty->force[2][0] = (vt[0]+yaw[0]) * lf;
	sty->force[2][1] = (vt[1]+yaw[1]) * lf;
	sty->force[2][2] = (vt[2]+yaw[2]) * lf;
	yaw[0] = (vr[0]-vf[0])*0.2;
	yaw[1] = (vr[1]-vf[1])*0.2;
	yaw[2] = (vr[2]-vf[2])*0.2;
	sty->force[3][0] = (vt[0]+yaw[0]) * rf;
	sty->force[3][1] = (vt[1]+yaw[1]) * rf;
	sty->force[3][2] = (vt[2]+yaw[2]) * rf;

	//dist
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
void flycon_observe(_obj* ent)
{
}
void flycon_consider(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	//position loop
	flycon_pos2att_position2velocity(ent, sty);
	flycon_pos2att_velocity2attitude(ent, sty);
	flycon_att2aacc_attitude2palstance(ent, sty);
	flycon_att2aacc_palstance2aacc(ent, sty);

	//height loop
	flycon_pos2xacc(ent, sty);
}
void flycon_operate(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	//Aacc+Xacc to force
	float ln = 0.1;
	float rn = 0.1;
	float lf = 0.1;
	float rf = 0.1;
	flycon_accel2force(&ln,&rn,&lf,&rf, sty);

	//force to motor
#define MAX 5.0
#define MIN 0.1
	if(ln > MAX)ln = MAX;
	if(ln < MIN)ln = MIN;
	if(rn > MAX)rn = MAX;
	if(rn < MIN)rn = MIN;
	if(lf > MAX)lf = MAX;
	if(lf < MIN)lf = MIN;
	if(rf > MAX)rf = MAX;
	if(rf < MIN)rf = MIN;
	dronelog("ln,rn,lf,rf = %f,%f,%f,%f\n",ln,rn,lf,rf);
	flycon_force2motor(ln, rn, lf, rf, sty);
}




void flycon_checkplace(_obj* ent)
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
		if(	(_virtual_ != world->hfmt) | (_scene3d_ != world->hfmt)){
			ent->REL_WORLD = rel->src;
			ent->REL_DRONE = rel->dst;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}




void flycon_setdesire(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* desire = sty->desire.displace_x;
	desire[0] = 0.0;
	desire[1] = 0.0;
	desire[2] = 10.0;
}
void flycon_changedesire(_obj* ent, float angle)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* expect = sty->desire.angular_x;
	expect[0] = 0.0;
	expect[1] = 0.0;
	expect[2] = getsin(angle/2);
	expect[3] = getcos(angle/2);
}




void flycon_report(_obj* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	_obj* dbg = ent->ENT_DBGUI;
	if(0 == dbg)return;

	//say("%s\n",__func__);
	dbg->onwriter(dbg, 0, 0, 0, sty, 0);
}




int flycon_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	dronelog("@flycon_read:%.4s\n",&foot);
	return 0;
}
int flycon_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//dronelog("@flycon_write:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
		flycon_checkplace(ent);
		flycon_setdesire(ent);

		flycon_observe(ent);
		flycon_consider(ent);
		flycon_operate(ent);

		flycon_report(ent);
	}
	if(_evby_ == stack[sp-1].foottype){
		flycon_checkplace(ent);
		flycon_changedesire(ent, buf[0]*PI/50 - PI);
	}
	return 0;
}
int flycon_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int flycon_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	_obj* dbg = peer->pchip;
	say("%s: %.4s, %.8s\n", __func__, &self->foottype, &dbg->type);

	switch(self->foottype){
	case _dbg_:
		ent->ENT_DBGUI = dbg;
		break;
	}
	return 0;
}




int flycon_reader()
{
	return 0;
}
int flycon_writer()
{
	return 0;
}
int flycon_delete(_obj* ent)
{
	return 0;
}
int flycon_create(_obj* ent, void* str)
{
	dronelog("@flycon_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;
	ent->ENT_DBGUI = 0;
	ent->whdf.fx0 = ent->whdf.fy0 = ent->whdf.fz0 = 0.0;
	ent->whdf.fxn = ent->whdf.fyn = ent->whdf.fzn = 0.0;
	return 0;
}
