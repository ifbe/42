#include "libuser.h"
#define _tar_ hex32('t','a','r',0)
#define _geom_ hex32('g','e','o','m')




int event3rd_movetar(struct style* camgeom, struct style* targeom, int dx, int dy)
{
	float* tar = targeom->fs.vc;
	tar[0] += dx;
	tar[1] += dy;

	float* cam = camgeom->fs.vc;
	cam[0] += dx;
	cam[1] += dy;
	return 0;
}
int event3rd_movecam(struct style* camgeom, struct style* targeom, int dx, int dy)
{
	int j;
	vec3 tartocam;
	float* tar = targeom->fs.vc;
	float* cam = camgeom->fs.vc;
	for(j=0;j<3;j++)tartocam[j] = cam[j] - tar[j];

	float a = dx*PI/1800;
	float c = cosine(a);
	float s = sine(a);
	cam[0] = tar[0] + tartocam[0]*c + tartocam[1]*s;
	cam[1] = tar[1] - tartocam[0]*s + tartocam[1]*c;

	for(j=0;j<3;j++)camgeom->fs.vf[j] = tar[j] - cam[j];
	vec3_cross(camgeom->fs.vr, camgeom->fs.vf, camgeom->fs.vt);
	return 0;
}
void* event3rd_find(struct entity* handler)
{
	int ret;
	struct halfrel* self;
	struct halfrel* peer;

	//from handler find target
	ret = relationsearch(handler, _tar_, &self, &peer);
	if(0 == ret)return 0;
	struct entity* tar = peer->pchip;
	if(0 == tar)return 0;

	//from target find geom
	ret = relationsearch(tar, _geom_, &self, &peer);
	if(0 == ret)return 0;

	return peer->pfoot;
}




int event3rd_read(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event3rd_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct event* ev, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;

	if(0x2d70 == ev->what){
		ent->iw0 = 0;
		return 0;
	}
	if(0x2b70 == ev->what){
		short* ch = (void*)ev;
		ent->ix0 = ch[0];
		ent->iy0 = ch[1];
		ent->iw0 = 1;
		return 0;
	}
	if(0x4070 == ev->what){
		if(0 == ent->iw0)return 0;

		struct style* camgeom = stack[rsp-1]->pfoot;
		if(0 == camgeom)return 0;
		struct style* targeom = event3rd_find(ent);
		if(0 == targeom)return 0;

		short* ch = (void*)ev;
		event3rd_movecam(camgeom, targeom, ch[0] - ent->ix0, ch[1] - ent->iy0);
		return 0;
	}
	if(_char_ == ev->what){
		struct style* camgeom = stack[rsp-1]->pfoot;
		if(0 == camgeom)return 0;
		struct style* targeom = event3rd_find(ent);
		if(0 == targeom)return 0;
		int dx=0,dy=0;
		if('a' == ev->why)dx =-100;
		if('d' == ev->why)dx = 100;
		if('s' == ev->why)dy =-100;
		if('w' == ev->why)dy = 100;
		event3rd_movetar(camgeom, targeom, dx, dy);
		return 0;
	}
	return 0;
}
int event3rd_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@event3rd_discon\n");
	return 0;
}
int event3rd_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@event3rd_linkup\n");
	return 0;
}




int event3rd_search(struct entity* win)
{
	return 0;
}
int event3rd_modify(struct entity* win)
{
	return 0;
}
int event3rd_delete(struct entity* win)
{
	return 0;
}
int event3rd_create(struct entity* act, void* flag)
{
	return 0;
}
