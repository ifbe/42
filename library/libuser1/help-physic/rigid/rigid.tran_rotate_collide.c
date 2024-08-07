#include "libuser.h"
#define TIME listu64.data0
#define FLAG listu64.data1
#define TEST listu64.data2
void inertia_tensor_of_block(mat3 Ival, float M, float lx, float ly, float lz);
void mat3_transposefrom(void* o, void* i);
void mat3_multiplyfrom(void* o, void* l, void* r);
void quaternion_multiplyfrom(void* o, void* l, void* r);
void quaternion2eulerian(float* q, float* a);




void rigidsimu_inertiatensor_local2world(mat3 Iworld, mat3 Ilocal, struct fstyle* sty, float mass)
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
		Rpos[j][0] = Rmat[0][j] = vr[j];
		Rpos[j][1] = Rmat[1][j] = vf[j];
		Rpos[j][2] = Rmat[2][j] = vt[j];
	}

	//Iworld = Rmat * Ilocal * Rpos
	mat3_multiplyfrom(tmp, Ilocal, Rpos);
	mat3_multiplyfrom(Iworld, Rmat, tmp);
/*
	//move
	Iworld[0][0] += mass * (vc[1]*vc[1]+vc[2]*vc[2]);
	Iworld[1][1] += mass * (vc[0]*vc[0]+vc[2]*vc[2]);
	Iworld[2][2] += mass * (vc[0]*vc[0]+vc[1]*vc[1]);
	Iworld[0][1] -= mass * vc[0] * vc[1];
	Iworld[1][0] -= mass * vc[0] * vc[1];
	Iworld[0][2] -= mass * vc[0] * vc[2];
	Iworld[2][0] -= mass * vc[0] * vc[2];
	Iworld[1][2] -= mass * vc[1] * vc[2];
	Iworld[2][1] -= mass * vc[1] * vc[2];*/
}
void rigidsimu_inertiatensor_angularalpha(vec3 a, vec3 t, mat3 m)
{
	a[0] = m[0][0]*t[0] + m[0][1]*t[0] + m[0][2]*t[0];
	a[1] = m[1][0]*t[1] + m[1][1]*t[1] + m[1][2]*t[1];
	a[2] = m[2][0]*t[2] + m[2][1]*t[2] + m[2][2]*t[2];
}
void rigidsimu_realforce(struct style* geom)
{
	float mass = 1.0;
	vec4* worldforce = geom->forceinfo.force;
	vec4* worldvector = geom->forceinfo.where;

	//centroid displace
	vec3 totalforce;
	totalforce[0] = worldforce[0][0] + worldforce[1][0] + worldforce[2][0] + worldforce[3][0];
	totalforce[1] = worldforce[0][1] + worldforce[1][1] + worldforce[2][1] + worldforce[3][1];
	totalforce[2] = worldforce[0][2] + worldforce[1][2] + worldforce[2][2] + worldforce[3][2];
	geom->fm.displace_a[0] = totalforce[0] / mass;
	geom->fm.displace_a[1] = totalforce[1] / mass;
	geom->fm.displace_a[2] = totalforce[2] / mass - 9.81;

	mat3 localinertia;
	inertia_tensor_of_block(localinertia, mass,
		geom->fs.vr[3]*2.0,
		geom->fs.vf[3]*2.0,
		geom->fs.vt[3]*2.0);
/*	logtoall("localinertia:\n%f,%f,%f\n%f,%f,%f\n%f,%f,%f\n",
		localinertia[0][0],localinertia[0][1],localinertia[0][2],
		localinertia[1][0],localinertia[1][1],localinertia[1][2],
		localinertia[2][0],localinertia[2][1],localinertia[2][2]
	);*/

	mat3 worldinertia;
	rigidsimu_inertiatensor_local2world(worldinertia, localinertia, &geom->fshape, mass);
/*	logtoall("worldinertia:\n%f,%f,%f\n%f,%f,%f\n%f,%f,%f\n",
		worldinertia[0][0],worldinertia[0][1],worldinertia[0][2],
		worldinertia[1][0],worldinertia[1][1],worldinertia[1][2],
		worldinertia[2][0],worldinertia[2][1],worldinertia[2][2]
	);*/

	mat3 worldinverse;
	mat3_inverse(worldinverse, worldinertia);
/*	logtoall("worldinverse:\n%f,%f,%f\n%f,%f,%f\n%f,%f,%f\n",
		worldinverse[0][0],worldinverse[0][1],worldinverse[0][2],
		worldinverse[1][0],worldinverse[1][1],worldinverse[1][2],
		worldinverse[2][0],worldinverse[2][1],worldinverse[2][2]
	);*/

	vec3 worldtorque[4];
	vec3_cross(worldtorque[0], worldvector[0], worldforce[0]);
	vec3_cross(worldtorque[1], worldvector[1], worldforce[1]);
	vec3_cross(worldtorque[2], worldvector[2], worldforce[2]);
	vec3_cross(worldtorque[3], worldvector[3], worldforce[3]);

	//righdbody rotation
	vec3 totaltorque;
	totaltorque[0] = worldtorque[0][0] + worldtorque[1][0] + worldtorque[2][0] + worldtorque[3][0];
	totaltorque[1] = worldtorque[0][1] + worldtorque[1][1] + worldtorque[2][1] + worldtorque[3][1];
	totaltorque[2] = worldtorque[0][2] + worldtorque[1][2] + worldtorque[2][2] + worldtorque[3][2];
	rigidsimu_inertiatensor_angularalpha(geom->fm.angular_a, totaltorque, worldinverse);
}
void rigidsimu_computeforce(_obj* ent, struct style* geom)
{
	logtoall("%s\n",__func__);
	if(0 == ent->TEST){
		float na = -geom->fm.angular_v[3];
		geom->fm.angular_a[0] = geom->fm.angular_v[0] * na;
		geom->fm.angular_a[1] = geom->fm.angular_v[1] * na;
		geom->fm.angular_a[2] = geom->fm.angular_v[2] * na;

		geom->fm.displace_a[0] = -geom->fm.displace_v[0] * 0.1;
		geom->fm.displace_a[1] = -geom->fm.displace_v[1] * 0.1;
		geom->fm.displace_a[2] = -geom->fm.displace_v[2] * 0.1 - 9.8;
	}

	rigidsimu_realforce(geom);
	if(ent->TEST < 0x80000000)ent->TEST--;
}




