#include "libuser.h"
#define TIME listu64.data0
#define FLAG listu64.data1




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
	logtoall("%f,%f\n", final->displace_v[2], final->displace_x[2]);

	geom->fs.vc[2] = final->displace_x[2];
	return 0;
}
int graveasy_working(_obj* ent)
{
	u64 now;
	float dt;
	_obj* world;
	struct relation* rel;
	struct style* geom;

	rel = ent->orel0;
	if(0 == rel)return 0;

	world = rel->pdstchip;
	if((_virtual_ != world->type)&&(_scene3d_ != world->type))return 0;

	now = timeread_us();
	logtoall("%llx\n", now);
	dt = (float)((now - ent->TIME)%1000000)/1000000.0;
	logtoall("dt=%f\n",dt);
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




int graveasy_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@graveasy_read:%p,%p\n",ent,foot);
	return 0;
}
int graveasy_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@graveasy_give:%p,%p\n",ent,foot);
	if(_clk_ == stack[sp-1].foottype)graveasy_working(ent);
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
	ent->FLAG = 0;
	return 0;
}
