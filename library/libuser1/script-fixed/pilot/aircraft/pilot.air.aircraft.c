#include "libuser.h"
#define DEBUG 1
void quaternion2matthree(float* q, float* m);


struct privdata{
	//
	_obj* aircraft_obj;
    void* aircraft_geom;
	//
	void* dbgui;
};


//0: check
void planecontrol_check(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;
	_obj* world;

	_obj* satellite = priv->aircraft_obj;
	if(0 == satellite)return;
	struct relation* rel = satellite->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->type) | (_scene3d_ != world->type)){
			priv->aircraft_geom = rel->psrcfoot;
			//logtoall("sate.geom=%p\n", priv->satellite_geom);
			break;
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
    struct privdata* priv = (void*)ent->priv_256b;

	struct style* sty = priv->aircraft_geom;
	if(0 == sty)return;
    if(DEBUG)logtoall("xx:%f,%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2],vec3_getlen(sty->fm.displace_x));
    if(DEBUG)logtoall("xv:%f,%f,%f,%f\n",sty->fm.displace_v[0],sty->fm.displace_v[1],sty->fm.displace_v[2],vec3_getlen(sty->fm.displace_v));

	int j;
	struct forceinfo* fi = &sty->forceinfo;
	for(j=0;j<3;j++){
		fi->where[fi->cnt][j] = sty->fmotion.displace_x[j] + sty->fshape.vr[j] - sty->fshape.vf[j] + sty->fshape.vt[j]/10;
		fi->force[fi->cnt][j] = sty->fshape.vf[j];
	}
	fi->cnt++;
}




void planecontrol_report(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct style* geom = priv->aircraft_geom;
	if(0 == geom)return;

	_obj* dbg = priv->dbgui;
	if(0 == dbg)return;

	//logtoall("%s\n",__func__);
	dbg->onwriter(dbg, 0, 0, 0, geom, 0);
}




int planecontrol_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//if(DEBUG)logtoall("@planecontrol_take:%.4s\n",&foot);
	return 0;
}
int planecontrol_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	if(DEBUG)logtoall("@planecontrol_give:%.4s\n",&foot);
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
	_obj* obj = peer->pchip;
	logtoall("%s: %.8s.%.4s -> %.8s.%.4s\n", __func__, &ent->type, &self->foottype, &obj->type, &peer->foottype);

    struct privdata* priv = (void*)ent->priv_256b;
	switch(self->foottype){
	case _dbg_:
		priv->dbgui = obj;
		logtoall("dbgui@%p\n", obj);
		break;
	case _dst_:
		priv->aircraft_obj = obj;
		logtoall("sate@%p\n", obj);
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
