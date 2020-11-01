#include "libuser.h"
#define TIME data0
#define FLAG data1




int graveasy_effect(struct style* geom, float dt)
{
	struct fmotion* final = &geom->fm;

	//a = -9.8, v -= 9.8*dt, x += v*dt
	final->displace_a[2] = -9.8;
	final->displace_v[2] -= 9.8 * dt;
	final->displace_x[2] = geom->fs.vc[2] + final->displace_v[2] * dt;

	//if collide
	if(final->displace_x[2] < 0.0){
		final->displace_x[2] = 0.00001;
		final->displace_v[2] = -0.5 * final->displace_v[2];
	}
	say("%f,%f\n", final->displace_v[2], final->displace_x[2]);

	geom->fs.vc[2] = final->displace_x[2];
	return 0;
}
int graveasy_working(struct entity* ent)
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
		graveasy_effect(geom, dt);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}




int graveasy_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@graveasy_read:%p,%p\n",ent,foot);
	return 0;
}
int graveasy_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@graveasy_give:%p,%p\n",ent,foot);
	if(_clk_ == stack[sp-1].flag)graveasy_working(ent);
	return 0;
}
int graveasy_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int graveasy_linkup(struct halfrel* self, struct halfrel* peer)
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
int graveasy_delete(struct entity* ent)
{
	return 0;
}
int graveasy_create(struct entity* ent, void* str)
{
	say("@graveasy_create\n");
	ent->FLAG = 0;
	return 0;
}
