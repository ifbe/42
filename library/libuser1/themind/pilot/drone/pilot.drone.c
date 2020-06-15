#include "libuser.h"
#define REL_WORLD buf0
#define REL_DRONE buf1
void quaternion2axisangle(float* q, float* a);
void axisangle2quaternion(float* a, float* q);
void quaternion_multiplyfrom(float* o, float* l, float* r);




#define POS_p 1.0
void flycon_pidloop_position2velocity(struct entity* ent, struct style* sty)
{
	vec4 differ;
	float* pidout = sty->desire.displace_v;
	float* desire = sty->desire.displace_x;
	float* actual = sty->actual.displace_x;

	desire[0] = 0.0;
	desire[1] = 0.0;
	desire[2] = 10.0;

	differ[0] = desire[0] - actual[0];
	differ[1] = desire[1] - actual[1];
	differ[2] = desire[2] - actual[2];

	pidout[0] = POS_p * differ[0];
	pidout[1] = POS_p * differ[1];
	pidout[2] = POS_p * differ[2];

	say("X_desire: %f,%f,%f\n", desire[0], desire[1], desire[2]);
	say("X_actual: %f,%f,%f\n", actual[0], actual[1], actual[2]);
	say("X_differ: %f,%f,%f\n", differ[0], differ[1], differ[2]);
	say("X_pidout: %f,%f,%f\n", pidout[0], pidout[1], pidout[2]);
}
#define VEL_p 1.0
void flycon_pidloop_velocity2attitude(struct entity* ent, struct style* sty, float* thrust)
{
	vec4 differ;
	float* pidout = sty->desire.displace_a;
	float* desire = sty->desire.displace_v;
	float* actual = sty->actual.displace_v;

	//differ = desire - actual
	differ[0] = desire[0] - actual[0];
	differ[1] = desire[1] - actual[1];
	differ[2] = desire[2] - actual[2];
	say("V_desire: %f,%f,%f\n", desire[0], desire[1], desire[2]);
	say("V_actual: %f,%f,%f\n", actual[0], actual[1], actual[2]);
	say("V_differ: %f,%f,%f\n", differ[0], differ[1], differ[2]);

	//world space output
	pidout[0] = VEL_p*differ[0];
	pidout[1] = VEL_p*differ[1];
	pidout[2] = VEL_p*differ[2];

	//coordinate basis
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

	//world space accel -> local space accel
	vec3 acc;
	acc[0] = vr[0]*pidout[0] + vr[1]*pidout[1] + vr[2]*pidout[2];
	acc[1] = vf[0]*pidout[0] + vf[1]*pidout[1] + vf[2]*pidout[2];
	acc[2] = vt[0]*pidout[0] + vt[1]*pidout[1] + vt[2]*pidout[2];	//local thrust
	if(acc[0] > 10.0)acc[0] = 10.0;
	if(acc[0] <-10.0)acc[0] =-10.0;
	if(acc[1] > 10.0)acc[1] = 10.0;
	if(acc[1] <-10.0)acc[1] =-10.0;
	*thrust = 2.45 + acc[2];
	say("%f,%f,%f\n",acc[0],acc[1],acc[2]);

	float a;
	vec4 v;
	quaternion2axisangle(sty->actual.angular_x, v);
	v[0] = 0.0;
	v[1] = 0.0;
	a = acc[0]*PI*0.01;	//max angle = 180*10*0.02 = 36 degree
	v[0] += vf[0] * a;
	v[1] += vf[1] * a;
	v[2] += vf[2] * a;
	a =-acc[1]*PI*0.01;	//max angle = 180*10*0.02 = 36 degree
	v[0] += vr[0] * a;
	v[1] += vr[1] * a;
	v[2] += vr[2] * a;
	axisangle2quaternion(v, sty->desire.angular_x);
}
#define ATT_p 5.0
void flycon_pidloop_attitude2palstance(struct entity* ent, struct style* sty)
{
	float* expect = sty->expect.angular_x;
	float* actual = sty->actual.angular_x;
	vec4 inverse = {-actual[0],-actual[1],-actual[2],actual[3]};
	say("x_desire: %f,%f,%f,%f\n", expect[0], expect[1], expect[2], expect[3]);
	say("x_actual: %f,%f,%f,%f\n", actual[0], actual[1], actual[2], actual[3]);

	//expect = Q? * actual -> Q? = expect * actual.inverse
	vec4 q;
	quaternion_multiplyfrom(q, expect, inverse);
	say("x_differ: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

	//only P, no I,D
	float* v = sty->expect.angular_v;
	quaternion2axisangle(q, v);
	v[0] *= ATT_p;
	v[1] *= ATT_p;
	v[2] *= ATT_p;
	say("x_pidout: %f,%f,%f\n", v[0],v[1],v[2]);
}
#define Kp 0.5
#define Ki 0.0001
#define Kd 0.1
void flycon_pidloop_palstance2accelerate(struct entity* ent, struct style* sty)
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
	float* new = sty->expect.angular_v;
	tmp = 0.8 * (top[0]*new[0] + top[1]*new[1] + top[2]*new[2]) * nxn;
	dst[0] = new[0] - top[0] * tmp;
	dst[1] = new[1] - top[1] * tmp;
	dst[2] = new[2] - top[2] * tmp;

	//differ: desire - actual
	vec4 e0;
	e0[0] = dst[0] - src[0];
	e0[1] = dst[1] - src[1];
	e0[2] = dst[2] - src[2];
	say("v_desire: %f,%f,%f,%f\n",dst[0],dst[1],dst[2],dst[3]);
	say("v_actual: %f,%f,%f,%f\n",src[0],src[1],src[2],src[3]);
	say("v_differ: %f,%f,%f\n",e0[0],e0[1],e0[2]);

	//pidout: compute
	float* e1 = &ent->fx0;
	float* e2 = &ent->fxn;
	float* out = sty->expect.angular_a;
	out[0] += Kp*(e0[0]-e1[0]) + Ki*e0[0] + Kd*(e0[0]+e2[0]-e1[0]*2);
	out[1] += Kp*(e0[1]-e1[1]) + Ki*e0[1] + Kd*(e0[1]+e2[1]-e1[1]*2);
	out[2] += Kp*(e0[2]-e1[2]) + Ki*e0[2] + Kd*(e0[2]+e2[2]-e1[2]*2);
	say("v_pidout: %f,%f,%f\n",out[0],out[1],out[2]);

	//pidval: update
	e2[0] = e1[0];
	e2[1] = e1[1];
	e2[2] = e1[2];
	e1[0] = e0[0];
	e1[1] = e0[1];
	e1[2] = e0[2];
}
void flycon_accel2force(float* ln, float* rn, float* lf, float* rf, struct style* sty, float thrust)
{
//------------thrust---------------
	*ln = thrust;
	*rn = thrust;
	*lf = thrust;
	*rf = thrust;


//------------rotate---------------
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

	//world2local
	vec3 l_acc;
	float* w_acc = sty->expect.angular_a;
	l_acc[0] = vr[0]*w_acc[0] + vr[1]*w_acc[1] + vr[2]*w_acc[2];
	l_acc[1] = vf[0]*w_acc[0] + vf[1]*w_acc[1] + vf[2]*w_acc[2];
	l_acc[2] = vt[0]*w_acc[0] + vt[1]*w_acc[1] + vt[2]*w_acc[2];	//ignore yaw_z
	say("loc: %f,%f,%f\n",l_acc[0],l_acc[1],l_acc[2]);

	//for rotate
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
void flycon_applyforce(struct entity* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float thrust = 2.4;
	//sty->expect.angular_x[0] = 0.0;
	//sty->expect.angular_x[1] = 0.0;
	//sty->expect.angular_x[2] = 0.0;
	//sty->expect.angular_x[3] = 1.0;
	flycon_pidloop_position2velocity(ent, sty);
	flycon_pidloop_velocity2attitude(ent, sty, &thrust);
	flycon_pidloop_attitude2palstance(ent, sty);
	flycon_pidloop_palstance2accelerate(ent, sty);

	float ln = 0.1;
	float rn = 0.1;
	float lf = 0.1;
	float rf = 0.1;
	flycon_accel2force(&ln,&rn,&lf,&rf, sty, thrust);
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
	say("ln,rn,lf,rf = %f,%f,%f,%f\n",ln,rn,lf,rf);
	flycon_force2motor( ln, rn, lf, rf, sty);
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
void flycon_changedesire(struct entity* ent, float angle)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	float* expect = sty->expect.angular_x;
	expect[0] = 0.0;
	expect[1] = 0.0;
	expect[2] = sine(angle/2);
	expect[3] = cosine(angle/2);
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
	if(_evby_ == foot){
		flycon_checkplace(ent);
		flycon_changedesire(ent, buf[0]*PI/50 - PI);
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
