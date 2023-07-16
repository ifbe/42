#include "libuser.h"
//
void forcpu_init(void*);
void forcpu_exit();
void formem_init(void*);
void formem_exit();

//
void thing_init(void*);
void thing_exit();
void world_init(void*);
void world_exit();

//
int baby_create(void*, void*, int, u8**);
int baby_delete(void*, void*);
int baby_attach(void*, void*);
int baby_detach(void*, void*);
int baby_taking(void*,void*, void*,int, p64,int, void*,int);
int baby_giving(void*,void*, void*,int, p64,int, void*,int);
int test_create(void*, void*, int, u8**);
int test_delete(void*, void*);
int test_attach(void*, void*);
int test_detach(void*, void*);
int test_taking(void*,void*, void*,int, p64,int, void*,int);
int test_giving(void*,void*, void*,int, p64,int, void*,int);

//
int virtimu_create(void*, void*, int, u8**);
int virtimu_delete(void*, void*);
int virtimu_attach(void*, void*);
int virtimu_detach(void*, void*);
int virtimu_taking(void*,void*, void*,int, p64,int, void*,int);
int virtimu_giving(void*,void*, void*,int, p64,int, void*,int);

//
int follow_create(void*, void*, int, u8**);
int follow_delete(void*);
int follow_attach(void*, void*);
int follow_detach(void*, void*);
int follow_taking(void*,void*, void*,int, p64,int, void*,int);
int follow_giving(void*,void*, void*,int, p64,int, void*,int);
int lookat_create(void*, void*, int, u8**);
int lookat_delete(void*);
int lookat_attach(void*, void*);
int lookat_detach(void*, void*);
int lookat_taking(void*,void*, void*,int, p64,int, void*,int);
int lookat_giving(void*,void*, void*,int, p64,int, void*,int);
int wander_create(void*, void*, int, u8**);
int wander_delete(void*);
int wander_attach(void*, void*);
int wander_detach(void*, void*);
int wander_taking(void*,void*, void*,int, p64,int, void*,int);
int wander_giving(void*,void*, void*,int, p64,int, void*,int);
int carcon_create(void*, void*, int, u8**);
int carcon_delete(void*, void*);
int carcon_attach(void*, void*);
int carcon_detach(void*, void*);
int carcon_taking(void*,void*, void*,int, p64,int, void*,int);
int carcon_giving(void*,void*, void*,int, p64,int, void*,int);
int flycon_create(void*, void*, int, u8**);
int flycon_delete(void*, void*);
int flycon_attach(void*, void*);
int flycon_detach(void*, void*);
int flycon_taking(void*,void*, void*,int, p64,int, void*,int);
int flycon_giving(void*,void*, void*,int, p64,int, void*,int);
int balancer_create(void*, void*, int, u8**);
int balancer_delete(void*, void*);
int balancer_attach(void*, void*);
int balancer_detach(void*, void*);
int balancer_taking(void*,void*, void*,int, p64,int, void*,int);
int balancer_giving(void*,void*, void*,int, p64,int, void*,int);

/*
int toycar_create(void*, void*, int, u8**);
int toycar_delete(void*, void*);
int toycar_attach(void*, void*);
int toycar_detach(void*, void*);
int toycar_taking(void*,void*, void*,int, p64,int, void*,int);
int toycar_giving(void*,void*, void*,int, p64,int, void*,int);
int stepcar_create(void*, void*, int, u8**);
int stepcar_delete(void*, void*);
int stepcar_attach(void*, void*);
int stepcar_detach(void*, void*);
int stepcar_taking(void*,void*, void*,int, p64,int, void*,int);
int stepcar_giving(void*,void*, void*,int, p64,int, void*,int);
*/

