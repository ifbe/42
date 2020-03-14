#include "libuser.h"
#define TIME data0
#define FLAG data1




int gravity_effect(struct style* geom, float dt)
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
	return 0;
}
int gravity_working(struct entity* ent)
{
	u64 now;
	float dt;
	struct entity* world;
	struct relation* rel;
	struct style* geom;

	rel = ent->orel0;
	if(0 == rel)return 0;

	world = rel->pdstchip;
	if((_virtual_ != world->fmt)&&(_scene3d_ != world->fmt))return 0;

	now = timeread();
	say("%llx\n", now);
	dt = (float)((now - ent->TIME)%1000000)/1000000.0;
	say("dt=%f\n",dt);
	if(ent->FLAG < 42)ent->FLAG += 1;

	rel = world->orel0;
	while(1){
		if(0 == rel)break;

		geom = rel->psrcfoot;
		gravity_effect(geom, dt);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}




int gravity_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@gravity_read:%.4s\n",&self->flag);
	return 0;
}
int gravity_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@gravity_write:%.4s\n",&self->flag);
	if(_clk_ == self->flag)gravity_working(self->pchip);
	return 0;
}
int gravity_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravity_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gravity_search()
{
	return 0;
}
int gravity_modify()
{
	return 0;
}
int gravity_delete(struct entity* ent)
{
	return 0;
}
int gravity_create(struct entity* ent, void* str)
{
	say("@gravity_create\n");
	ent->FLAG = 0;
	return 0;
}
