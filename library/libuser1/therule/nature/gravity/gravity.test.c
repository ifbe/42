#include "libuser.h"
#define TIME data0
#define FLAG data1
#define JUMP data2




int gravtest_effect(struct style* geom, float dt)
{
	vec4 v,q;
	float a,i,c,s;
	struct fmotion* final = &geom->fm;
say("%f,%f,%f,%f\n",final->angular_v[0],final->angular_v[1],final->angular_v[2],final->angular_v[3]);


	//angular
	v[0] = final->angular_v[0] * final->angular_v[3];
	v[1] = final->angular_v[1] * final->angular_v[3];
	v[2] = final->angular_v[2] * final->angular_v[3];

	//omega = alpha * dt
	v[0] += final->angular_a[0] * dt;
	v[1] += final->angular_a[1] * dt;
	v[2] += final->angular_a[2] * dt;

	//helper
	a = vec3_getlen(v);
	i = 1.0 / a;
	c = cosine(a);
	s = sine(a);

	//quaternion
	q[0] = v[0] * s * i;
	q[1] = v[1] * s * i;
	q[2] = v[2] * s * i;
	q[3] = c;
	quaternion_rotate(geom->fs.vr, q);
	quaternion_rotate(geom->fs.vf, q);
	quaternion_rotate(geom->fs.vt, q);

	//write back
	final->angular_v[0] = v[0] * i;
	final->angular_v[1] = v[1] * i;
	final->angular_v[2] = v[2] * i;
	final->angular_v[3] = a;


	//displacement
	final->displace_v[2] += final->displace_a[2] * dt;
	final->displace_x[2] += final->displace_v[2] * dt;


	//collide
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
	float a,dt;
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
		if(ent->JUMP){
			geom->fm.angular_a[0] = 0.0;
			geom->fm.angular_a[1] = 0.0;
			geom->fm.angular_a[2] = 1.0;

			geom->fm.displace_a[0] = 0.0;
			geom->fm.displace_a[1] = 0.0;
			geom->fm.displace_a[2] = 9.8;
			ent->JUMP--;
		}
		else{
			a = -geom->fm.angular_v[3];
			geom->fm.angular_a[0] = geom->fm.angular_v[0] * a;
			geom->fm.angular_a[1] = geom->fm.angular_v[1] * a;
			geom->fm.angular_a[2] = geom->fm.angular_v[2] * a;

			geom->fm.displace_a[0] = 0.0;
			geom->fm.displace_a[1] = 0.0;
			geom->fm.displace_a[2] =-9.8;
		}
		gravtest_effect(geom, dt);

		rel = samesrcnextdst(rel);
	}

	ent->TIME = now;
	return 0;
}
int gravtest_addforce(struct entity* ent)
{
	say("@gravtest_addforce\n");
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
	if(_ioby_ == foot){
		gravtest_addforce(ent);
		ent->JUMP += 10;
	}
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
	ent->JUMP = 0;
	return 0;
}