//
int schematic_create(void*, void*, int, u8**);
int schematic_delete(void*, void*);
int schematic_attach(void*, void*);
int schematic_detach(void*, void*);
int schematic_giving(void*,void*, void*,int, p64,int, void*,int);
int schematic_taking(void*,void*, void*,int, p64,int, void*,int);
int printboard_create(void*, void*, int, u8**);
int printboard_delete(void*, void*);
int printboard_attach(void*, void*);
int printboard_detach(void*, void*);
int printboard_giving(void*,void*, void*,int, p64,int, void*,int);
int printboard_taking(void*,void*, void*,int, p64,int, void*,int);
int analog_create(void*, void*, int, u8**);
int analog_delete(void*, void*);
int analog_attach(void*, void*);
int analog_detach(void*, void*);
int analog_giving(void*,void*, void*,int, p64,int, void*,int);
int analog_taking(void*,void*, void*,int, p64,int, void*,int);
int digital_create(void*, void*, int, u8**);
int digital_delete(void*, void*);
int digital_attach(void*, void*);
int digital_detach(void*, void*);
int digital_giving(void*,void*, void*,int, p64,int, void*,int);
int digital_taking(void*,void*, void*,int, p64,int, void*,int);

//
int force_create(void*, void*, int, u8**);
int force_delete(void*, void*);
int force_attach(void*, void*);
int force_detach(void*, void*);
int force_giving(void*,void*, void*,int, p64,int, void*,int);
int force_taking(void*,void*, void*,int, p64,int, void*,int);
int graveasy_create(void*, void*, int, u8**);
int graveasy_delete(void*, void*);
int graveasy_attach(void*, void*);
int graveasy_detach(void*, void*);
int graveasy_giving(void*,void*, void*,int, p64,int, void*,int);
int graveasy_taking(void*,void*, void*,int, p64,int, void*,int);
int gravtest_create(void*, void*, int, u8**);
int gravtest_delete(void*, void*);
int gravtest_attach(void*, void*);
int gravtest_detach(void*, void*);
int gravtest_giving(void*,void*, void*,int, p64,int, void*,int);
int gravtest_taking(void*,void*, void*,int, p64,int, void*,int);
int rigidsimu_create(void*, void*, int, u8**);
int rigidsimu_delete(void*, void*);
int rigidsimu_attach(void*, void*);
int rigidsimu_detach(void*, void*);
int rigidsimu_giving(void*,void*, void*,int, p64,int, void*,int);
int rigidsimu_taking(void*,void*, void*,int, p64,int, void*,int);

//scene
int virtual_create(void*, void*, int, u8**);
int virtual_delete(void*, void*);
int virtual_attach(void*, void*);
int virtual_detach(void*, void*);
int virtual_giving(void*,void*, void*,int, p64,int, void*,int);
int virtual_taking(void*,void*, void*,int, p64,int, void*,int);
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_attach(void*, void*);
int scene3d_detach(void*, void*);
int scene3d_giving(void*,void*, void*,int, p64,int, void*,int);
int scene3d_taking(void*,void*, void*,int, p64,int, void*,int);
int axis3d_create(void*, void*, int, u8**);
int axis3d_delete(void*, void*);
int axis3d_attach(void*, void*);
int axis3d_detach(void*, void*);
int axis3d_giving(void*,void*, void*,int, p64,int, void*,int);
int axis3d_taking(void*,void*, void*,int, p64,int, void*,int);
int guide3d_create(void*, void*, int, u8**);
int guide3d_delete(void*, void*);
int guide3d_attach(void*, void*);
int guide3d_detach(void*, void*);
int guide3d_giving(void*,void*, void*,int, p64,int, void*,int);
int guide3d_taking(void*,void*, void*,int, p64,int, void*,int);
int border2d_create(void*, void*, int, u8**);
int border2d_delete(void*, void*);
int border2d_attach(void*, void*);
int border2d_detach(void*, void*);
int border2d_giving(void*,void*, void*,int, p64,int, void*,int);
int border2d_taking(void*,void*, void*,int, p64,int, void*,int);
int border3d_create(void*, void*, int, u8**);
int border3d_delete(void*, void*);
int border3d_attach(void*, void*);
int border3d_detach(void*, void*);
int border3d_giving(void*,void*, void*,int, p64,int, void*,int);
int border3d_taking(void*,void*, void*,int, p64,int, void*,int);

