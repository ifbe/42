#include "libuser.h"
#define DEBUG 0
void quaternion2matthree(float* q, float* m);


struct privdata{
    void* REL_WORLD;
    void* REL_ROCKET;
	void* ENT_DBGUI;
};


//0: check
void planecontrol_check(_obj* ent)
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
void planecontrol_readsensor(_obj* ent)
{
}
void planecontrol_sensor2state(_obj* ent)
{
}


//2: desire
void planecontrol_state2desire(_obj* ent)
{
}

//3: pid
void planecontrol_calcpid(_obj* ent)
{
}




void planecontrol_report(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct halfrel* rel = priv->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

	_obj* dbg = priv->ENT_DBGUI;
	if(0 == dbg)return;

	//logtoall("%s\n",__func__);
	dbg->onwriter(dbg, 0, 0, 0, sty, 0);
}




int planecontrol_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//if(DEBUG)logtoall("@planecontrol_take:%.4s\n",&foot);
	return 0;
}
int planecontrol_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//if(DEBUG)logtoall("@planecontrol_give:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
        //0
		planecontrol_check(ent);

        //1: sensor algo

        //2: set desire
        planecontrol_state2desire(ent);

        //3: pid
		planecontrol_calcpid(ent);

		//
		planecontrol_report(ent);
	}
	return 0;
}
int planecontrol_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int planecontrol_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	_obj* dbg = peer->pchip;
	logtoall("%s: %.4s, %.8s\n", __func__, &self->foottype, &dbg->type);

    struct privdata* priv = (void*)ent->priv_256b;
	switch(self->foottype){
	case _dbg_:
		priv->ENT_DBGUI = dbg;
		break;
	}
	return 0;
}




int planecontrol_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int planecontrol_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int planecontrol_delete(_obj* ent)
{
	return 0;
}
int planecontrol_create(_obj* ent, void* str)
{
	logtoall("@planecontrol_create\n");
	return 0;
}
