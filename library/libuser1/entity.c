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
int balancer_create(void*, void*, int, u8**);
int balancer_delete(void*, void*);
int balancer_attach(void*, void*);
int balancer_detach(void*, void*);
int balancer_taking(void*,void*, void*,int, p64,int, void*,int);
int balancer_giving(void*,void*, void*,int, p64,int, void*,int);
int dronecontrol_create(void*, void*, int, u8**);
int dronecontrol_delete(void*, void*);
int dronecontrol_attach(void*, void*);
int dronecontrol_detach(void*, void*);
int dronecontrol_taking(void*,void*, void*,int, p64,int, void*,int);
int dronecontrol_giving(void*,void*, void*,int, p64,int, void*,int);
int rocketcontrol_create(void*, void*, int, u8**);
int rocketcontrol_delete(void*, void*);
int rocketcontrol_attach(void*, void*);
int rocketcontrol_detach(void*, void*);
int rocketcontrol_taking(void*,void*, void*,int, p64,int, void*,int);
int rocketcontrol_giving(void*,void*, void*,int, p64,int, void*,int);

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
int gravity_create(void*, void*, int, u8**);
int gravity_delete(void*, void*);
int gravity_reading(void*,void*, p64,int, void*,int);
int gravity_writing(void*,void*, p64,int, void*,int);
int gravity_attach(void*, void*);
int gravity_detach(void*, void*);
int gravity_giving(void*,void*, void*,int, p64,int, void*,int);
int gravity_taking(void*,void*, void*,int, p64,int, void*,int);

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