//
int wndmgr_create(void*, void*, int, u8**);
int wndmgr_delete(void*, void*);
int wndmgr_attach(void*, void*);
int wndmgr_detach(void*, void*);
int wndmgr_give(void*,void*, void*,int, p64,int, void*,int);
int wndmgr_take(void*,void*, void*,int, p64,int, void*,int);
int htmlroot_create(void*, void*, int, u8**);
int htmlroot_delete(void*);
int htmlroot_attach(void*, void*);
int htmlroot_detach(void*, void*);
int htmlroot_giving(void*,void*, void*,int, p64,int, void*,int);
int htmlroot_taking(void*,void*, void*,int, p64,int, void*,int);
int xamlroot_create(void*, void*, int, u8**);
int xamlroot_delete(void*, void*);
int xamlroot_attach(void*, void*);
int xamlroot_detach(void*, void*);
int xamlroot_giving(void*,void*, void*,int, p64,int, void*,int);
int xamlroot_taking(void*,void*, void*,int, p64,int, void*,int);

//
int mmiospace_create(void*, void*, int, u8**);
int mmiospace_delete(void*);
int mmiospace_attach(void*, void*);
int mmiospace_detach(void*, void*);
int mmiospace_giving(void*,void*, void*,int, p64,int, void*,int);
int mmiospace_taking(void*,void*, void*,int, p64,int, void*,int);
int portspace_create(void*, void*, int, u8**);
int portspace_delete(void*, void*);
int portspace_attach(void*, void*);
int portspace_detach(void*, void*);
int portspace_giving(void*,void*, void*,int, p64,int, void*,int);
int portspace_taking(void*,void*, void*,int, p64,int, void*,int);

//gl41 helper
int gl41data_create(_obj*, void*, int, u8**);
int gl41data_delete(_obj*);
int gl41data_attach(void*, void*);
int gl41data_detach(void*, void*);
int gl41data_taking(_obj*,void*, _syn*,int, p64,int, void*,int);
int gl41data_giving(_obj*,void*, _syn*,int, p64,int, void*,int);
int gl41coop_create(_obj*, void*, int, u8**);
int gl41coop_delete(_obj*);
int gl41coop_attach(void*, void*);
int gl41coop_detach(void*, void*);
int gl41coop_taking(_obj*,void*, void*,int, p64,int, void*,int);
int gl41coop_giving(_obj*,void*, void*,int, p64,int, void*,int);

//event
int cam1rd_create(void*, void*, int, u8**);
int cam1rd_delete(void*);
int cam1rd_attach(void*, void*);
int cam1rd_detach(void*, void*);
int cam1rd_taking(void*,void*, void*,int, p64,int, void*,int);
int cam1rd_giving(void*,void*, void*,int, p64,int, void*,int);
int cam3rd_create(void*, void*, int, u8**);
int cam3rd_delete(void*);
int cam3rd_attach(void*, void*);
int cam3rd_detach(void*, void*);
int cam3rd_taking(void*,void*, void*,int, p64,int, void*,int);
int cam3rd_giving(void*,void*, void*,int, p64,int, void*,int);
int camrts_create(void*, void*, int, u8**);
int camrts_delete(void*);
int camrts_attach(void*, void*);
int camrts_detach(void*, void*);
int camrts_taking(void*,void*, void*,int, p64,int, void*,int);
int camrts_giving(void*,void*, void*,int, p64,int, void*,int);
int clickray_create(void*, void*, int, u8**);
int clickray_delete(void*);
int clickray_attach(void*, void*);
int clickray_detach(void*, void*);
int clickray_taking(void*,void*, void*,int, p64,int, void*,int);
int clickray_giving(void*,void*, void*,int, p64,int, void*,int);
int touchobj_create(void*, void*, int, u8**);
int touchobj_delete(void*);
int touchobj_attach(void*, void*);
int touchobj_detach(void*, void*);
int touchobj_taking(void*,void*, void*,int, p64,int, void*,int);
int touchobj_giving(void*,void*, void*,int, p64,int, void*,int);




