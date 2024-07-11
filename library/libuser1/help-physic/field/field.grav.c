#include "libsoft.h"
#define DEBUG 0
float gravity_f32(float m1,float m2,float r);
float vec3_getlen(vec3 v);
float vec3_setlen(vec3 v, float f);


struct privdata{
    u64 time;
	u64 count;
	struct style* geom[6];
};


void gravity_computeforce(struct style** geom, int count)
{
	float len;
	float Mself,Mpeer;
	struct forceinfo* fi;

	int j,k;
	for(j=0;j<count;j++){
		for(k=0;k<count;k++){
			if(j==k)continue;

			fi = &geom[j]->forceinfo;

			//F dir
			fi->force[fi->cnt][0] = geom[k]->fm.displace_x[0] - geom[j]->fm.displace_x[0];
			fi->force[fi->cnt][1] = geom[k]->fm.displace_x[1] - geom[j]->fm.displace_x[1];
			fi->force[fi->cnt][2] = geom[k]->fm.displace_x[2] - geom[j]->fm.displace_x[2];

			//F size
			len = vec3_getlen(fi->force[fi->cnt]);
			if(len < 1e-21){
				fi->force[fi->cnt][3] = 0.0;
			}
			else{
				Mself = geom[j]->physic.inertiatensor[3][3];
				Mpeer = geom[k]->physic.inertiatensor[3][3];
				fi->force[fi->cnt][3] = gravity_f32(Mself, Mpeer, len);
			}
			vec3_setlen(fi->force[fi->cnt], fi->force[fi->cnt][3]);

			//F point
			fi->where[fi->cnt][0] = geom[j]->fm.displace_x[0];
			fi->where[fi->cnt][1] = geom[j]->fm.displace_x[1];
			fi->where[fi->cnt][2] = geom[j]->fm.displace_x[2];

			//log
			if(DEBUG)logtoall("computeforce j=%d,k=%d,t=%d: force=%f,%f,%f,%f, where=%f,%f,%f, I=%f,%f,%f,%f\n",
				j, k, fi->cnt,
				fi->force[fi->cnt][0],fi->force[fi->cnt][1],fi->force[fi->cnt][2],fi->force[fi->cnt][3],
				fi->where[fi->cnt][0],fi->where[fi->cnt][1],fi->where[fi->cnt][2],
				geom[j]->physic.inertiatensor[0][0], geom[j]->physic.inertiatensor[1][1], geom[j]->physic.inertiatensor[2][2], geom[j]->physic.inertiatensor[3][3]
			);

			//+1
			fi->cnt += 1;
		}
	}
}

void gravity_force2accel(struct style** geom, int count)
{
	struct forceinfo* fi;
	float mass_inv;
	int j,k;
	for(j=0;j<count;j++){
		fi = &geom[j]->forceinfo;

		vec3 totalforce = {0,0,0};
		for(k=0;k<fi->cnt;k++){
			if(vec3_getlen(fi->force[k]) < 1e-12)continue;
			totalforce[0] += fi->force[k][0];
			totalforce[1] += fi->force[k][1];
			totalforce[2] += fi->force[k][2];
		}

		//N/kg = N/(ton*1000)
		//km/(s^2) = [m/(s^2)]/1000 = [N/kg]/1000 = N/(ton*1000)/1000 = N/(ton*1000000)
		mass_inv = 1.0/(geom[j]->physic.inertiatensor[3][3]*1000000);
		geom[j]->fm.displace_a[0] = totalforce[0] * mass_inv;
		geom[j]->fm.displace_a[1] = totalforce[1] * mass_inv;
		geom[j]->fm.displace_a[2] = totalforce[2] * mass_inv;

		if(DEBUG)logtoall("force2accel j=%d: m=%f, f=%f,%f,%f,%f, a=%f,%f,%f,%f\n",
			j, geom[j]->physic.inertiatensor[3][3],
			totalforce[0], totalforce[1], totalforce[2], vec3_getlen(totalforce),
			geom[j]->fm.displace_a[0], geom[j]->fm.displace_a[1], geom[j]->fm.displace_a[2], vec3_getlen(geom[j]->fm.displace_a));
	}

	for(j=0;j<count;j++){
		geom[j]->forceinfo.cnt = 0;
	}
}

void gravity_acceltime(struct style** geom, int count, float dt)
{
	int j;
	float la,lv,lx;
	struct fmotion* final = 0;
	for(j=0;j<count;j++){
		final = &geom[j]->actual;

		la = vec3_getlen(final->displace_a);
		if(la > 1e-12){
			final->displace_v[0] += final->displace_a[0] * dt;
			final->displace_v[1] += final->displace_a[1] * dt;
			final->displace_v[2] += final->displace_a[2] * dt;
		}

		//displacement
		lv = vec3_getlen(final->displace_v);
		if(lv > 1e-12){
			final->displace_x[0] += final->displace_v[0] * dt;
			final->displace_x[1] += final->displace_v[1] * dt;
			final->displace_x[2] += final->displace_v[2] * dt;
		}

		//writeback position
		lx = vec3_getlen(final->displace_x);
		geom[j]->fs.vc[0] = final->displace_x[0];
		geom[j]->fs.vc[1] = final->displace_x[1];
		geom[j]->fs.vc[2] = final->displace_x[2];

		if(DEBUG)logtoall("acceltime j=%d: v=%f,%f,%f,%f, x=%f,%f,%f,%f\n", j,
			geom[j]->fm.displace_v[0], geom[j]->fm.displace_v[1], geom[j]->fm.displace_v[2], lv,
			geom[j]->fm.displace_x[0], geom[j]->fm.displace_x[1], geom[j]->fm.displace_x[2], lx
		);
	}
}


int gravity_foreach(_obj* ent)
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

	gravity_computeforce(priv->geom, priv->count);

	gravity_force2accel(priv->geom, priv->count);

	gravity_acceltime(priv->geom, priv->count, dt);

	priv->time = now;
	return 0;
}




int gravity_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@gravity_read:%.4s\n",&foot);
	return 0;
}
int gravity_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//logtoall("@gravity_write:%.4s\n",&foot);
	switch(stack[sp-1].foottype){
	case _clk_:
		gravity_foreach(ent);
		break;
	case _ioby_:
		switch(buf[0]){
		case '1':break;
		}
		break;
	}
	return 0;
}
int gravity_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravity_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gravity_reading(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int gravity_writing(_obj* ent,void* foot, p64 arg,int key, u8* buf,int len)
{
	return 0;
}
int gravity_delete(_obj* ent)
{
	return 0;
}
int gravity_create(_obj* ent, void* str)
{
	logtoall("@gravity_create\n");
	return 0;
}
