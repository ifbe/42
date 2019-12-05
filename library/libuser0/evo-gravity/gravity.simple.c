#include "libuser.h"




void gravity_effect(struct style* geom, float dt)
{
	struct fmotion delta;
	struct fmotion* added = &geom->fm;

	//delta.j_displace[2] = 0;
	//delta.a_displace[2] = 0;
	delta.v_displace[2] = -9.8 * dt;
	delta.x_displace[2] = added->v_displace[2] * dt;

	//added->j_displace[2] += delta.j_displace[2];
	//added->a_displace[2] += delta.a_displace[2];
	added->v_displace[2] += delta.v_displace[2];
	added->x_displace[2] += delta.x_displace[2];

	if(geom->fs.vc[2] + delta.x_displace[2] > 0.0)geom->fs.vc[2] += delta.x_displace[2];
	say("%f,%f,%f,%f\n", added->v_displace[2], added->x_displace[2], delta.x_displace[2], geom->fs.vc[2]);
}




int gravity_search()
{
	return 0;
}
int gravity_modify()
{
	return 0;
}
int gravity_delete(struct supply* sup)
{
	return 0;
}
int gravity_create(struct supply* sup, void* str)
{
	say("@gravity_create\n");
	sup->data3 = 0;
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
	if(sup->data3 < 42)sup->data3 += 1;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		geom = rel->psrcfoot;
		gravity_effect(geom, dt);

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