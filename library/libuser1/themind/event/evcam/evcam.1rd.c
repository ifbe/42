#include "libuser.h"
#define _tar_ hex32('t','a','r',0)




struct privdata{
	struct entity* cam;
	struct entity* tar;

	struct style* caminworld;
	struct style* tarinworld;

	int mousedown_x;
	int mousedown_y;
	int mousedown_z;
	int mousedown_flag;
	int mousemove_x;
	int mousemove_y;
	int mousemove_z;
	int mousemove_flag;

	int touchdown[4];
	int touchmove[4];
};




void* cam1rd_findgeom(struct entity* ent)
{
	struct relation* rel;
	struct entity* world;
	if(0 == ent)return 0;

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(_virtual_ == world->type)return rel->psrcfoot;
		if(_scene3d_ == world->type)return rel->psrcfoot;
		rel = samedstnextsrc(rel);
	}

	rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		world = rel->pdstchip;
		if(_virtual_ == world->type)return rel->pdstfoot;
		if(_scene3d_ == world->type)return rel->pdstfoot;
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int cam1rd_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct privdata* own = ent->buf0;
	if(0 == own)return 0;

	struct style* cam = own->caminworld;
	if(0 == cam){
		cam = cam1rd_findgeom(own->cam);
		if(0 == cam)return 0;

		own->caminworld = cam;
	}

	struct style* tar = own->tarinworld;
	if(0 == tar){
		tar = cam1rd_findgeom(own->tar);
		if(0 == tar)return 0;

		own->tarinworld = tar;
	}

	cam->fs.vc[0] = tar->fs.vc[0] + tar->fs.vt[0];
	cam->fs.vc[1] = tar->fs.vc[1] + tar->fs.vt[1];
	cam->fs.vc[2] = tar->fs.vc[2] + tar->fs.vt[2];

	cam->fs.vf[0] = tar->fs.vf[0];
	cam->fs.vf[1] = tar->fs.vf[1];
	cam->fs.vf[2] = tar->fs.vf[2];
	vec3_normalize(cam->fs.vf);

	cam->fs.vr[0] = tar->fs.vr[0];
	cam->fs.vr[1] = tar->fs.vr[1];
	cam->fs.vr[2] = tar->fs.vr[2];
	vec3_normalize(cam->fs.vr);

	cam->fs.vt[0] = tar->fs.vt[0];
	cam->fs.vt[1] = tar->fs.vt[1];
	cam->fs.vt[2] = tar->fs.vt[2];
	return 0;
}
int cam1rd_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, struct event* ev, int len)
{
	return 0;
}
int cam1rd_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@cam1rd_discon\n");
	return 0;
}
int cam1rd_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@cam1rd_linkup\n");
	struct entity* ent = self->pchip;
	struct privdata* own = ent->buf0;
	switch(self->flag){
	case _cam_:own->cam = peer->pchip;break;
	case _tar_:own->tar = peer->pchip;break;
	}
	return 0;
}




int cam1rd_search(struct entity* win)
{
	return 0;
}
int cam1rd_modify(struct entity* win)
{
	return 0;
}
int cam1rd_delete(struct entity* win)
{
	return 0;
}
int cam1rd_create(struct entity* act, void* flag)
{
	struct privdata* own = act->buf0 = memorycreate(0x1000, 0);
	return 0;
}
