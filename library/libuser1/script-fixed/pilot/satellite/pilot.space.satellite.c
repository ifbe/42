#include "libuser.h"
#define DEBUG 1
void quaternion2matthree(float* q, float* m);


struct privdata{
	//earth
	_obj* earth_obj;
    void* earth_geom;
	//satellite
	_obj* satellite_obj;
    void* satellite_geom;
	//debug
	_obj* dbgui;
};


//0: check
void satellitecontrol_check(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;
	_obj* world;

	_obj* satellite = priv->satellite_obj;
	if(0 == satellite)return;
	struct relation* rel = satellite->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->type) | (_scene3d_ != world->type)){
			priv->satellite_geom = rel->psrcfoot;
			//logtoall("sate.geom=%p\n", priv->satellite_geom);
			break;
		}
		rel = samedstnextsrc(rel);
	}

	_obj* earth = priv->earth_obj;
	if(0 == earth)return;
	rel = earth->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(	(_virtual_ != world->type) | (_scene3d_ != world->type)){
			priv->earth_geom = rel->psrcfoot;
			//logtoall("earth.geom=%p\n", priv->earth_geom);
			break;
		}
		rel = samedstnextsrc(rel);
	}
}


//1: sensor
void satellitecontrol_readsensor(_obj* ent)
{
}
void satellitecontrol_sensor2state(_obj* ent)
{
}


//2: desire
void satellitecontrol_state2desire(_obj* ent)
{
/*
    struct privdata* priv = (void*)ent->priv_256b;

	struct halfrel* rel = priv->world_satellite;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;
    logtoall("xyz:%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2]);
*/
}

//masscenter, localtoworldmat, localspaceshape -> output
void compute_position(float* cm, float (*m)[3], struct fstyle* s, float* p)
{
	//vec3 vr = {m[0][0], m[0][1], m[0][2]};
	//vec3 vf = {m[1][0], m[1][1], m[1][2]};
	vec3 vt = {m[2][0], m[2][1], m[2][2]};

	//float lenr = vec3_getlen(s->vr);
	//float lenf = vec3_getlen(s->vf);
	float lent = vec3_getlen(s->vt);

	p[0] = cm[0] - vt[0]*lent;
	p[1] = cm[1] - vt[1]*lent;
	p[2] = cm[2] - vt[2]*lent;
}

//localtoworldmat, localspaceshape, angle -> output
void compute_direction(float (*m)[3], struct fstyle* fs, float angle, float* d)
{
	float c = getcos(angle);
	float s = getsin(angle);

	vec3 vr = {m[0][0], m[0][1], m[0][2]};
	vec3 vf = {m[1][0], m[1][1], m[1][2]};
	vec3 vt = {m[2][0], m[2][1], m[2][2]};

	d[0] = vr[0]*s + vt[0]*c;
	d[1] = vr[1]*s + vt[1]*c;
	d[2] = vr[2]*s + vt[2]*c;
}

//3: pid
void satellitecontrol_calcpid(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct style* sty_earth = priv->earth_geom;
	if(0 == sty_earth)return;
    if(DEBUG)logtoall("earth.x:%f,%f,%f,%f\n",sty_earth->fm.displace_x[0],sty_earth->fm.displace_x[1],sty_earth->fm.displace_x[2],vec3_getlen(sty_earth->fm.displace_x));
    if(DEBUG)logtoall("earth.v:%f,%f,%f,%f\n",sty_earth->fm.displace_v[0],sty_earth->fm.displace_v[1],sty_earth->fm.displace_v[2],vec3_getlen(sty_earth->fm.displace_v));

	struct style* sty = priv->satellite_geom;
	if(0 == sty)return;
    if(DEBUG)logtoall("sate.x:%f,%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2],vec3_getlen(sty->fm.displace_x));
    if(DEBUG)logtoall("sate.v:%f,%f,%f,%f\n",sty->fm.displace_v[0],sty->fm.displace_v[1],sty->fm.displace_v[2],vec3_getlen(sty->fm.displace_v));

	//q to m
	float* q = sty->fm.angular_x;
}




void satellitecontrol_report(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct style* geom = priv->satellite_geom;
	if(0 == geom)return;

	_obj* dbg = priv->dbgui;
	if(0 == dbg)return;

	//logtoall("%s\n",__func__);
	dbg->onwriter(dbg, 0, 0, 0, geom, 0);
}




int satellitecontrol_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//if(DEBUG)logtoall("@satellitecontrol_take:%.4s\n",&foot);
	return 0;
}
int satellitecontrol_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//if(DEBUG)logtoall("@satellitecontrol_give:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
        //0
		satellitecontrol_check(ent);

        //1: sensor algo

        //2: set desire
        satellitecontrol_state2desire(ent);

        //3: pid
		satellitecontrol_calcpid(ent);

		//
		satellitecontrol_report(ent);
	}
	return 0;
}
int satellitecontrol_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int satellitecontrol_attach(struct halfrel* self, struct halfrel* peer)
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
		priv->satellite_obj = obj;
		logtoall("sate@%p\n", obj);
		break;
	case hex32('e','a','r','t'):
		priv->earth_obj = obj;
		logtoall("earth@%p\n", obj);
		break;
	}
	return 0;
}




int satellitecontrol_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int satellitecontrol_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int satellitecontrol_delete(_obj* ent)
{
	return 0;
}
int satellitecontrol_create(_obj* ent, void* str)
{
	logtoall("@satellitecontrol_create\n");
	return 0;
}
