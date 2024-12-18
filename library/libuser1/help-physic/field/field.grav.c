#include "libsoft.h"
#define DEBUG 0
#define DEBUG_DETAIL 0
float gravity_f32(float m1,float m2,float r);
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


void gravity_computeforce(struct style** geom, int count)
{
	float len;
	float Mself,Mpeer;
	struct forceinfo* fi;
	if(DEBUG)logtoall("gravity_computeforce enter{\n");

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
	if(DEBUG)logtoall("}gravity_computeforce leave\n");
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

	if(DEBUG)logtoall("\n");

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
