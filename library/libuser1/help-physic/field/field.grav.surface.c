#include "libuser.h"
#define DEBUG 1


struct privdata{
    u64 time;
	u64 count;
	struct style* geom[6];
};


void graveasy_each(struct style** geom, int count)
{
	if(DEBUG)logtoall("gravity_computeforce enter{\n");

	int j,k;
	struct forceinfo* fi;
	for(j=0;j<count;j++){
		fi = &geom[j]->forceinfo;

		//F point
		fi->where[fi->cnt][0] = geom[j]->fm.displace_x[0];
		fi->where[fi->cnt][1] = geom[j]->fm.displace_x[1];
		fi->where[fi->cnt][2] = geom[j]->fm.displace_x[2];
		fi->force[fi->cnt][0] = 0;
		fi->force[fi->cnt][1] = 0;
		fi->force[fi->cnt][2] = -9.8 * geom[j]->physic.inertiatensor[3][3];
		fi->cnt += 1;
	}
	if(DEBUG)logtoall("}gravity_computeforce leave\n");
}
int graveasy_working(_obj* ent)
{
    struct privdata* priv = (void*)ent->priv_256b;

	u64 now = timeread_us();
	//logtoall("%llx\n", now);
	float dt = (float)((now - priv->time)%1000000)/1000000.0;
	//logtoall("dt=%f\n",dt);

	struct relation* rel = ent->orel0;
	if(0 == rel)return 0;

	_obj* world = rel->pdstchip;
	if((_virtual_ != world->type)&&(_scene3d_ != world->type))return 0;

	priv->count = 0;
	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		priv->geom[priv->count] = rel->psrcfoot;
		priv->count += 1;

		rel = samesrcnextdst(rel);
	}

	graveasy_each(priv->geom, priv->count);

	if(DEBUG)logtoall("\n");

	priv->time = now;
	return 0;
}




int graveasy_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@graveasy_read:%p,%p\n",ent,foot);
	return 0;
}
int graveasy_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@graveasy_give:%p,%p\n",ent,foot);
	switch(stack[sp-1].foottype){
	case _clk_:
		graveasy_working(ent);
		break;
	}
	return 0;
}
int graveasy_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int graveasy_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int graveasy_search()
{
	return 0;
}
int graveasy_modify()
{
	return 0;
}
int graveasy_delete(_obj* ent)
{
	return 0;
}
int graveasy_create(_obj* ent, void* str)
{
	logtoall("@graveasy_create\n");
	return 0;
}