static _obj* entity = 0;
static int actlen = 0;
static struct style* style = 0;
static int stylen = 0;
void entity_init(u8* addr)
{
	say("[e,f):entity initing\n");

	int j;
	entity = (void*)(addr+0x000000);
	style = (void*)(addr+0x100000);

#define maxsz (0x100000/sizeof(_obj))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<maxsz;j++)entity[j].tier = _ent_;

	formem_init(addr);
	forcpu_init(addr);

	world_init(addr);
	thing_init(addr);

	say("[e,f):entity inited\n");
}
void entity_exit()
{
	say("[e,f):entity exiting\n");

	thing_exit();
	world_exit();

	forcpu_exit();
	formem_exit();

	style = 0;
	entity = 0;

	say("[e,f):entity exited\n");
}
void* entity_alloc()
{
	int j,max;
	_obj* act;

	max = 0x100000 / sizeof(_obj);
	for(j=0;j<max;j++){
		act = &entity[j];
		if(0 == act->hfmt)return act;
	}
	return 0;
}
void entity_recycle()
{
}




void* style_alloc()
{
#define maxlen (sizeof(struct style))
	int j;
	u8* buf;

	buf = (void*)style + stylen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	stylen += maxlen;
	return buf;
}
void style_recycle()
{
}




/*
void entityinput_touch(_obj* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touch[btn].xn = x;
		win->touch[btn].yn = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touch[btn].x0 = win->touch[btn].xn = x;
		win->touch[btn].y0 = win->touch[btn].yn = y;
		win->touch[btn].z0 = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touch[btn].z0 = 0;
	}
}*/




void* entity_createfromclone(u64 fmt, u8* arg, int argc, u8** argv)
{
	int j;
	u8* src;
	u8* dst;
	_obj* ent;
	_obj* tmp = 0x100000 + (void*)entity;

	for(j=1;j<256;j++){
		tmp = &tmp[-1];		//prev
		if(fmt == tmp->hfmt)goto found;
	}
	return 0;

found:
	ent = entity_alloc();
	dst = (void*)ent;
	for(j=0;j<sizeof(_obj);j++)dst[j] = 0;

	ent->tier = tmp->tier;
	ent->type = tmp->type;
	ent->hfmt  = tmp->hfmt;
	ent->vfmt = tmp->vfmt;

	ent->oncreate = tmp->oncreate;
	ent->ondelete = tmp->ondelete;
	ent->onreader = tmp->onreader;
	ent->onwriter = tmp->onwriter;

	ent->onattach = tmp->onattach;
	ent->ondetach = tmp->ondetach;
	ent->ontaking = tmp->ontaking;
	ent->ongiving = tmp->ongiving;

	ent->oncreate(ent, arg, argc, argv);
	return ent;
}
void* entity_createfromfile(u64 fmt, u8* arg, int argc, u8** argv)
{
	return 0;
}



void* entity_create(u64 type, void* buf, int argc, u8** argv)
{
	_obj* act;
	//say("%llx,%llx\n", type, buf);

	switch(type){
//----------------world----------------
	case _virtual_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _virtual_;
		virtual_create(act, buf, argc, argv);
		return act;
	}
	case _axis3d_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _axis3d_;
		axis3d_create(act, buf, argc, argv);
		return act;
	}
	case _guide3d_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _guide3d_;
		guide3d_create(act, buf, argc, argv);
		return act;
	}
	case _border3d_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _border3d_;
		border3d_create(act, buf, argc, argv);
		return act;
	}
	case _scene3d_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _scene3d_;
		scene3d_create(act, buf, argc, argv);
		return act;
	}

	case _wndmgr_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _wndmgr_;
		wndmgr_create(act, buf, argc, argv);
		return act;
	}
	case _border2d_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _border2d_;
		border2d_create(act, buf, argc, argv);
		return act;
	}
	case _htmlroot_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _htmlroot_;
		htmlroot_create(act, buf, argc, argv);
		return act;
	}
	case _xamlroot_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _xamlroot_;
		xamlroot_create(act, buf, argc, argv);
		return act;
	}

	case _mmio_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _mmio_;
		mmiospace_create(act, buf, argc, argv);
		return act;
	}
	case _port_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _port_;
		portspace_create(act, buf, argc, argv);
		return act;
	}

	//circuit
	case _sch_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _sch_;
		schematic_create(act, buf, argc, argv);
		return act;
	}
	case _pcb_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _pcb_;
		printboard_create(act, buf, argc, argv);
		return act;
	}
	case _analog_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _analog_;
		analog_create(act, buf, argc, argv);
		return act;
	}
	case _digital_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _digital_;
		digital_create(act, buf, argc, argv);
		return act;
	}

