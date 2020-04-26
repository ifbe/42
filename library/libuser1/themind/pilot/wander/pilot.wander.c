#include "libuser.h"
#define _self_ hex32('s','e','l','f')




struct privdata{
void* self;
void* selfgeom;
//origin position
float x0;
float y0;
float z0;
float t0;
//target position
float x;
float y;
float z;
float t;
};
void* wander_where(struct entity* ent)
{
	struct relation* rel;
	struct entity* world;

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(_virtual_ == world->type)return rel->psrcfoot;
		rel = samedstnextsrc(rel);
	}
	return 0;
}
void wander_doit(struct privdata* own, struct fstyle* this)
{
	float dx = own->x - this->vc[0];
	float dy = own->y - this->vc[1];
	float dz = own->z - this->vc[2];
	float dist = squareroot(dx*dx + dy*dy + dz*dz);
	if(dist < 20.0){
		own->x = own->x0 + (float)(getrandom()&0xfff);
		own->y = own->y0 + (float)(getrandom()&0xfff);
		own->z = own->z0;

		dx = own->x - this->vc[0];
		dy = own->y - this->vc[1];
		dz = own->z - this->vc[2];
		dist = squareroot(dx*dx + dy*dy + dz*dz);
	}

	say("at(%f,%f,%f), to(%f,%f,%f)\n",
		this->vc[0], this->vc[1], this->vc[2],
		own->x, own->y, own->z
	);

	float lenf = vec3_getlen(this->vf);
	this->vf[0] = dx;
	this->vf[1] = dy;
	this->vf[2] = dz;
	vec3_setlen(this->vf, lenf);

	float lenr = vec3_getlen(this->vr);
	this->vr[0] = dy;
	this->vr[1] =-dx;
	this->vr[2] = dz;
	vec3_setlen(this->vr, lenr);

	this->vc[0] += 10*dx/dist;
	this->vc[1] += 10*dy/dist;
	//this->vc[2] += dz/dist;
}




int wander_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int wander_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	say("@wander_write:%.4s\n",&foot);
	if(_clk_ == foot){
		struct privdata* own = ent->buf0;
		if(0 == own->self)return 0;

		struct fstyle* geom = own->selfgeom;
		if(0 == geom){
			geom = own->selfgeom = wander_where(own->self);
			if(0 == geom)return 0;

			own->x = own->x0 = geom->vc[0];
			own->y = own->y0 = geom->vc[1];
			own->z = own->z0 = geom->vc[2];
		}

		wander_doit(own, geom);
	}
	return 0;
}
int wander_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@wander_discon\n");
	return 0;
}
int wander_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	struct privdata* own = ent->buf0;
	say("@wander_linkup: %.4s\n", &self->flag);

    switch(self->flag){
    case _self_:own->self = peer->pchip;break;
    }
	return 0;
}




int wander_search(struct entity* win)
{
	return 0;
}
int wander_modify(struct entity* win)
{
	return 0;
}
int wander_delete(struct entity* win)
{
	return 0;
}
int wander_create(struct entity* act, void* flag)
{
	struct privdata* own = act->buf0 = memorycreate(0x1000, 0);
	own->self = 0;
	own->selfgeom = 0;
	return 0;
}