int rigidsimu_applydisplace(_obj* ent, struct style* geom, float dt)
{
	struct fmotion* final = &geom->actual;

	final->displace_v[0] += final->displace_a[0] * dt;
	final->displace_v[1] += final->displace_a[1] * dt;
	final->displace_v[2] += final->displace_a[2] * dt;
	final->displace_x[0] += final->displace_v[0] * dt;
	final->displace_x[1] += final->displace_v[1] * dt;
	final->displace_x[2] += final->displace_v[2] * dt;
	return 0;
}




int rigidsimu_applyangular(_obj* ent, struct style* geom, float dt)
{
	vec4 v,ql,qr;
	float a,invn,sbyn;
	struct fmotion* final = &geom->actual;
	float* q = geom->fm.angular_x;


//logtoall("omega_old=%f,%f,%f,%f\n",final->angular_v[0],final->angular_v[1],final->angular_v[2],final->angular_v[3]);
	//omega_now
	v[0] = final->angular_v[0] * final->angular_v[3];
	v[1] = final->angular_v[1] * final->angular_v[3];
	v[2] = final->angular_v[2] * final->angular_v[3];

	//omega_new = omega_now + alpha * dt
	v[0] += final->angular_a[0] * dt;
	v[1] += final->angular_a[1] * dt;
	v[2] += final->angular_a[2] * dt;

	//alpha
	a = vec3_getlen(v);
	if(a < 1e-36){
		final->angular_v[0] = 0.0;
		final->angular_v[1] = 0.0;
		final->angular_v[2] = 1.0;
		final->angular_v[3] = 0.0;
	}
	else{
		//update palstance
		invn = 1.0 / a;
		final->angular_v[0] = v[0] * invn;
		final->angular_v[1] = v[1] * invn;
		final->angular_v[2] = v[2] * invn;
		final->angular_v[3] = a;

		//update attitude: ql=change, qr=current
		sbyn = getsin(dt*a/2) * invn;
		ql[0] = v[0] * sbyn;
		ql[1] = v[1] * sbyn;
		ql[2] = v[2] * sbyn;
		ql[3] = getcos(dt*a/2);
		qr[0] = geom->fm.angular_x[0];
		qr[1] = geom->fm.angular_x[1];
		qr[2] = geom->fm.angular_x[2];
		qr[3] = geom->fm.angular_x[3];
		//logtoall("ql=%f,%f,%f,%f\n",ql[0],ql[1],ql[2],ql[3]);
		//logtoall("qr=%f,%f,%f,%f\n",qr[0],qr[1],qr[2],qr[3]);
		quaternion_multiplyfrom(q, ql, qr);
		//logtoall("q?=%f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);
		quaternion_normalize(q);
		//logtoall("q!=%f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

		//writeback attitude
		a = geom->fshape.vr[3];
		geom->fshape.vr[0] = a * (1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0);
		geom->fshape.vr[1] = a * (2.0 * (q[0]*q[1] + q[2]*q[3]));
		geom->fshape.vr[2] = a * (2.0 * (q[0]*q[2] - q[1]*q[3]));

		a = geom->fshape.vf[3];
		geom->fshape.vf[0] = a * (2.0 * (q[0]*q[1] - q[2]*q[3]));
		geom->fshape.vf[1] = a * (1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0);
		geom->fshape.vf[2] = a * (2.0 * (q[1]*q[2] + q[0]*q[3]));

		a = geom->fshape.vt[3];
		geom->fshape.vt[0] = a * (2.0 * (q[0]*q[2] + q[1]*q[3]));
		geom->fshape.vt[1] = a * (2.0 * (q[1]*q[2] - q[0]*q[3]));
		geom->fshape.vt[2] = a * (1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0);
	}
//logtoall("omega_new=%f,%f,%f,%f\n",final->angular_v[0],final->angular_v[1],final->angular_v[2],final->angular_v[3]);
	return 0;
}