//----------------mind----------------
 	case _baby_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _baby_;
		baby_create(act, buf, argc, argv);
		return act;
	}
	case _test_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _test_;
		test_create(act, buf, argc, argv);
		return act;
	}

	//event
	case _cam1rd_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _cam1rd_;
		cam1rd_create(act, buf, argc, argv);
		return act;
	}
	case _cam3rd_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _cam3rd_;
		cam3rd_create(act, buf, argc, argv);
		return act;
	}
	case _camrts_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _camrts_;
		camrts_create(act, buf, argc, argv);
		return act;
	}

	case _touchobj_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _touchobj_;
		touchobj_create(act, buf, argc, argv);
		return act;
	}
	case _clickray_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _clickray_;
		clickray_create(act, buf, argc, argv);
		return act;
	}

//---------------sensor----------------
	case _virtimu_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _virtimu_;
		virtimu_create(act, buf, argc, argv);
		return act;
	}

//---------------robot----------------
	case _follow_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _follow_;
		follow_create(act, buf, argc, argv);
		return act;
	}
	case _lookat_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _lookat_;
		lookat_create(act, buf, argc, argv);
		return act;
	}
	case _wander_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _wander_;
		wander_create(act, buf, argc, argv);
		return act;
	}

	case _carcon_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _carcon_;
		carcon_create(act, buf, argc, argv);
		return act;
	}
	case _flycon_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _flycon_;
		flycon_create(act, buf, argc, argv);
		return act;
	}
	case _balancer_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _balancer_;
		balancer_create(act, buf, argc, argv);
		return act;
	}

//----------------rule----------------

	//physic
	case _force_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _force_;
		force_create(act, buf, argc, argv);
		return act;
	}
	case _graveasy_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _graveasy_;
		graveasy_create(act, buf, argc, argv);
		return act;
	}
	case _gravtest_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _gravtest_;
		gravtest_create(act, buf, argc, argv);
		return act;
	}
	case _rigidall_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _rigidall_;
		rigidsimu_create(act, buf, argc, argv);
		return act;
	}

