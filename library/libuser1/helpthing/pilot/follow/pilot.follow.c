#include "libuser.h"
#define _self_ hex32('s','e','l','f')
#define _that_ hex32('t','h','a','t')




struct privdata{
void* that;
void* thatgeom;
void* self;
void* selfgeom;
};
void* follow_where(_obj* ent)
{
	struct relation* rel;
	_obj* world;

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(_virtual_ == world->type)return rel->psrcfoot;
		rel = samedstnextsrc(rel);
	}
	return 0;
}
void follow_doit(struct fstyle* this, struct fstyle* that)
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

	if(dist < 10000.0)return;
	dist = 10000/dist;
	this->vc[0] = that->vc[0] - x*dist;
	this->vc[1] = that->vc[1] - y*dist;
	this->vc[2] = that->vc[2] - z*dist;
}




int follow_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int follow_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	say("@follow_write:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
		struct privdata* own = ent->priv_ptr;
		if(0 == own->that)return 0;
		if(0 == own->self)return 0;

		if(0 == own->thatgeom)own->thatgeom = follow_where(own->that);
		if(0 == own->selfgeom)own->selfgeom = follow_where(own->self);

		struct fstyle* thatgeom = own->thatgeom;
		if(0 == thatgeom)return 0;
		struct fstyle* selfgeom = own->selfgeom;
		if(0 == selfgeom)return 0;

		follow_doit(own->selfgeom, own->thatgeom);
	}
	return 0;
}
int follow_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@follow_detach\n");
	return 0;
}
int follow_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	struct privdata* own = ent->priv_ptr;
	say("@follow_attach: %.4s\n", &self->foottype);

	switch(self->foottype){
	case _self_:own->self = peer->pchip;break;
	case _that_:own->that = peer->pchip;break;
	}
	return 0;
}




int follow_search(_obj* win)
{
	return 0;
}
int follow_modify(_obj* win)
{
	return 0;
}
int follow_delete(_obj* win)
{
	return 0;
}
int follow_create(_obj* act, void* flag)
{
	struct privdata* own = act->priv_ptr = memorycreate(0x1000, 0);
	own->self = 0;
	own->selfgeom = 0;
	own->that = 0;
	own->thatgeom = 0;
	return 0;
}
