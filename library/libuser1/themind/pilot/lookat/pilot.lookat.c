#include "libuser.h"
#define _self_ hex32('s','e','l','f')
#define _that_ hex32('t','h','a','t')




struct privdata{
void* that;
void* thatgeom;
void* self;
void* selfgeom;
};
void* lookat_where(struct entity* ent)
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
void lookat_doit(struct fstyle* this, struct fstyle* that)
{
	say("at(%f,%f,%f), to(%f,%f,%f)\n",
		this->vc[0], this->vc[1], this->vc[2],
		that->vc[0], that->vc[1], that->vc[2]
	);

	float x = that->vc[0] - this->vc[0];
	float y = that->vc[1] - this->vc[1];
	float z = that->vc[2] - this->vc[2];
	float dist = squareroot(x*x + y*y + z*z);
	if(dist < 1.0)return;

	float lenf = vec3_getlen(this->vf);
	this->vf[0] = x;
	this->vf[1] = y;
	this->vf[2] = z;
	vec3_setlen(this->vf, lenf);

	float lenr = vec3_getlen(this->vr);
	this->vr[0] = y;
	this->vr[1] =-x;
	this->vr[2] = z;
	vec3_setlen(this->vr, lenr);
}




int lookat_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int lookat_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	say("@lookat_write:%.4s\n",&foot);
	if(_clk_ == foot){
		struct privdata* own = ent->buf0;
		if(0 == own->that)return 0;
		if(0 == own->self)return 0;

		if(0 == own->thatgeom)own->thatgeom = lookat_where(own->that);
		if(0 == own->thatgeom)return 0;

		if(0 == own->selfgeom)own->selfgeom = lookat_where(own->self);
		if(0 == own->selfgeom)return 0;

		lookat_doit(own->selfgeom, own->thatgeom);
	}
	return 0;
}
int lookat_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@lookat_discon\n");
	return 0;
}
int lookat_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	struct privdata* own = ent->buf0;
	say("@lookat_linkup: %.4s\n", &self->flag);

    switch(self->flag){
    case _self_:own->self = peer->pchip;break;
    case _that_:own->that = peer->pchip;break;
    }
	return 0;
}




int lookat_search(struct entity* win)
{
	return 0;
}
int lookat_modify(struct entity* win)
{
	return 0;
}
int lookat_delete(struct entity* win)
{
	return 0;
}
int lookat_create(struct entity* act, void* flag)
{
	struct privdata* own = act->buf0 = memorycreate(0x1000, 0);
	own->self = 0;
	own->selfgeom = 0;
	own->that = 0;
	own->thatgeom = 0;
	return 0;
}
