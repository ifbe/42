#include "libuser.h"




int gravity_search()
{
	return 0;
}
int gravity_modify()
{
	return 0;
}
int gravity_delete(struct supply* win)
{
	return 0;
}
int gravity_create(struct supply* win, void* str)
{
	say("@gravity_create\n");
	return 0;
}




int gravity_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	u64 now;
	float dt;
	struct supply* sup;
	struct entity* world;
	struct relation* rel;
	struct style* geom;
	say("@gravity_read\n");

	sup = self->pchip;
	if(0 == sup)return 0;

	rel = sup->orel0;
	if(0 == rel)return 0;

	world = rel->pdstchip;
	if((_world3d_ != world->fmt)&&(_scene3d_ != world->fmt))return 0;

	now = timeread();
	say("%llx\n", now);
	dt = (float)((now - sup->data0)%1000000)/1000000.0;
	say("dt=%f\n",dt);

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		geom = rel->psrcfoot;
		geom->fs.vc[2] -= 10*dt;
		say("%f\n", geom->fs.vc[2]);

		rel = samesrcnextdst(rel);
	}

	sup->data0 = now;
	return 0;
}
int gravity_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@gravity_write\n");
	return 0;
}
int gravity_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravity_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}