#include "libuser.h"
#define DEBUG 0
void quaternion2matthree(float* q, float* m);


struct privdata{
    void* REL_WORLD;
    void* REL_ROCKET;
	void* ENT_DBGUI;
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

//masscenter, localtoworldmat, localspaceshape -> output
void rocket_compute_position(float* cm, float (*m)[3], struct fstyle* s, float* p)
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
void rocket_compute_direction(float (*m)[3], struct fstyle* fs, float angle, float* d)
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
void rocketcontrol_calcpid(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	struct halfrel* rel = priv->REL_WORLD;
	if(0 == rel)return;

	struct style* sty = rel->pfoot;
	if(0 == sty)return;

    if(DEBUG)logtoall("x:%f,%f,%f,%f\n",sty->fm.displace_x[0],sty->fm.displace_x[1],sty->fm.displace_x[2],vec3_getlen(sty->fm.displace_x));
    if(DEBUG)logtoall("v:%f,%f,%f,%f\n",sty->fm.displace_v[0],sty->fm.displace_v[1],sty->fm.displace_v[2],vec3_getlen(sty->fm.displace_v));

	//q to m
	float* q = sty->fm.angular_x;
	mat3 m;
	quaternion2matthree(q, &m[0][0]);

	//where = mass center
	struct forceinfo* fi = &sty->forceinfo;
	//fi->where[fi->cnt][0] = sty->fm.displace_x[0];
	//fi->where[fi->cnt][1] = sty->fm.displace_x[1];
	//fi->where[fi->cnt][2] = sty->fm.displace_x[2];
	rocket_compute_position(sty->fm.displace_x, m, &sty->fshape, fi->where[fi->cnt]);

	//direction = bias 7 to 8 degree = vr*sin8 + vt*cos8
	//fi->force[fi->cnt][0] = sty->fm.displace_x[0];
	//fi->force[fi->cnt][1] = sty->fm.displace_x[1];
	//fi->force[fi->cnt][2] = sty->fm.displace_x[2];
	u64 time = timeread_ms();
	float bias = 10 * getsin(tau * (time%60000)/60000);
	rocket_compute_direction(m, &sty->fshape, bias*PI/180, fi->force[fi->cnt]);

	//strength = mass_kg * 14 = mass_ton*1000*14
	float fval = sty->physic.inertiatensor[3][3] * 14 * 1000;
	vec3_setlen(fi->force[fi->cnt], fval);

	fi->cnt += 1;
}




void rocketcontrol_report(_obj* ent)
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




int rocketcontrol_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//if(DEBUG)logtoall("@rocketcontrol_take:%.4s\n",&foot);
	return 0;
}
int rocketcontrol_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	//if(DEBUG)logtoall("@rocketcontrol_give:%.4s\n",&foot);
	if(_clk_ == stack[sp-1].foottype){
        //0
		rocketcontrol_check(ent);

        //1: sensor algo

        //2: set desire
        rocketcontrol_state2desire(ent);

        //3: pid
		rocketcontrol_calcpid(ent);

		//
		rocketcontrol_report(ent);
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

    struct privdata* priv = (void*)ent->priv_256b;
	switch(self->foottype){
	case _dbg_:
		priv->ENT_DBGUI = dbg;
		break;
	}
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