int rigidsimu_solvecollide(_obj* ent, struct style* geom)
{
	logtoall("%s\n",__func__);
	struct fmotion* final = &geom->actual;

	float* dx = final->displace_x;
	float* ax = final->angular_x;

	vec4 ve;
	quaternion2eulerian(ax, ve);

	float x_ang = ve[0]*PI/180;
	float x_cos = getcos(x_ang);

	float wheelsize = geom->fshape.vt[3]/2;
	float massheight = wheelsize*(1+x_cos);

	if(final->displace_x[2] < massheight){
		final->displace_x[2] = massheight;
		final->displace_v[2] = -0.3 * final->displace_v[2];
	}
	//logtoall("%f,%f\n", final->displace_v[2], final->displace_x[2]);

	//writeback position
	geom->fs.vc[0] = final->displace_x[0];
	geom->fs.vc[1] = final->displace_x[1];
	geom->fs.vc[2] = final->displace_x[2];
	return 0;
}




int rigidsimu_foreach(_obj* ent)
{
	u64 now;
	float dt;
	_obj* world;
	struct relation* rel;
	struct style* geom;

	rel = ent->orel0;
	if(0 == rel)return 0;

	world = rel->pdstchip;
	if((_virtual_ != world->type)&&(_scene3d_ != world->type))return 0;

	now = timeread_us();
	//logtoall("%llx\n", now);
	dt = (float)((now - ent->TIME)%1000000)/1000000.0;
	//logtoall("dt=%f\n",dt);
	if(ent->FLAG < 42)ent->FLAG += 1;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		geom = rel->psrcfoot;
		rigidsimu_computeforce(ent, geom);

		rigidsimu_applydisplace(ent, geom, dt);

		rigidsimu_applyangular(ent, geom, dt);

		rigidsimu_solvecollide(ent, geom);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}




int rigidsimu_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@rigidsimu_read:%.4s\n",&foot);
	return 0;
}
int rigidsimu_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//logtoall("@rigidsimu_write:%.4s\n",&foot);
	switch(stack[sp-1].foottype){
	case _clk_:
		rigidsimu_foreach(ent);
		break;
	case _ioby_:
		switch(buf[0]){
		case '1':ent->TEST = 0xffffffff;break;
		case '0':ent->TEST = 0;break;
		default:ent->TEST += 16;
		}
		break;
	}
	return 0;
}
int rigidsimu_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rigidsimu_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int rigidsimu_reader(_obj* ent,void* foot, p64 arg,int key, u8* buf,int len)
{
	return 0;
}
int rigidsimu_writer(_obj* ent,void* foot, p64 arg,int key, u8* buf,int len)
{
	return 0;
}
int rigidsimu_delete(_obj* ent)
{
	return 0;
}
int rigidsimu_create(_obj* ent, void* str)
{
	logtoall("@rigidsimu_create\n");
	ent->FLAG = 0;
	ent->TEST = 0;
	return 0;
}
