#include "libsoft.h"
#define DEBUG 1
#define DEBUG_DETAIL 1
float rigidsimu_f32(float m1,float m2,float r);
//
float vec3_getlen(vec3 v);
float vec3_setlen(vec3 v, float f);
float vec3_cross(float*, float*, float*);
//
int mat3_inverse(float* d, float* s);
void mat3_multiplyfrom(float* o, float* u, float* v);
//
void quaternion_normalize(float* q);
void quaternion_conjugate(float* s, float* d);
void quaternion_multiplyfrom(void* o, void* l, void* r);
void quaternion_rotatefrom(float* o, float* v, float* q);


struct privdata{
    u64 time;
	u64 count;
	struct style* geom[6];
};


void rigidsimu_force2accel(struct style** geom, int count)
{
	struct forceinfo* fi;
	float mass_inv;
	int j,k;
	for(j=0;j<count;j++){			//for each object
		fi = &geom[j]->forceinfo;

		vec3 world_force = {0,0,0};
		vec3 world_relcm_torque = {0,0,0};
		for(k=0;k<fi->cnt;k++){
			if(vec3_getlen(fi->force[k]) < 1e-12)continue;
			world_force[0] += fi->force[k][0];
			world_force[1] += fi->force[k][1];
			world_force[2] += fi->force[k][2];

			vec3 world_relcm_where = {
				fi->where[k][0] - geom[j]->fm.displace_x[0],
				fi->where[k][1] - geom[j]->fm.displace_x[1],
				fi->where[k][2] - geom[j]->fm.displace_x[2]};
			vec3 torque;
			vec3_cross(torque, world_relcm_where, fi->force[k]);
			world_relcm_torque[0] += torque[0];
			world_relcm_torque[1] += torque[1];
			world_relcm_torque[2] += torque[2];
			if(DEBUG_DETAIL)logtoall("(%f,%f,%f)-(%f,%f,%f)=(%f,%f,%f)\n",
				fi->where[k][0], fi->where[k][1], fi->where[k][2],
				geom[j]->fm.displace_x[0], geom[j]->fm.displace_x[1], geom[j]->fm.displace_x[2],
				world_relcm_where[0],world_relcm_where[1],world_relcm_where[2]
				);
			if(DEBUG_DETAIL)logtoall("(%f,%f,%f)X(%f,%f,%f)=(%f,%f,%f)\n",
				world_relcm_where[0],world_relcm_where[1],world_relcm_where[2],
				fi->force[k][0], fi->force[k][1], fi->force[k][2],
				torque[0], torque[1], torque[2]);
		}

		//displace
		//a = km/(s^2) = [m/(s^2)]/1000 = F/M = [F/kg]/1000 = F/(ton*1000)/1000 = F/(ton*1000000)
		mass_inv = 1.0/(geom[j]->physic.inertiatensor[3][3]*1000000);
		geom[j]->fm.displace_a[0] = world_force[0] * mass_inv;
		geom[j]->fm.displace_a[1] = world_force[1] * mass_inv;
		geom[j]->fm.displace_a[2] = world_force[2] * mass_inv;

		//angular
		//local torque
		vec4 q_inv;
		quaternion_conjugate(geom[j]->fm.angular_x, q_inv);
		vec3 local_relcm_torque;
		quaternion_rotatefrom(local_relcm_torque, world_relcm_torque, q_inv);
		//local inertia
		mat3 local_relcm_inertia_tensor = {
			geom[j]->physic.inertiatensor[0][0],
			geom[j]->physic.inertiatensor[0][1],
			geom[j]->physic.inertiatensor[0][2],
			geom[j]->physic.inertiatensor[1][0],
			geom[j]->physic.inertiatensor[1][1],
			geom[j]->physic.inertiatensor[1][2],
			geom[j]->physic.inertiatensor[2][0],
			geom[j]->physic.inertiatensor[2][1],
			geom[j]->physic.inertiatensor[2][2]};
		mat3 local_relcm_inertia_inverse;
		mat3_inverse(&local_relcm_inertia_inverse[0][0], &local_relcm_inertia_tensor[0][0]);
		//local_angular_accel = L / I
		vec3 aa;
		mat3_multiplyfrom(aa, local_relcm_torque, &local_relcm_inertia_inverse[0][0]);
		//world_angular_accel
		quaternion_rotatefrom(geom[j]->fm.angular_a, aa, geom[j]->fm.angular_x);

		//debug
		if(DEBUG)logtoall("force2accel j=%d: m=%f, f=%f,%f,%f,%f, t=%f,%f,%f, xa=%f,%f,%f,%f, aa=%f,%f,%f\n",
			j, geom[j]->physic.inertiatensor[3][3],
			world_force[0], world_force[1], world_force[2], vec3_getlen(world_force),
			world_relcm_torque[0], world_relcm_torque[1], world_relcm_torque[2],
			geom[j]->fm.displace_a[0], geom[j]->fm.displace_a[1], geom[j]->fm.displace_a[2], vec3_getlen(geom[j]->fm.displace_a),
			geom[j]->fm.angular_a[0], geom[j]->fm.angular_a[1], geom[j]->fm.angular_a[2]
		);
	}

	for(j=0;j<count;j++){
		geom[j]->forceinfo.cntlast = geom[j]->forceinfo.cnt;
		geom[j]->forceinfo.cnt = 0;
	}
}

