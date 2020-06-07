#include "libuser.h"
#define TIME data0
#define FLAG data1
#define TEST data2
void inertia_tensor_of_block(mat3 Ival, mat3 Iinv, float M, float lx, float ly, float lz);
void mat3_transposefrom(void* o, void* i);
void mat3_multiplyfrom(void* o, void* l, void* r);
void quaternion_multiplyfrom(void* o, void* l, void* r);




void gravtest_resistance(struct style* geom)
{
	float na = -geom->fm.angular_v[3];
	geom->fm.angular_a[0] = geom->fm.angular_v[0] * na;
	geom->fm.angular_a[1] = geom->fm.angular_v[1] * na;
	geom->fm.angular_a[2] = geom->fm.angular_v[2] * na;

	geom->fm.displace_a[0] = -geom->fm.displace_v[0] * 0.1;
	geom->fm.displace_a[1] = -geom->fm.displace_v[1] * 0.1;
	geom->fm.displace_a[2] = -geom->fm.displace_v[2] * 0.1 - 9.8;
}
void gravtest_testforce(struct style* geom)
{
	//say("@gravtest_addforce\n");
	geom->fm.angular_a[0] = 0.0;
	geom->fm.angular_a[1] = 0.0;
	geom->fm.angular_a[2] = 1.0;

	geom->fm.displace_a[0] = 0.0;
	geom->fm.displace_a[1] = 0.0;
	geom->fm.displace_a[2] = 9.8;
}




