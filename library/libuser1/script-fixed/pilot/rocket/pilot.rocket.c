#include "libuser.h"
#define DEBUG 0


struct privdata{
    void* REL_WORLD;
    void* REL_ROCKET;
};


//0: check
void rocketcontrol_check(_obj* ent)
{
	struct halfrel* tmp[2];
	int ret = relationsearch(ent, _dst_, &tmp[0], &tmp[1]);
	if(ret <= 0)return;

	_obj* rocket = tmp[1]->pchip;
	if(0 == rocket)return;

    struct privdata* priv = (void*)ent->priv_256b;

	_obj* world;
	struct relation* rel = rocket->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->type) | (_scene3d_ != world->type)){
			priv->REL_WORLD = rel->src;
			priv->REL_ROCKET = rel->dst;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}


//1: sensor
void rocketcontrol_readsensor(_obj* ent)
{
}
void rocketcontrol_sensor2state(_obj* ent)
{
}


//2: desire
void rocketcontrol_state2desire(_obj* ent)
{
/*
    struct privdata* priv = (void*)ent->priv_256b;

	struct halfrel* rel = priv->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;
    logtoall("xyz:%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2]);
*/
}


//3: pid
void rocketcontrol_calcpid(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct halfrel* rel = priv->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

    //logtoall("x:%f,%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2],vec3_getlen(sty->fm.displace_x));
    //logtoall("v:%f,%f,%f,%f\n",sty->fm.displace_v[0],sty->fm.displace_v[1],sty->fm.displace_v[2],vec3_getlen(sty->fm.displace_v));

	struct forceinfo* fi = &sty->forceinfo;

	fi->force[fi->cnt][0] = sty->fm.displace_x[0];
	fi->force[fi->cnt][1] = sty->fm.displace_x[1];
	fi->force[fi->cnt][2] = sty->fm.displace_x[2];
	vec3_setlen(fi->force[fi->cnt], sty->physic.inertiatensor[3][3] * 14 * 1000);

	fi->cnt += 1;
}




int rocketcontrol_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(DEBUG)logtoall("@rocketcontrol_take:%.4s\n",&foot);
	return 0;
}
int rocketcontrol_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	if(DEBUG)logtoall("@rocketcontrol_give:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
        //0
		rocketcontrol_check(ent);

        //1: sensor algo

        //2: set desire
        rocketcontrol_state2desire(ent);

        //3: pid
		rocketcontrol_calcpid(ent);
	}
	return 0;
}
int rocketcontrol_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rocketcontrol_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	_obj* dbg = peer->pchip;
	logtoall("%s: %.4s, %.8s\n", __func__, &self->foottype, &dbg->type);

	return 0;
}




int rocketcontrol_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int rocketcontrol_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int rocketcontrol_delete(_obj* ent)
{
	return 0;
}
int rocketcontrol_create(_obj* ent, void* str)
{
	logtoall("@rocketcontrol_create\n");
	return 0;
}
