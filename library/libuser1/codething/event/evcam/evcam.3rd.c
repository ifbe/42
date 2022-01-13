#include "libuser.h"
#define _tar_ hex32('t','a','r',0)
void quaternion_rotatefrom(float* o, float* v, float* q);




struct privdata{
	_obj* cam;
	_obj* tar;

	struct style* caminworld;
	struct style* tarinworld;

	vec4 cam2tar;
	vec4 tar2tmp;	//shoulder surfing

	int mousedown_x;
	int mousedown_y;
	int mousedown_z;
	int mousedown_flag;
	int mousemove_x;
	int mousemove_y;
	int mousemove_z;
	int mousemove_flag;

	int touchdown[4];
	int touchmove[4];
};




int cam3rd_movetar(struct fstyle* camgeom, struct fstyle* targeom, int dx, int dy, int dz)
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
int cam3rd_nearfar(struct privdata* own, struct fstyle* cam, struct fstyle* tar, int key, int val)
{
	float k = 1.0;
	if('b' == key)k = 1.01 - val*0.01;
	if('f' == key)k = 0.99 - val*0.01;
	//say("%x,%x\n",key,val);

	own->cam2tar[3] *= k;
	vec3_setlen(own->cam2tar, own->cam2tar[3]);
	cam->vc[0] = tar->vc[0] - own->cam2tar[0];
	cam->vc[1] = tar->vc[1] - own->cam2tar[1];
	cam->vc[2] = tar->vc[2] - own->cam2tar[2];

	cam->vf[0] = own->cam2tar[0];
	cam->vf[1] = own->cam2tar[1];
	cam->vf[2] = own->cam2tar[2];
	vec3_normalize(cam->vf);

	//
	cam->vr[0] = cam->vf[1];
	cam->vr[1] =-cam->vf[0];
	cam->vr[2] = 0.0;
	vec3_normalize(cam->vr);

	//a × b = [aybz-azby, azbx-axbz, axby-aybx]
	cam->vt[0] = cam->vr[1]*cam->vf[2] - cam->vr[2]*cam->vf[1];
	cam->vt[1] = cam->vr[2]*cam->vf[0] - cam->vr[0]*cam->vf[2];
	cam->vt[2] = cam->vr[0]*cam->vf[1] - cam->vr[1]*cam->vf[0];
	return 0;
}
int cam3rd_movecam(struct privdata* own, struct fstyle* cam, struct fstyle* tar, int dx, int dy)
{
	float a,c,s;
	float q[4];
	float tmp[4];

	if(dy != 0){
		a = -dy * 0.005;
		s = getsin(a);
		c = getcos(a);

		//r
		q[0] = own->cam2tar[1];
		q[1] =-own->cam2tar[0];
		a = s / squareroot(q[0]*q[0] + q[1]*q[1]);

		//q
		q[0] *= a;
		q[1] *= a;
		q[2] = 0.0;
		q[3] = c;

		//new
		quaternion_rotatefrom(tmp, own->cam2tar, q);
		if(tmp[0]*own->cam2tar[0] + tmp[1]*own->cam2tar[1] > 0.0){
			own->cam2tar[0] = tmp[0];
			own->cam2tar[1] = tmp[1];
			own->cam2tar[2] = tmp[2];
		}
	}

	if(dx != 0){
		a = -dx * 0.005;
		s = getsin(a);
		c = getcos(a);
		tmp[0] = own->cam2tar[0];
		tmp[1] = own->cam2tar[1];
		own->cam2tar[0] = tmp[0]*c - tmp[1]*s;
		own->cam2tar[1] = tmp[0]*s + tmp[1]*c;
	}

	cam->vc[0] = tar->vc[0] - own->cam2tar[0];
	cam->vc[1] = tar->vc[1] - own->cam2tar[1];
	cam->vc[2] = tar->vc[2] - own->cam2tar[2];

	cam->vf[0] = tar->vc[0] - cam->vc[0];
	cam->vf[1] = tar->vc[1] - cam->vc[1];
	cam->vf[2] = tar->vc[2] - cam->vc[2];
	vec3_normalize(cam->vf);

	//a × b= [aybz-azby,azbx-axbz, axby-aybx]
	cam->vr[0] = cam->vf[1];
	cam->vr[1] =-cam->vf[0];
	cam->vr[2] = 0.0;
	vec3_normalize(cam->vr);

	//
	cam->vt[0] = cam->vr[1]*cam->vf[2] - cam->vr[2]*cam->vf[1];
	cam->vt[1] = cam->vr[2]*cam->vf[0] - cam->vr[0]*cam->vf[2];
	cam->vt[2] = cam->vr[0]*cam->vf[1] - cam->vr[1]*cam->vf[0];
	return 0;
}
void* cam3rd_findgeom(_obj* ent)
{
	struct relation* rel;
	_obj* world;
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




int cam3rd_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct privdata* own = ent->priv_ptr;
	if(0 == own)return 0;

	struct style* cam = own->caminworld;
	if(0 == cam){
		cam = cam3rd_findgeom(own->cam);
		if(0 == cam)return 0;

		own->caminworld = cam;
	}

	struct style* tar = own->tarinworld;
	if(0 == tar){
		tar = cam3rd_findgeom(own->tar);
		if(0 == tar)return 0;

		own->tarinworld = tar;
	}

	cam->fs.vc[0] = tar->fs.vc[0] - own->cam2tar[0];
	cam->fs.vc[1] = tar->fs.vc[1] - own->cam2tar[1];
	cam->fs.vc[2] = tar->fs.vc[2] - own->cam2tar[2];

	cam->fs.vf[0] = own->cam2tar[0];
	cam->fs.vf[1] = own->cam2tar[1];
	cam->fs.vf[2] = own->cam2tar[2];
	vec3_normalize(cam->fs.vf);

	//
	cam->fs.vr[0] = cam->fs.vf[1];
	cam->fs.vr[1] =-cam->fs.vf[0];
	cam->fs.vr[2] = 0.0;
	vec3_normalize(cam->fs.vr);

	//a × b = [aybz-azby, azbx-axbz, axby-aybx]
	cam->fs.vt[0] = cam->fs.vr[1]*cam->fs.vf[2] - cam->fs.vr[2]*cam->fs.vf[1];
	cam->fs.vt[1] = cam->fs.vr[2]*cam->fs.vf[0] - cam->fs.vr[0]*cam->fs.vf[2];
	cam->fs.vt[2] = cam->fs.vr[0]*cam->fs.vf[1] - cam->fs.vr[1]*cam->fs.vf[0];
	return 0;
}
int cam3rd_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, struct event* ev, int len)
{
	//printmemory(ev,16);
	struct privdata* own = ent->priv_ptr;
	if(0 == own)return 0;

	struct style* camgeom = own->caminworld;
	if(0 == camgeom){
		camgeom = cam3rd_findgeom(own->cam);
		if(0 == camgeom)return 0;

		own->caminworld = camgeom;
	}

	struct style* targeom = own->tarinworld;
	if(0 == targeom){
		targeom = cam3rd_findgeom(own->tar);
		if(0 == targeom)return 0;

		own->tarinworld = targeom;
	}

	if(0x2d70 == ev->what){
		own->mousedown_flag = 0;
		return 0;
	}
	if(0x2b70 == ev->what){
		short* ch = (void*)ev;
		own->mousedown_x = own->mousemove_x = ch[0];
		own->mousedown_y = own->mousemove_y = ch[1];
		own->mousedown_flag = 1;
		return 0;
	}
	if(0x4070 == ev->what){
		short* ch = (void*)ev;
		switch(ch[3]){
		case 'f':
		case 'b':{
			cam3rd_nearfar(own, &camgeom->fshape, &targeom->fshape, ch[3], ch[2]);
			break;
		}//mouse scroll
		case 'l':
		case 'r':{
			if(0 == own->mousedown_flag)return 0;
			cam3rd_movecam(own, &camgeom->fshape, &targeom->fshape, ch[0] - own->mousemove_x, ch[1] - own->mousemove_y);
			own->mousemove_x = ch[0];
			own->mousemove_y = ch[1];
			break;
		}//mouse move
		}//switch
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
		cam3rd_movetar(&camgeom->fshape, &targeom->fshape, dx, dy, dz);
		return 0;
	}
	else if('j' == (ev->what&0xff))
	{
		short* t = (void*)ev;
		if(joy_left == (ev->what & joy_mask))
		{
			int dx=0;
			int dy=0;
			if((t[0]<-4096)|(t[0]>4096))dx = t[0]/256;
			if((t[1]<-4096)|(t[1]>4096))dy = t[1]/256;
			cam3rd_movetar(&camgeom->fshape, &targeom->fshape, dx, dy, 0);
			return 0;
		}
		else if(joy_right == (ev->what & joy_mask))
		{
			int dx=0;
			int dy=0;
			if((t[0]<-4096)|(t[0]>4096))dx = t[0]/4096;
			if((t[1]<-4096)|(t[1]>4096))dy =-t[1]/4096;
			cam3rd_movecam(own, &camgeom->fshape, &targeom->fshape, dx, dy);
			return 0;
		}
	}
	return 0;
}
int cam3rd_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@cam3rd_detach\n");
	return 0;
}
int cam3rd_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@cam3rd_attach\n");
	_obj* ent = self->pchip;
	struct privdata* own = ent->priv_ptr;
	switch(self->foottype){
	case _cam_:own->cam = peer->pchip;break;
	case _tar_:own->tar = peer->pchip;break;
	}
	return 0;
}




int cam3rd_search(_obj* win)
{
	return 0;
}
int cam3rd_modify(_obj* win)
{
	return 0;
}
int cam3rd_delete(_obj* win)
{
	return 0;
}
int cam3rd_create(_obj* act, void* flag)
{
	struct privdata* own = act->priv_ptr = memorycreate(0x1000, 0);
	own->cam2tar[0] = 0.0;
	own->cam2tar[1] = 7.07;
	own->cam2tar[2] =-7.07;
	own->cam2tar[3] = 10.0;
	return 0;
}
