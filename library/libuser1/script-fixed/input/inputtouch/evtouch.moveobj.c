#include "libuser.h"
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




struct finger{
short x0;	//origin screen position
short y0;
short z0;
short w0;
short xn;	//current screen position
short yn;
short zn;
short wn;
float px;	//origin world position
float py;
float pz;
float pw;
float qx;	//current world position
float qy;
float qz;
float qw;
};




int touchobj_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int touchobj_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	struct event* ev = buf;
	short* t = buf;

	struct finger* fg = ent->priv_ptr;
	if(touch_away == ev->what){
		if(t[3] < 0)return 0;
		if(t[3] <10)return 0;

		fg = &fg[t[3]];
		fg->w0 = 1;
	}
	if(touch_onto == ev->what){
		if(t[3] < 0)return 0;
		if(t[3] <10)return 0;

		fg = &fg[t[3]];
		fg->w0 = 1;

		fg->x0 = fg->xn = t[0];
		fg->y0 = fg->yn = t[1];
		fg->z0 = fg->zn = t[2];

		//screen position to world position
	}
	if(touch_abs == ev->what){
		if(t[3] < 0)return 0;
		if(t[3] <10)return 0;

		fg = &fg[t[3]];
		fg->w0 = 1;

		fg->xn = t[0];
		fg->yn = t[1];
		fg->zn = t[2];

		//screen position to world position
	}
	return 0;
}
int touchobj_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@touchobj_detach\n");
	return 0;
}
int touchobj_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@touchobj_attach\n");
	return 0;
}




int touchobj_search(_obj* win)
{
	return 0;
}
int touchobj_modify(_obj* win)
{
	return 0;
}
int touchobj_delete(_obj* win)
{
	return 0;
}
int touchobj_create(_obj* act, void* flag)
{
	act->priv_ptr = memoryalloc(0x1000, 0);
	return 0;
}
