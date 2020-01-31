#include "libuser.h"
#define MOUSEDOWN iw0
#define _tar_ hex32('t','a','r',0)
#define _geom_ hex32('g','e','o','m')




int event3rd_movetar(struct style* camgeom, struct style* targeom, int dx, int dy, int dz)
{
	float* tar = targeom->fs.vc;
	tar[0] += dx;
	tar[1] += dy;
	tar[2] += dz;

	float* cam = camgeom->fs.vc;
	cam[0] += dx;
	cam[1] += dy;
	cam[2] += dz;
	return 0;
}
int event3rd_movecam(struct fstyle* cam, struct fstyle* tar, int dx, int dy)
{
	float a,c,s;
	float v[4];
	v[0] = cam->vc[0] - tar->vc[0];
	v[1] = cam->vc[1] - tar->vc[1];
	v[2] = cam->vc[2] - tar->vc[2];

	if(dy != 0){
		quaternion_operation(v, tar->vr, -dy/100.0);
		cam->vc[0] = tar->vc[0]+v[0];
		cam->vc[1] = tar->vc[1]+v[1];
		cam->vc[2] = tar->vc[2]+v[2];
	}
	if(dx != 0){
		a = -dx/100.0;
		s = sine(a);
		c = cosine(a);
		cam->vc[0] = tar->vc[0] + v[0]*c - v[1]*s;
		cam->vc[1] = tar->vc[1] + v[0]*s + v[1]*c;
	}

	cam->vf[0] = tar->vc[0] - cam->vc[0];
	cam->vf[1] = tar->vc[1] - cam->vc[1];
	cam->vf[2] = tar->vc[2] - cam->vc[2];
	vec3_normalize(cam->vf);

	//a × b= [aybz-azby,azbx-axbz, axby-aybx]
	cam->vr[0] = cam->vf[1];
	cam->vr[1] =-cam->vf[0];
	cam->vr[2] = 0.0;

	//
	cam->vt[0] = cam->vr[1]*cam->vf[2] - cam->vr[2]*cam->vf[1];
	cam->vt[1] = cam->vr[2]*cam->vf[0] - cam->vr[0]*cam->vf[2];
	cam->vt[2] = cam->vr[0]*cam->vf[1] - cam->vr[1]*cam->vf[0];
	return 0;
}
void* event3rd_find(struct entity* handler)
{
	int ret;
	struct halfrel* self;
	struct halfrel* peer;

	//from handler find target
	ret = relationsearch(handler, _tar_, &self, &peer);
	if(0 == ret)return 0;
	struct entity* tar = peer->pchip;
	if(0 == tar)return 0;

	//from target find geom
	ret = relationsearch(tar, _geom_, &self, &peer);
	if(0 == ret)return 0;

	return peer->pfoot;
}




int event3rd_read(struct halfrel* self, struct halfrel* peer, void* arg, int rsp, void* buf, int len)
{
	return 0;
}
int event3rd_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct event* ev, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;

	if(0x2d70 == ev->what){
		ent->MOUSEDOWN = 0;
		return 0;
	}
	if(0x2b70 == ev->what){
		short* ch = (void*)ev;
		ent->ix0 = ent->ixn = ch[0];
		ent->iy0 = ent->iyn = ch[1];
		ent->MOUSEDOWN = 1;
		return 0;
	}
	if(0x4070 == ev->what){
		if(0 == ent->MOUSEDOWN)return 0;

		struct style* camgeom = stack[rsp-1]->pfoot;
		if(0 == camgeom)return 0;
		struct style* targeom = event3rd_find(ent);
		if(0 == targeom)return 0;

		short* ch = (void*)ev;
		event3rd_movecam(&camgeom->fs, &targeom->fs, ch[0] - ent->ixn, ch[1] - ent->iyn);
		ent->ixn = ch[0];
		ent->iyn = ch[1];
		return 0;
	}
	if(_char_ == ev->what){
		int dx=0,dy=0,dz=0;
		switch(ev->why){
			case 'a':dx =-100;break;
			case 'd':dx = 100;break;
			case 's':dy =-100;break;
			case 'w':dy = 100;break;
			case 'q':dz =-100;break;
			case 'e':dz = 100;break;
			default:return 0;
		}
		struct style* camgeom = stack[rsp-1]->pfoot;
		if(0 == camgeom)return 0;
		struct style* targeom = event3rd_find(ent);
		if(0 == targeom)return 0;
		event3rd_movetar(camgeom, targeom, dx, dy, dz);
		return 0;
	}
	return 0;
}
int event3rd_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@event3rd_discon\n");
	return 0;
}
int event3rd_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@event3rd_linkup\n");
	return 0;
}




int event3rd_search(struct entity* win)
{
	return 0;
}
int event3rd_modify(struct entity* win)
{
	return 0;
}
int event3rd_delete(struct entity* win)
{
	return 0;
}
int event3rd_create(struct entity* act, void* flag)
{
	return 0;
}