void inertiatensor_local2world(mat3 Iworld, mat3 Ilocal, struct fstyle* sty, float mass)
{
	int j;
	mat3 tmp;
	mat3 Rmat,Rpos;
	vec3 vr,vf,vt;
	float* vc = sty->vc;

	vec3_normalizefrom(vr, sty->vr);
	vec3_normalizefrom(vf, sty->vf);
	vec3_normalizefrom(vt, sty->vt);
	for(j=0;j<3;j++){
		Rmat[j][0] = Rpos[0][j] = vr[j];
		Rmat[j][1] = Rpos[1][j] = vf[j];
		Rmat[j][2] = Rpos[2][j] = vt[j];
	}

	//Iworld = Rmat * Ilocal * Rpos
	mat3_multiplyfrom(tmp, Ilocal, Rpos);
	mat3_multiplyfrom(Iworld, Rmat, tmp);

	//move
	Iworld[0][0] += mass * (vc[1]*vc[1]+vc[2]*vc[2]);
	Iworld[1][1] += mass * (vc[0]*vc[0]+vc[2]*vc[2]);
	Iworld[2][2] += mass * (vc[0]*vc[0]+vc[1]*vc[1]);
	Iworld[0][1] -= mass * vc[0] * vc[1];
	Iworld[1][0] -= mass * vc[0] * vc[1];
	Iworld[0][2] -= mass * vc[0] * vc[2];
	Iworld[2][0] -= mass * vc[0] * vc[2];
	Iworld[1][2] -= mass * vc[1] * vc[2];
	Iworld[2][1] -= mass * vc[1] * vc[2];
}
void inertiatensor_angularalpha(vec3 a, vec3 t, mat3 m)
{
	a[0] = m[0][0]*t[0] + m[0][1]*t[0] + m[0][2]*t[0];
	a[1] = m[1][0]*t[1] + m[1][1]*t[1] + m[1][2]*t[1];
	a[2] = m[2][0]*t[2] + m[2][1]*t[2] + m[2][2]*t[2];
}
void gravtest_realforce(struct style* geom)
{
	float mass = 1.0;
	mat3 localinverse;	//inverse inertia
	inertia_tensor_of_block(0, localinverse, mass,
		vec3_getlen(geom->fs.vr)*2.0,
		vec3_getlen(geom->fs.vf)*2.0,
		vec3_getlen(geom->fs.vt)*2.0);

	mat3 worldinverse;
	inertiatensor_local2world(worldinverse, localinverse, &geom->fshape, mass);
/*
	vec3 worldvector[4];
	worldvector[0][0] =-geom->fs.vr[0] -geom->fs.vf[0];
	worldvector[0][1] =-geom->fs.vr[1] -geom->fs.vf[1];
	worldvector[0][2] =-geom->fs.vr[2] -geom->fs.vf[2];
	worldvector[1][0] = geom->fs.vr[0] -geom->fs.vf[0];
	worldvector[1][1] = geom->fs.vr[1] -geom->fs.vf[1];
	worldvector[1][2] = geom->fs.vr[2] -geom->fs.vf[2];
	worldvector[2][0] =-geom->fs.vr[0] +geom->fs.vf[0];
	worldvector[2][1] =-geom->fs.vr[1] +geom->fs.vf[1];
	worldvector[2][2] =-geom->fs.vr[2] +geom->fs.vf[2];
	worldvector[3][0] = geom->fs.vr[0] +geom->fs.vf[0];
	worldvector[3][1] = geom->fs.vr[1] +geom->fs.vf[1];
	worldvector[3][2] = geom->fs.vr[2] +geom->fs.vf[2];

	float f;
	vec3 worldforce[4];
	f = ('z'==key) ? 1.0002 : 1.0;
	worldforce[0][0] = geom->fs.vt[0] * f;
	worldforce[0][1] = geom->fs.vt[1] * f;
	worldforce[0][2] = geom->fs.vt[2] * f;
	f = ('c'==key) ? 1.0002 : 1.0;
	worldforce[1][0] = geom->fs.vt[0] * f;
	worldforce[1][1] = geom->fs.vt[1] * f;
	worldforce[1][2] = geom->fs.vt[2] * f;
	f = ('q'==key) ? 1.0002 : 1.0;
	worldforce[2][0] = geom->fs.vt[0] * f;
	worldforce[2][1] = geom->fs.vt[1] * f;
	worldforce[2][2] = geom->fs.vt[2] * f;
	f = ('e'==key) ? 1.0002 : 1.0;
	worldforce[3][0] = geom->fs.vt[0] * f;
	worldforce[3][1] = geom->fs.vt[1] * f;
	worldforce[3][2] = geom->fs.vt[2] * f;
*/
	vec4* worldvector = geom->where;
	vec4* worldforce = geom->force;

	vec3 worldtorque[4];
	vec3_cross(worldtorque[0], worldvector[0], worldforce[0]);
	vec3_cross(worldtorque[1], worldvector[1], worldforce[1]);
	vec3_cross(worldtorque[2], worldvector[2], worldforce[2]);
	vec3_cross(worldtorque[3], worldvector[3], worldforce[3]);

	vec3 totalforce;
	totalforce[0] = worldforce[0][0] + worldforce[1][0] + worldforce[2][0] + worldforce[3][0];
	totalforce[1] = worldforce[0][1] + worldforce[1][1] + worldforce[2][1] + worldforce[3][1];
	totalforce[2] = worldforce[0][2] + worldforce[1][2] + worldforce[2][2] + worldforce[3][2];

	vec3 totaltorque;
	totaltorque[0] = worldtorque[0][0] + worldtorque[1][0] + worldtorque[2][0] + worldtorque[3][0];
	totaltorque[1] = worldtorque[0][1] + worldtorque[1][1] + worldtorque[2][1] + worldtorque[3][1];
	totaltorque[2] = worldtorque[0][2] + worldtorque[1][2] + worldtorque[2][2] + worldtorque[3][2];

	//
	geom->fm.displace_a[0] = totalforce[0] / mass;
	geom->fm.displace_a[1] = totalforce[1] / mass;
	geom->fm.displace_a[2] = totalforce[2] / mass - 9.81;
	inertiatensor_angularalpha(geom->fm.angular_a, totaltorque, worldinverse);
}




