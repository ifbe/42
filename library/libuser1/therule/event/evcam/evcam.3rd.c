#include "libuser.h"
#define THECAM buf0
#define THETAR buf1
#define MOUSEDOWN iw0
#define _tar_ hex32('t','a','r',0)
#define _geom_ hex32('g','e','o','m')




int event3rd_movetar(struct fstyle* camgeom, struct fstyle* targeom, int dx, int dy, int dz)
{
	float* tar = targeom->vc;
	tar[0] += dx;
	tar[1] += dy;
	tar[2] += dz;

	float* cam = camgeom->vc;
	cam[0] += dx;
	cam[1] += dy;
	cam[2] += dz;
	return 0;
}
int event3rd_nearfar(struct fstyle* cam, struct fstyle* tar, int key, int val)
{
	float k = 1.0;
	if('b' == key)k = 1.01 - val*0.01;
	if('f' == key)k = 0.99 - val*0.01;
	//say("%x,%x\n",key,val);

	vec3 v;
	v[0] = cam->vc[0] - tar->vc[0];
	v[1] = cam->vc[1] - tar->vc[1];
	v[2] = cam->vc[2] - tar->vc[2];
	cam->vc[0] = tar->vc[0] + v[0]*k;
	cam->vc[1] = tar->vc[1] + v[1]*k;
	cam->vc[2] = tar->vc[2] + v[2]*k;
	return 0;
}
int event3rd_movecam(struct fstyle* cam, struct fstyle* tar, int dx, int dy)
{
	float a,c,s;
	float vec[4];
	float tmp[4];

	if(dy != 0){
		tmp[0] = vec[0] = cam->vc[0] - tar->vc[0];
		tmp[1] = vec[1] = cam->vc[1] - tar->vc[1];
		tmp[2] = vec[2] = cam->vc[2] - tar->vc[2];
		quaternion_operation(tmp, cam->vr, -dy/100.0);

		if(tmp[0]*vec[0] + tmp[1]*vec[1] > 0.0){
			cam->vc[0] = tar->vc[0]+tmp[0];
			cam->vc[1] = tar->vc[1]+tmp[1];
			cam->vc[2] = tar->vc[2]+tmp[2];
		}
	}

	if(dx != 0){
		vec[0] = cam->vc[0] - tar->vc[0];
		vec[1] = cam->vc[1] - tar->vc[1];
		a = -dx/100.0;
		s = sine(a);
		c = cosine(a);
		cam->vc[0] = tar->vc[0] + vec[0]*c - vec[1]*s;
		cam->vc[1] = tar->vc[1] + vec[0]*s + vec[1]*c;
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
void* event3rd_findgeom(struct entity* ent)
{
	struct relation* rel;
	struct entity* world;
	if(0 == ent)return 0;

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		world = rel->psrcchip;
		if(_virtual_ == world->type)return rel->psrcfoot;
		if(_scene3d_ == world->type)return rel->psrcfoot;
		rel = samedstnextsrc(rel);
	}

	rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		world = rel->pdstchip;
		if(_virtual_ == world->type)return rel->pdstfoot;
		if(_scene3d_ == world->type)return rel->pdstfoot;
		rel = samesrcnextdst(rel);
	}

	return 0;
}




int event3rd_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int event3rd_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, struct event* ev, int len)
{
	//printmemory(ev,16);
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
		struct fstyle* camgeom = event3rd_findgeom(ent->THECAM);
		if(0 == camgeom)return 0;

		struct fstyle* targeom = event3rd_findgeom(ent->THETAR);
		if(0 == targeom)return 0;

		short* ch = (void*)ev;
		switch(ch[3]){
		case 'f':
		case 'b':{
			event3rd_nearfar(camgeom, targeom, ch[3], ch[2]);
			break;
		}//mouse scroll
		case 'l':
		case 'r':{
			if(0 == ent->MOUSEDOWN)return 0;
			event3rd_movecam(camgeom, targeom, ch[0] - ent->ixn, ch[1] - ent->iyn);
			ent->ixn = ch[0];
			ent->iyn = ch[1];
			break;
		}//mouse move
		}//switch
		return 0;
	}
	if(_char_ == ev->what){
		struct fstyle* camgeom = event3rd_findgeom(ent->THECAM);
		if(0 == camgeom)return 0;

		struct fstyle* targeom = event3rd_findgeom(ent->THETAR);
		if(0 == targeom)return 0;

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
		event3rd_movetar(camgeom, targeom, dx, dy, dz);
		return 0;
	}
	else if('j' == (ev->what&0xff))
	{
		struct fstyle* camgeom = event3rd_findgeom(ent->THECAM);
		if(0 == camgeom)return 0;

		struct fstyle* targeom = event3rd_findgeom(ent->THETAR);
		if(0 == targeom)return 0;

		short* t = (void*)ev;
		if(joy_left == (ev->what & joy_mask))
		{
			int dx=0;
			int dy=0;
			if((t[0]<-4096)|(t[0]>4096))dx = t[0]/256;
			if((t[1]<-4096)|(t[1]>4096))dy = t[1]/256;
			event3rd_movetar(camgeom, targeom, dx, dy, 0);
			return 0;
		}
		else if(joy_right == (ev->what & joy_mask))
		{
			int dx=0;
			int dy=0;
			if((t[0]<-4096)|(t[0]>4096))dx = t[0]/4096;
			if((t[1]<-4096)|(t[1]>4096))dy =-t[1]/4096;
			event3rd_movecam(camgeom, targeom, dx, dy);
			return 0;
		}
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
	struct entity* ent = self->pchip;
	switch(self->flag){
	case _cam_:ent->THECAM = peer->pchip;break;
	case _tar_:ent->THETAR = peer->pchip;break;
	}
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
