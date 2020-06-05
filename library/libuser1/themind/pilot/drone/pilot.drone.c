#include "libuser.h"
#define REL_WORLD buf0
#define REL_DRONE buf1
void quaternion2eulerian(float* q, float* eulerian);




void flycon_pidloop_angle(float* result, float* expect, struct fstyle* fs)
{
	result[0] = arctan2(fs->vf[2], fs->vt[2]);
	result[1] =-arctan2(fs->vf[0], fs->vr[0]);
}
void flycon_pidloop_speed(float* result, float* expect, struct fstyle* fs)
{
}
void flycon_applyforce(struct entity* ent)
{
	struct halfrel* rel = ent->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	struct fstyle* fs = &sty->fs;
	struct ftest* ft = &sty->ft;

	vec3 eulerian;
	flycon_pidloop_angle(eulerian, 0, fs);
	say("eulerian:%f,%f\n", eulerian[0], eulerian[1]);

	float ln = 1.0;
	float rn = 1.0;
	float lf = 1.0;
	float rf = 1.0;
	lf -= eulerian[0] * 0.0001;
	rf -= eulerian[0] * 0.0001;
	ln += eulerian[0] * 0.0001;
	rn += eulerian[0] * 0.0001;
	ln -= eulerian[1] * 0.0001;
	lf -= eulerian[1] * 0.0001;
	rn += eulerian[1] * 0.0001;
	rf += eulerian[1] * 0.0001;

	ft->force[0][0] = fs->vt[0] * ln;
	ft->force[0][1] = fs->vt[1] * ln;
	ft->force[0][2] = fs->vt[2] * ln;
	ft->force[1][0] = fs->vt[0] * rn;
	ft->force[1][1] = fs->vt[1] * rn;
	ft->force[1][2] = fs->vt[2] * rn;
	ft->force[2][0] = fs->vt[0] * lf;
	ft->force[2][1] = fs->vt[1] * lf;
	ft->force[2][2] = fs->vt[2] * lf;
	ft->force[3][0] = fs->vt[0] * rf;
	ft->force[3][1] = fs->vt[1] * rf;
	ft->force[3][2] = fs->vt[2] * rf;

	ft->where[0][0] =-fs->vr[0] -fs->vf[0];
	ft->where[0][1] =-fs->vr[1] -fs->vf[1];
	ft->where[0][2] =-fs->vr[2] -fs->vf[2];
	ft->where[1][0] = fs->vr[0] -fs->vf[0];
	ft->where[1][1] = fs->vr[1] -fs->vf[1];
	ft->where[1][2] = fs->vr[2] -fs->vf[2];
	ft->where[2][0] =-fs->vr[0] +fs->vf[0];
	ft->where[2][1] =-fs->vr[1] +fs->vf[1];
	ft->where[2][2] =-fs->vr[2] +fs->vf[2];
	ft->where[3][0] = fs->vr[0] +fs->vf[0];
	ft->where[3][1] = fs->vr[1] +fs->vf[1];
	ft->where[3][2] = fs->vr[2] +fs->vf[2];
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
	say("@flycon_write:%.4s\n",&foot);
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
	return 0;
}