int gravtest_effect(struct style* geom, float dt)
{
	vec4 v,ql,qr;
	float a,invn,sbyn;
	struct fmotion* final = &geom->fm;
	float* q = geom->fm.angular_x;


	//angular
	v[0] = final->angular_v[0] * final->angular_v[3];
	v[1] = final->angular_v[1] * final->angular_v[3];
	v[2] = final->angular_v[2] * final->angular_v[3];

	//omega = alpha * dt
	v[0] += final->angular_a[0] * dt;
	v[1] += final->angular_a[1] * dt;
	v[2] += final->angular_a[2] * dt;

	//helper
	a = vec3_getlen(v);
	invn = 1.0 / a;
	sbyn = sine(a/2) * invn;
	final->angular_v[0] = v[0] * invn;
	final->angular_v[1] = v[1] * invn;
	final->angular_v[2] = v[2] * invn;
	final->angular_v[3] = a;

	//quaternion
	ql[0] = v[0] * sbyn;
	ql[1] = v[1] * sbyn;
	ql[2] = v[2] * sbyn;
	ql[3] = cosine(a/2);
	qr[0] = geom->fm.angular_x[0];
	qr[1] = geom->fm.angular_x[1];
	qr[2] = geom->fm.angular_x[2];
	qr[3] = geom->fm.angular_x[3];
	quaternion_multiplyfrom(geom->fm.angular_x, ql, qr);
	quaternion_normalize(geom->fm.angular_x);

	//writeback attitude
	geom->fshape.vr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	geom->fshape.vr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	geom->fshape.vr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);

	geom->fshape.vf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	geom->fshape.vf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	geom->fshape.vf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);

	geom->fshape.vt[0] = 2.0 * (q[0]*q[2] + q[1]*q[3]);
	geom->fshape.vt[1] = 2.0 * (q[1]*q[2] - q[0]*q[3]);
	geom->fshape.vt[2] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;


	//displacement
	final->displace_v[0] += final->displace_a[0] * dt;
	final->displace_v[1] += final->displace_a[1] * dt;
	final->displace_v[2] += final->displace_a[2] * dt;
	final->displace_x[0] += final->displace_v[0] * dt;
	final->displace_x[1] += final->displace_v[1] * dt;
	final->displace_x[2] += final->displace_v[2] * dt;


	//collide
	if(final->displace_x[2] < 0.0){
		final->displace_x[2] = 0.00001;
		final->displace_v[2] = -0.5 * final->displace_v[2];
	}
	//say("%f,%f\n", final->displace_v[2], final->displace_x[2]);

	//writeback position
	geom->fs.vc[0] = final->displace_x[0];
	geom->fs.vc[1] = final->displace_x[1];
	geom->fs.vc[2] = final->displace_x[2];
	return 0;
}
int gravtest_foreach(struct entity* ent)
{
	u64 now;
	float dt;
	struct entity* world;
	struct relation* rel;
	struct style* geom;

	rel = ent->orel0;
	if(0 == rel)return 0;

	world = rel->pdstchip;
	if((_virtual_ != world->fmt)&&(_scene3d_ != world->fmt))return 0;

	now = timeread();
	//say("%llx\n", now);
	dt = (float)((now - ent->TIME)%1000000)/1000000.0;
	//say("dt=%f\n",dt);
	if(ent->FLAG < 42)ent->FLAG += 1;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		geom = rel->psrcfoot;
		if(ent->TEST){
			//gravtest_testforce(geom);
			gravtest_realforce(geom);
			if(ent->TEST < 0x80000000)ent->TEST--;
		}
		else{
			gravtest_resistance(geom);
		}
		gravtest_effect(geom, dt);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}




int gravtest_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@gravtest_read:%.4s\n",&foot);
	return 0;
}
int gravtest_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//say("@gravtest_write:%.4s\n",&foot);
	if(_clk_ == foot)gravtest_foreach(ent);
	if(_ioby_ == foot){
		switch(buf[0]){
			case '1':ent->TEST = 0xffffffff;break;
			case '0':ent->TEST = 0;break;
			default:ent->TEST += 16;
		}
	}
	return 0;
}
int gravtest_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravtest_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gravtest_search()
{
	return 0;
}
int gravtest_modify()
{
	return 0;
}
int gravtest_delete(struct entity* ent)
{
	return 0;
}
int gravtest_create(struct entity* ent, void* str)
{
	say("@gravtest_create\n");
	ent->FLAG = 0;
	ent->TEST = 0;
	return 0;
}