void rigidsimu_acceltime_displace(struct style* geom, float dt)
{
	float la,lv,lx;
	struct fmotion* final = &geom->actual;

	//new_v = old_v + a*dt
	la = vec3_getlen(final->displace_a);
	if(la > 1e-12){
		final->displace_v[0] += final->displace_a[0] * dt;
		final->displace_v[1] += final->displace_a[1] * dt;
		final->displace_v[2] += final->displace_a[2] * dt;
	}

	//check
	lv = vec3_getlen(final->displace_v);
	if(lv < 1e-12)return;

	//new_x = old_x + v*dt
	final->displace_x[0] += final->displace_v[0] * dt;
	final->displace_x[1] += final->displace_v[1] * dt;
	final->displace_x[2] += final->displace_v[2] * dt;

	//writeback
	lx = vec3_getlen(final->displace_x);
	geom->fs.vc[0] = final->displace_x[0];
	geom->fs.vc[1] = final->displace_x[1];
	geom->fs.vc[2] = final->displace_x[2];
}
void rigidsimu_acceltime_angular(struct style* geom, float dt)
{
	float a,invn,sbyn;
	vec4 v,ql,qr;
	struct fmotion* final = &geom->actual;

	//angular:new_v = old_v + a*dt
	v[0] = final->angular_v[0] * final->angular_v[3] + final->angular_a[0] * dt;
	v[1] = final->angular_v[1] * final->angular_v[3] + final->angular_a[1] * dt;
	v[2] = final->angular_v[2] * final->angular_v[3] + final->angular_a[2] * dt;

	//angular_speed < threshold: dont change
	a = vec3_getlen(v);
	if(a < 1e-36)return;

	invn = 1.0 / a;
	final->angular_v[0] = v[0] * invn;
	final->angular_v[1] = v[1] * invn;
	final->angular_v[2] = v[2] * invn;
	final->angular_v[3] = a;

	//angular:new_x = old_x + a*dt
	sbyn = getsin(dt*a/2) * invn;
	ql[0] = v[0] * sbyn;
	ql[1] = v[1] * sbyn;
	ql[2] = v[2] * sbyn;
	ql[3] = getcos(dt*a/2);
	qr[0] = geom->fm.angular_x[0];
	qr[1] = geom->fm.angular_x[1];
	qr[2] = geom->fm.angular_x[2];
	qr[3] = geom->fm.angular_x[3];

	float* q = geom->fm.angular_x;
	quaternion_multiplyfrom(q, ql, qr);
	quaternion_normalize(q);
}
void rigidsimu_acceltime(struct style** geom, int count, float dt)
{
	int j;
	struct fmotion* final = 0;
	for(j=0;j<count;j++){
		final = &geom[j]->actual;
		rigidsimu_acceltime_displace(geom[j], dt);
		rigidsimu_acceltime_angular(geom[j], dt);

		if(DEBUG)logtoall("acceltime j=%d: xv=%f,%f,%f, xx=%f,%f,%f, av=%f,%f,%f, ax=%f,%f,%f\n", j,
			geom[j]->fm.displace_v[0], geom[j]->fm.displace_v[1], geom[j]->fm.displace_v[2],
			geom[j]->fm.displace_x[0], geom[j]->fm.displace_x[1], geom[j]->fm.displace_x[2],
			geom[j]->fm.angular_v[0], geom[j]->fm.angular_v[1], geom[j]->fm.angular_v[2],
			geom[j]->fm.angular_x[0], geom[j]->fm.angular_x[1], geom[j]->fm.angular_x[2]
		);
	}
}


int rigidsimu_foreach(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	u64 now = timeread_us();
	//logtoall("%llx\n", now);
	float dt = (float)((now - priv->time)%1000000)/1000000.0;
	//logtoall("dt=%f\n",dt);

	struct relation* rel = ent->orel0;
	if(0 == rel)return 0;

	_obj* world = rel->pdstchip;
	if((_virtual_ != world->type)&&(_scene3d_ != world->type))return 0;

	priv->count = 0;
	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		priv->geom[priv->count] = rel->psrcfoot;
		priv->count += 1;

		rel = samesrcnextdst(rel);
	}

	rigidsimu_force2accel(priv->geom, priv->count);

	rigidsimu_acceltime(priv->geom, priv->count, dt);

	if(DEBUG)logtoall("\n");

	priv->time = now;
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
		case '1':break;
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




int rigidsimu_reading(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int rigidsimu_writing(_obj* ent,void* foot, p64 arg,int key, u8* buf,int len)
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
	return 0;
}