#define maxsz (0x100000/sizeof(_obj))
void entity_init(u8* addr, int size)
{
	logtoall("[e,f):entity initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j] = 0;

	entity = (void*)(addr+0x000000);
	actlen = maxsz-1;
	for(j=0;j<maxsz;j++)entity[j].tier = _ent_;

	style = (void*)(addr+0x100000);

	formem_init(addr);
	forcpu_init(addr);

	world_init(addr);
	thing_init(addr);

	logtoall("[e,f):entity inited\n");
}
void entity_exit()
{
	logtoall("[e,f):entity exiting\n");

	thing_exit();
	world_exit();

	forcpu_exit();
	formem_exit();

	style = 0;
	entity = 0;

	logtoall("[e,f):entity exited\n");
}


void* entity_findfmt(u64 fmt){
	_obj* tmp = 0x100000 + (void*)entity;
	int j;
	for(j=1;j<256;j++){
		tmp = &tmp[-1];		//prev
		if(fmt == tmp->type)return tmp;
	}
	return 0;
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



//alloc: get on empty memory in internal memory
void entity_recycle()
{
}
void* entity_alloc()
{
	int j,max;
	_obj* act;

	max = 0x100000 / sizeof(_obj);
	for(j=0;j<max;j++){
		act = &entity[j];
		if(0 == act->type)return act;
	}
	return 0;
}
void* entity_alloc_fromclone(_obj* tmp)
{
	_obj* ent = entity_alloc();
	if(0 == ent)return 0;

	int j;
	u8* dst = (void*)ent;
	for(j=0;j<sizeof(_obj);j++)dst[j] = 0;

	ent->tier = tmp->tier;
	ent->kind = tmp->kind;
	ent->type = tmp->type;
	ent->vfmt = tmp->vfmt;

	ent->oncreate = tmp->oncreate;
	ent->ondelete = tmp->ondelete;
	ent->onreader = tmp->onreader;
	ent->onwriter = tmp->onwriter;

	ent->onattach = tmp->onattach;
	ent->ondetach = tmp->ondetach;
	ent->ontaking = tmp->ontaking;
	ent->ongiving = tmp->ongiving;
	return ent;
}
void* entity_alloc_fromtype(u64 type)
{
	_obj* tmp = entity_findfmt(type);
	if(tmp){
		return entity_alloc_fromclone(tmp);
	}

	_obj* obj = entity_alloc();
	if(0 == obj)return 0;

	//obj->tier = tier;		//should be tier: bootup
	//obj->kind = kind;		//should be class: usb
	obj->type = type;		//should be type: xhci
	//obj->vfmt = vfmt;		//should be model: intelxhci
	return obj;
}
void* entity_alloc_fromfile(u64 fmt)
{
	return 0;
}




int entity_create(_obj* act, void* buf, int argc, u8** argv)
{
	//logtoall("%llx,%llx\n", type, buf);
	if(act->oncreate){
		return act->oncreate(act, buf, argc, argv);
	}

	switch(act->type){
	case _virtual_:
		virtual_create(act, buf, argc, argv);
		break;
	case _axis3d_:
		axis3d_create(act, buf, argc, argv);
		break;
	case _guide3d_:
		guide3d_create(act, buf, argc, argv);
		break;
	case _border3d_:
		border3d_create(act, buf, argc, argv);
		break;
	case _scene3d_:
		scene3d_create(act, buf, argc, argv);
		break;

	case _wndmgr_:
		wndmgr_create(act, buf, argc, argv);
		break;
	case _border2d_:
		border2d_create(act, buf, argc, argv);
		break;
	case _htmlroot_:
		htmlroot_create(act, buf, argc, argv);
		break;
	case _xamlroot_:
		xamlroot_create(act, buf, argc, argv);
		break;

	case _mmio_:
		mmiospace_create(act, buf, argc, argv);
		break;
	case _port_:
		portspace_create(act, buf, argc, argv);
		break;

	//circuit
	case _sch_:
		schematic_create(act, buf, argc, argv);
		break;
	case _pcb_:
		printboard_create(act, buf, argc, argv);
		break;
	case _analog_:
		analog_create(act, buf, argc, argv);
		break;
	case _digital_:
		digital_create(act, buf, argc, argv);
		break;

//----------------mind----------------
 	case _baby_:
		baby_create(act, buf, argc, argv);
		break;
	case _test_:
		test_create(act, buf, argc, argv);
		break;

	//event
	case _cam1rd_:
		cam1rd_create(act, buf, argc, argv);
		break;
	case _cam3rd_:
		cam3rd_create(act, buf, argc, argv);
		break;
	case _camrts_:
		camrts_create(act, buf, argc, argv);
		break;

	case _touchobj_:
		touchobj_create(act, buf, argc, argv);
		break;
	case _clickray_:
		clickray_create(act, buf, argc, argv);
		break;

//---------------sensor----------------
	case _virtimu_:
		virtimu_create(act, buf, argc, argv);
		break;

//---------------robot----------------
	case _follow_:
		follow_create(act, buf, argc, argv);
		break;
	case _lookat_:
		lookat_create(act, buf, argc, argv);
		break;
	case _wander_:
		wander_create(act, buf, argc, argv);
		break;

	case _carcon_:
		carcon_create(act, buf, argc, argv);
		break;
	case _balancer_:
		balancer_create(act, buf, argc, argv);
		break;
	case _dronecontrol_:
		dronecontrol_create(act, buf, argc, argv);
		break;
	case _rocketcontrol_:
		rocketcontrol_create(act, buf, argc, argv);
		break;

//----------------rule----------------

	//physic
	case _force_:
		force_create(act, buf, argc, argv);
		break;
	case _graveasy_:
		graveasy_create(act, buf, argc, argv);
		break;
	case _gravtest_:
		gravtest_create(act, buf, argc, argv);
		break;
	case _rigidall_:
		rigidsimu_create(act, buf, argc, argv);
		break;
	case _gravity_:
		gravity_create(act, buf, argc, argv);
		break;
	}//switch

	return 0;
}
int entity_delete(_obj* act)
{
	if(0 == act)return 0;

	if(act->ondelete){
		return act->ondelete(act);
	}

	switch(act->type){
	case _virtual_:
		virtual_delete(act, 0);
		break;
	}

	return 0;
}
int entity_reader(_obj* act,void* foot, p64 arg,int key, void* buf,int len)
{
	if(act->onreader){
		return act->onreader(act, foot, arg,key, buf, len);
	}

	return 0;
}
int entity_writer(_obj* act,void* foot, p64 arg,int key, void* buf,int len)
{
	if(act->onwriter){
		return act->onwriter(act, foot, arg,key, buf, len);
	}

	return 0;
}




int entity_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	if(0 == self)return 0;

	_obj* act = self->pchip;
	if(0 == act)return 0;

	if(act->onattach){
		return act->onattach(self, peer);
	}

	//logtoall("@entity_attach\n");
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
	case _gravity_:return gravity_attach(self, peer);

	case _sch_:return schematic_attach(self, peer);
	case _pcb_:return printboard_attach(self, peer);
	case _analog_:return analog_attach(self, peer);
	case _digital_:return digital_attach(self, peer);

	case _virtimu_:return virtimu_attach(self, peer);

	case _follow_:return follow_attach(self, peer);
	case _lookat_:return lookat_attach(self, peer);
	case _wander_:return wander_attach(self, peer);
	case _carcon_:return carcon_attach(self, peer);
	case _balancer_:return balancer_attach(self, peer);
	case _dronecontrol_:return dronecontrol_attach(self, peer);
	case _rocketcontrol_:return rocketcontrol_attach(self, peer);

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

	return 0;
}
int entity_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	if(0 == self)return 0;

	_obj* act = self->pchip;
	if(0 == act)return 0;

	if(act->ondetach){
		return act->ondetach(self, peer);
	}

	//logtoall("@entity_detach\n");
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
	case _gravity_:return gravity_detach(self, peer);

	case _sch_:return schematic_detach(self, peer);
	case _pcb_:return printboard_detach(self, peer);
	case _analog_:return analog_detach(self, peer);
	case _digital_:return digital_detach(self, peer);

	case _virtimu_:return virtimu_detach(self, peer);

	case _follow_:return follow_detach(self, peer);
	case _lookat_:return lookat_detach(self, peer);
	case _wander_:return wander_detach(self, peer);
	case _carcon_:return carcon_detach(self, peer);
	case _balancer_:return balancer_detach(self, peer);
	case _dronecontrol_:return dronecontrol_detach(self, peer);
	case _rocketcontrol_:return rocketcontrol_detach(self, peer);

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

	return 0;
}
int entity_takeby(_obj* act,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("%.8s, %p\n",&act->type, act->ontaking);
	if(act->ontaking){
		return act->ontaking(act,foot, stack,sp, arg,key, buf,len);
	}

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
	case _gravity_:return gravity_taking(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_taking(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_taking(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_taking(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_taking(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_taking(act,foot, stack,sp, arg,key, buf,len);

	case _follow_:return follow_taking(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_taking(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_taking(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_taking(act,foot, stack,sp, arg,key, buf,len);
	case _balancer_:return balancer_taking(act,foot, stack,sp, arg,key, buf,len);
	case _dronecontrol_:return dronecontrol_taking(act,foot, stack,sp, arg,key, buf,len);
	case _rocketcontrol_:return rocketcontrol_taking(act,foot, stack,sp, arg,key, buf,len);

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
	return 0;
}
int entity_giveby(_obj* act,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(act->ongiving){
		return act->ongiving(act,foot, stack,sp, arg,key, buf,len);
	}

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
	case _gravity_:return gravity_giving(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_giving(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_giving(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_giving(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_giving(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_giving(act,foot, stack,sp, arg,key, buf,len);

	case _follow_:return follow_giving(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_giving(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_giving(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_giving(act,foot, stack,sp, arg,key, buf,len);
	case _balancer_:return balancer_giving(act,foot, stack,sp, arg,key, buf,len);
	case _dronecontrol_:return dronecontrol_giving(act,foot, stack,sp, arg,key, buf,len);
	case _rocketcontrol_:return rocketcontrol_giving(act,foot, stack,sp, arg,key, buf,len);

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

	return 0;
}




//cmdline function
int entitycommand_insert(u8* name, u8* arg)
{
	u64 type;
	str2type64(name, (u8*)&type);

	_obj* obj = entity_alloc_fromtype(type);
	entity_create(obj, arg, 0, 0);
	return 0;
}
int entitycommand_remove(u8* name)
{
	return 0;
}
int entitycommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxsz;j++){
			act = &entity[j];
			if(0 == act->type)continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->kind, &act->type, &act->vfmt);
		}
		if(0 == j)logtoall("empty entity\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == entity[j].type)break;
			if(0 == cmp(&entity[j].type, name))logtoall("name=%d,node=%p\n", name, &entity[j]);
			break;
		}
	}
	return 0;
}
int entitycommand_modify(int argc, u8** argv)
{
	return 0;
}
void* entitycommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("entity insert name arg\n");
		logtoall("entity search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//entity create name arg
		entitycommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//entity search <name>
		entitycommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
