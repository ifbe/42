#include "libuser.h"
#define DEBUG 0
#define REL_WORLD listptr.buf0
#define REL_DRONE listptr.buf1
void quaternion_rotatefrom(float* o, float* v, float* q);




void virtimu_checkplace(_obj* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _src_, &tmp[0], &tmp[1]);
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
void virtimu_senseforce(_obj* ent, float* out)
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
	if(DEBUG)say("world: %f,%f,%f,%f,%f,%f\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5]);

	vec4 quat;
	for(j=0;j<3;j++)quat[j] = -sty->actual.angular_x[j];
	quat[3] = sty->actual.angular_x[3];

	quaternion_rotatefrom(&out[0], &tmp[0], quat);
	quaternion_rotatefrom(&out[3], &tmp[3], quat);
	say("local: %f,%f,%f,%f,%f,%f\n",out[0],out[1],out[2],out[3],out[4],out[5]);
}




int virtimu_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	say("@virtimu_read:%p,%p\n", ent, foot);
	return 0;
}
int virtimu_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//say("@virtimu_write:%.4s\n",&foot);
	if(_clk_ == stack[sp=1].foottype){
		float out[6];
		virtimu_checkplace(ent);
		virtimu_senseforce(ent, out);
		give_data_into_peer(ent,_dst_, stack,sp, 0,0, out, 6);
	}
	return 0;
}
int virtimu_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int virtimu_attach(struct halfrel* self, struct halfrel* peer)
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
int virtimu_delete(_obj* ent)
{
	return 0;
}
int virtimu_create(_obj* ent, void* str)
{
	say("@virtimu_create\n");
	ent->REL_WORLD = 0;
	ent->REL_DRONE = 0;
	return 0;
}
