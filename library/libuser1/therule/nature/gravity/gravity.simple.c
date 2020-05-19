#include "libuser.h"
#define TIME data0
#define FLAG data1




int gravity_effect(struct style* geom, float dt)
{
	struct fmotion* final = &geom->fm;

	//a = -9.8, v -= 9.8*dt, x += v*dt
	final->a_displace[2] = -9.8;
	final->v_displace[2] -= 9.8 * dt;
	final->x_displace[2] = geom->fs.vc[2] + final->v_displace[2] * dt;

	//if collide
	if(final->x_displace[2] < 0.0){
		final->x_displace[2] = 0.00001;
		final->v_displace[2] = -0.5 * final->v_displace[2];
	}
	say("%f,%f\n", final->v_displace[2], final->x_displace[2]);

	geom->fs.vc[2] = final->x_displace[2];
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




int gravity_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@gravity_read:%.4s\n",&foot);
	return 0;
}
int gravity_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@gravity_write:%.4s\n",&foot);
	if(_clk_ == foot)gravity_working(ent);
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