//----------------other----------------
/*	case _bdc_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _bdc_;
		toycar_create(act, buf, argc, argv);
		return act;
	}
	case _step_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _step_;
		stepcar_create(act, buf, argc, argv);
		return act;
	}
*/
/*	case _gl41data_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _gl41data_;
		gl41data_create(act, buf, argc, argv);
		return act;
	}
	case _gl41coop_:
	{
		act = entity_alloc();
		act->hfmt = act->type = _gl41coop_;
		gl41coop_create(act, buf, argc, argv);
		return act;
	}*/

	}//switch

	//
	return entity_createfromclone(type, buf, argc, argv);
}
int entity_delete(_obj* p)
{
	if(0 == p)return 0;
	_obj* act = p;

	switch(act->type){
		case _orig_:return 0;
		case _copy_:return 0;
		case _ORIG_:act->type = _orig_;
		case _COPY_:act->type = _copy_;
	}
	act->ondelete(act);

	return 0;
}
int entity_reader(_obj* act,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int entity_writer(_obj* act,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}




int entity_attach(struct halfrel* self, struct halfrel* peer)
{
	if(0 == self)return 0;

	_obj* act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_attach\n");
	switch(act->type){
	case _cam1rd_:return cam1rd_attach(self, peer);
	case _cam3rd_:return cam3rd_attach(self, peer);
	case _camrts_:return camrts_attach(self, peer);

	case _touchobj_:return touchobj_attach(self, peer);
	case _clickray_:return clickray_attach(self, peer);

	//case _gl41data_:return gl41data_attach(self, peer);
	//case _gl41coop_:return gl41coop_attach(self, peer);

	//case _field_:return field_attach(self, peer);
	//case _fluid_:return fluid_attach(self, peer);
	case _force_:return force_attach(self, peer);
	case _graveasy_:return graveasy_attach(self, peer);
	case _gravtest_:return gravtest_attach(self, peer);
	case _rigidall_:return rigidsimu_attach(self, peer);

	case _sch_:return schematic_attach(self, peer);
	case _pcb_:return printboard_attach(self, peer);
	case _analog_:return analog_attach(self, peer);
	case _digital_:return digital_attach(self, peer);

	case _virtimu_:return virtimu_attach(self, peer);

	case _follow_:return follow_attach(self, peer);
	case _lookat_:return lookat_attach(self, peer);
	case _wander_:return wander_attach(self, peer);
	case _carcon_:return carcon_attach(self, peer);
	case _flycon_:return flycon_attach(self, peer);
	case _balancer_:return balancer_attach(self, peer);

	case _virtual_:return virtual_attach(self, peer);
	case _scene3d_:return scene3d_attach(self, peer);
	case _axis3d_:return axis3d_attach(self, peer);
	case _guide3d_:return guide3d_attach(self, peer);
	case _border2d_:return border2d_attach(self, peer);
	case _border3d_:return border3d_attach(self, peer);

	case _htmlroot_:return htmlroot_attach(self, peer);
	case _xamlroot_:return xamlroot_attach(self, peer);
	case _mmio_:return mmiospace_attach(self, peer);
	case _port_:return portspace_attach(self, peer);
/*
	case _step_:return stepcar_attach(self, peer);
	case _bdc_:return toycar_attach(self, peer);
*/
	case _test_:return test_attach(self, peer);
	case _baby_:return baby_attach(self, peer);
	}

	if(0 == act->onattach)return 0;
	return act->onattach(self, peer);
}
int entity_detach(struct halfrel* self, struct halfrel* peer)
{
	if(0 == self)return 0;

	_obj* act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_detach\n");
	switch(act->type){
	case _cam1rd_:return cam1rd_detach(self, peer);
	case _cam3rd_:return cam3rd_detach(self, peer);
	case _camrts_:return camrts_detach(self, peer);

	case _touchobj_:return touchobj_detach(self, peer);
	case _clickray_:return clickray_detach(self, peer);

	//case _gl41data_:return gl41data_detach(self, peer);
	//case _gl41coop_:return gl41coop_detach(self, peer);

	//case _field_:return field_detach(self, peer);
	//case _fluid_:return fluid_detach(self, peer);
	case _force_:return force_detach(self, peer);
	case _graveasy_:return graveasy_detach(self, peer);
	case _gravtest_:return gravtest_detach(self, peer);
	case _rigidall_:return rigidsimu_detach(self, peer);

	case _sch_:return schematic_detach(self, peer);
	case _pcb_:return printboard_detach(self, peer);
	case _analog_:return analog_detach(self, peer);
	case _digital_:return digital_detach(self, peer);

	case _virtimu_:return virtimu_detach(self, peer);

	case _follow_:return follow_detach(self, peer);
	case _lookat_:return lookat_detach(self, peer);
	case _wander_:return wander_detach(self, peer);
	case _carcon_:return carcon_detach(self, peer);
	case _flycon_:return flycon_detach(self, peer);
	case _balancer_:return balancer_detach(self, peer);

	case _virtual_:return virtual_detach(self, peer);
	case _scene3d_:return scene3d_detach(self, peer);
	case _axis3d_:return axis3d_detach(self, peer);
	case _guide3d_:return guide3d_detach(self, peer);
	case _border2d_:return border2d_detach(self, peer);
	case _border3d_:return border3d_detach(self, peer);

	case _htmlroot_:return htmlroot_detach(self, peer);
	case _xamlroot_:return xamlroot_detach(self, peer);
	case _mmio_:return mmiospace_detach(self, peer);
	case _port_:return portspace_detach(self, peer);
/*
	case _step_:return stepcar_detach(self, peer);
	case _bdc_:return toycar_detach(self, peer);
*/
	case _test_:return test_detach(self, peer);
	case _baby_:return baby_detach(self, peer);
	}

	if(0 == act->ondetach)return 0;
	return act->ondetach(self, peer);
}
int entity_takeby(_obj* act,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(act->type){
	case _cam1rd_:return cam1rd_taking(act,foot, stack,sp, arg,key, buf,len);
	case _cam3rd_:return cam3rd_taking(act,foot, stack,sp, arg,key, buf,len);
	case _camrts_:return camrts_taking(act,foot, stack,sp, arg,key, buf,len);
	case _touchobj_:return touchobj_taking(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_taking(act,foot, stack,sp, arg,key, buf,len);

	//case _gl41data_:return gl41data_taking(act,foot, stack,sp, arg,key, buf,len);
	//case _gl41coop_:return gl41coop_taking(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_taking(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_taking(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_taking(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_taking(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_taking(act,foot, stack,sp, arg,key, buf,len);
	case _rigidall_:return rigidsimu_taking(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_taking(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_taking(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_taking(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_taking(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_taking(act,foot, stack,sp, arg,key, buf,len);

	case _follow_:return follow_taking(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_taking(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_taking(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_taking(act,foot, stack,sp, arg,key, buf,len);
	case _flycon_:return flycon_taking(act,foot, stack,sp, arg,key, buf,len);
	case _balancer_:return balancer_taking(act,foot, stack,sp, arg,key, buf,len);

	case _virtual_:return virtual_taking(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _axis3d_:return axis3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _wndmgr_:return wndmgr_take(act,foot, stack,sp, arg,key, buf,len);

	case _htmlroot_:return htmlroot_taking(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_taking(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_taking(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_taking(act,foot, stack,sp, arg,key, buf,len);
/*
	case _step_:return stepcar_taking(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_taking(act,foot, stack,sp, arg,key, buf,len);
*/
	case _test_:return test_taking(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_taking(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->ontaking)return 0;
	return act->ontaking(act,foot, stack,sp, arg,key, buf,len);
}
int entity_giveby(_obj* act,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(act->type){
	case _cam1rd_:return cam1rd_giving(act,foot, stack,sp, arg,key, buf,len);
	case _cam3rd_:return cam3rd_giving(act,foot, stack,sp, arg,key, buf,len);
	case _camrts_:return camrts_giving(act,foot, stack,sp, arg,key, buf,len);

	case _touchobj_:return touchobj_giving(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_giving(act,foot, stack,sp, arg,key, buf,len);

	//case _gl41data_:return gl41data_giving(act,foot, stack,sp, arg,key, buf,len);
	//case _gl41coop_:return gl41coop_giving(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_giving(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_giving(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_giving(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_giving(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_giving(act,foot, stack,sp, arg,key, buf,len);
	case _rigidall_:return rigidsimu_giving(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_giving(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_giving(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_giving(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_giving(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_giving(act,foot, stack,sp, arg,key, buf,len);

	case _follow_:return follow_giving(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_giving(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_giving(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_giving(act,foot, stack,sp, arg,key, buf,len);
	case _flycon_:return flycon_giving(act,foot, stack,sp, arg,key, buf,len);
	case _balancer_:return balancer_giving(act,foot, stack,sp, arg,key, buf,len);

	case _virtual_:return virtual_giving(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _axis3d_:return axis3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _wndmgr_:return wndmgr_give(act,foot, stack,sp, arg,key, buf,len);

	case _htmlroot_:return htmlroot_giving(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_giving(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_giving(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_giving(act,foot, stack,sp, arg,key, buf,len);
/*
	case _step_:return stepcar_giving(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_giving(act,foot, stack,sp, arg,key, buf,len);
*/
	case _test_:return test_giving(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_giving(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->ongiving)return 0;
	return act->ongiving(act,foot, stack,sp, arg,key, buf,len);
}




int entity_insert(u8* buf, int len)
{
	return 0;
}
int entity_remove(u8* buf, int len)
{
	return 0;
}
void* entity_search(u8* buf, int len)
{
	int j,k;
	u8* p;
	_obj* act;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			act = &entity[j];
			if(0 == act->hfmt)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->hfmt, &act->hfmt);
		}
		if(0 == j)say("empty entity\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == entity[j].hfmt)break;
			p = (void*)(&entity[j].hfmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &entity[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}
void* entity_modify(int argc, u8** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		entity_create(name, argv[3], argc-3, &argv[3]);
	}

	return 0;
}
