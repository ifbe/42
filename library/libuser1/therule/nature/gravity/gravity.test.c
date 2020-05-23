#include "libuser.h"
#define TIME data0
#define FLAG data1




int gravtest_effect(struct style* geom, float dt)
{
	vec3 delta;
	struct fmotion* final = &geom->fm;
say("%f,%f,%f,%f\n",final->angular_v[0],final->angular_v[1],final->angular_v[2],final->angular_v[3]);
	quaternion_rotate(geom->fs.vr, final->angular_v);
	quaternion_rotate(geom->fs.vf, final->angular_v);
	quaternion_rotate(geom->fs.vt, final->angular_v);

	//a = -9.8, v -= 9.8*dt, x += v*dt
	final->displace_a[2] = -9.8;
	final->displace_v[2] -= 9.8 * dt;
	final->displace_x[2] += final->displace_v[2] * dt;

	//if collide
	if(final->displace_x[2] < 0.0){
		final->displace_x[2] = 0.00001;
		final->displace_v[2] = -0.5 * final->displace_v[2];
	}
	say("%f,%f\n", final->displace_v[2], final->displace_x[2]);

	geom->fs.vc[2] = final->displace_x[2];
	return 0;
}
int gravtest_working(struct entity* ent)
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
		gravtest_effect(geom, dt);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}




int gravtest_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@gravtest_read:%.4s\n",&foot);
	return 0;
}
int gravtest_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@gravtest_write:%.4s\n",&foot);
	if(_clk_ == foot)gravtest_working(ent);
	return 0;
}
int gravtest_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gravtest_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gravtest_search()
{
	return 0;
}
int gravtest_modify()
{
	return 0;
}
int gravtest_delete(struct entity* ent)
{
	return 0;
}
int gravtest_create(struct entity* ent, void* str)
{
	say("@gravtest_create\n");
	ent->FLAG = 0;
	return 0;
}
