#include "libuser.h"
#define REL_WORLD buf0
#define REL_DRONE buf1
void quaternion_rotatefrom(float* o, float* v, float* q);




void virtimu_checkplace(struct entity* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _src_, &tmp[0], &tmp[1]);
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
void virtimu_senseforce(struct entity* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	int j;
	float k;
	float tmp[6];
	k = sty->actual.angular_v[3];
	for(j=0;j<3;j++){
		tmp[j+0] = sty->actual.angular_v[j] * k;
		tmp[j+3] = sty->actual.displace_a[j];
	}
	say("world: %f,%f,%f,%f,%f,%f\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5]);

	vec4 quat;
	for(j=0;j<3;j++)quat[j] = -sty->actual.angular_x[j];
	quat[3] = sty->actual.angular_x[3];

	float out[6];
	quaternion_rotatefrom(&out[0], &tmp[0], quat);
	quaternion_rotatefrom(&out[3], &tmp[3], quat);
	say("local: %f,%f,%f,%f,%f,%f\n",out[0],out[1],out[2],out[3],out[4],out[5]);
	give_data_into_peer(ent,_dst_, 0,0, 0,0, out, 6);
}




int virtimu_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@virtimu_read:%p,%p\n", ent, foot);
	return 0;
}
int virtimu_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	//say("@virtimu_write:%.4s\n",&foot);
	if(_clk_ == stack[sp=1].flag){
		virtimu_checkplace(ent);
		virtimu_senseforce(ent);
	}
	return 0;
}
int virtimu_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int virtimu_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int virtimu_search()
{
	return 0;
}
int virtimu_modify()
{
	return 0;
}
int virtimu_delete(struct entity* ent)
{
	return 0;
}
int virtimu_create(struct entity* ent, void* str)
{
	say("@virtimu_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;
	return 0;
}
