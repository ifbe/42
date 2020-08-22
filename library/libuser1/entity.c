#include "libuser.h"
//
void forcpu_init();
void forcpu_free();
void formem_init();
void formem_free();

//
void thing_free();
void thing_init(void*);
void world_free();
void world_init(void*);
void mind_free();
void mind_init(void*);
void rule_free();
void rule_init(void*);

//
int baby_create(void*, void*, int, u8**);
int baby_delete(void*, void*);
int baby_linkup(void*, void*);
int baby_discon(void*, void*);
int baby_taking(void*,int, void*,int, void*,int, void*,int);
int baby_giving(void*,int, void*,int, void*,int, void*,int);
int test_create(void*, void*, int, u8**);
int test_delete(void*, void*);
int test_linkup(void*, void*);
int test_discon(void*, void*);
int test_taking(void*,int, void*,int, void*,int, void*,int);
int test_giving(void*,int, void*,int, void*,int, void*,int);

//
int virtimu_create(void*, void*, int, u8**);
int virtimu_delete(void*, void*);
int virtimu_linkup(void*, void*);
int virtimu_discon(void*, void*);
int virtimu_taking(void*,int, void*,int, void*,int, void*,int);
int virtimu_giving(void*,int, void*,int, void*,int, void*,int);
int carcon_create(void*, void*, int, u8**);
int carcon_delete(void*, void*);
int carcon_linkup(void*, void*);
int carcon_discon(void*, void*);
int carcon_taking(void*,int, void*,int, void*,int, void*,int);
int carcon_giving(void*,int, void*,int, void*,int, void*,int);
int flycon_create(void*, void*, int, u8**);
int flycon_delete(void*, void*);
int flycon_linkup(void*, void*);
int flycon_discon(void*, void*);
int flycon_taking(void*,int, void*,int, void*,int, void*,int);
int flycon_giving(void*,int, void*,int, void*,int, void*,int);

//
int toycar_create(void*, void*, int, u8**);
int toycar_delete(void*, void*);
int toycar_linkup(void*, void*);
int toycar_discon(void*, void*);
int toycar_taking(void*,int, void*,int, void*,int, void*,int);
int toycar_giving(void*,int, void*,int, void*,int, void*,int);
int stepcar_create(void*, void*, int, u8**);
int stepcar_delete(void*, void*);
int stepcar_linkup(void*, void*);
int stepcar_discon(void*, void*);
int stepcar_taking(void*,int, void*,int, void*,int, void*,int);
int stepcar_giving(void*,int, void*,int, void*,int, void*,int);


//
int schematic_create(void*, void*, int, u8**);
int schematic_delete(void*, void*);
int schematic_linkup(void*, void*);
int schematic_discon(void*, void*);
int schematic_giving(void*,int, void*,int, void*,int, void*,int);
int schematic_taking(void*,int, void*,int, void*,int, void*,int);
int printboard_create(void*, void*, int, u8**);
int printboard_delete(void*, void*);
int printboard_linkup(void*, void*);
int printboard_discon(void*, void*);
int printboard_giving(void*,int, void*,int, void*,int, void*,int);
int printboard_taking(void*,int, void*,int, void*,int, void*,int);
int analog_create(void*, void*, int, u8**);
int analog_delete(void*, void*);
int analog_linkup(void*, void*);
int analog_discon(void*, void*);
int analog_giving(void*,int, void*,int, void*,int, void*,int);
int analog_taking(void*,int, void*,int, void*,int, void*,int);
int digital_create(void*, void*, int, u8**);
int digital_delete(void*, void*);
int digital_linkup(void*, void*);
int digital_discon(void*, void*);
int digital_giving(void*,int, void*,int, void*,int, void*,int);
int digital_taking(void*,int, void*,int, void*,int, void*,int);

//
int force_create(void*, void*, int, u8**);
int force_delete(void*, void*);
int force_linkup(void*, void*);
int force_discon(void*, void*);
int force_giving(void*,int, void*,int, void*,int, void*,int);
int force_taking(void*,int, void*,int, void*,int, void*,int);
int graveasy_create(void*, void*, int, u8**);
int graveasy_delete(void*, void*);
int graveasy_linkup(void*, void*);
int graveasy_discon(void*, void*);
int graveasy_giving(void*,int, void*,int, void*,int, void*,int);
int graveasy_taking(void*,int, void*,int, void*,int, void*,int);
int gravtest_create(void*, void*, int, u8**);
int gravtest_delete(void*, void*);
int gravtest_linkup(void*, void*);
int gravtest_discon(void*, void*);
int gravtest_giving(void*,int, void*,int, void*,int, void*,int);
int gravtest_taking(void*,int, void*,int, void*,int, void*,int);

//scene
int axis3d_create(void*, void*, int, u8**);
int axis3d_delete(void*, void*);
int axis3d_linkup(void*, void*);
int axis3d_discon(void*, void*);
int axis3d_giving(void*,int, void*,int, void*,int, void*,int);
int axis3d_taking(void*,int, void*,int, void*,int, void*,int);
int guide3d_create(void*, void*, int, u8**);
int guide3d_delete(void*, void*);
int guide3d_linkup(void*, void*);
int guide3d_discon(void*, void*);
int guide3d_giving(void*,int, void*,int, void*,int, void*,int);
int guide3d_taking(void*,int, void*,int, void*,int, void*,int);
int border2d_create(void*, void*, int, u8**);
int border2d_delete(void*, void*);
int border2d_linkup(void*, void*);
int border2d_discon(void*, void*);
int border2d_giving(void*,int, void*,int, void*,int, void*,int);
int border2d_taking(void*,int, void*,int, void*,int, void*,int);
int border3d_create(void*, void*, int, u8**);
int border3d_delete(void*, void*);
int border3d_linkup(void*, void*);
int border3d_discon(void*, void*);
int border3d_giving(void*,int, void*,int, void*,int, void*,int);
int border3d_taking(void*,int, void*,int, void*,int, void*,int);
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_linkup(void*, void*);
int scene3d_discon(void*, void*);
int scene3d_giving(void*,int, void*,int, void*,int, void*,int);
int scene3d_taking(void*,int, void*,int, void*,int, void*,int);

//
int reality_create(void*, void*, int, u8**);
int reality_delete(void*);
int reality_linkup(void*, void*);
int reality_discon(void*, void*);
int reality_giving(void*,int, void*,int, void*,int, void*,int);
int reality_taking(void*,int, void*,int, void*,int, void*,int);
int virtual_create(void*, void*, int, u8**);
int virtual_delete(void*, void*);
int virtual_linkup(void*, void*);
int virtual_discon(void*, void*);
int virtual_giving(void*,int, void*,int, void*,int, void*,int);
int virtual_taking(void*,int, void*,int, void*,int, void*,int);
int htmlroot_create(void*, void*, int, u8**);
int htmlroot_delete(void*);
int htmlroot_linkup(void*, void*);
int htmlroot_discon(void*, void*);
int htmlroot_giving(void*,int, void*,int, void*,int, void*,int);
int htmlroot_taking(void*,int, void*,int, void*,int, void*,int);
int xamlroot_create(void*, void*, int, u8**);
int xamlroot_delete(void*, void*);
int xamlroot_linkup(void*, void*);
int xamlroot_discon(void*, void*);
int xamlroot_giving(void*,int, void*,int, void*,int, void*,int);
int xamlroot_taking(void*,int, void*,int, void*,int, void*,int);
int mmiospace_create(void*, void*, int, u8**);
int mmiospace_delete(void*);
int mmiospace_linkup(void*, void*);
int mmiospace_discon(void*, void*);
int mmiospace_giving(void*,int, void*,int, void*,int, void*,int);
int mmiospace_taking(void*,int, void*,int, void*,int, void*,int);
int portspace_create(void*, void*, int, u8**);
int portspace_delete(void*, void*);
int portspace_linkup(void*, void*);
int portspace_discon(void*, void*);
int portspace_giving(void*,int, void*,int, void*,int, void*,int);
int portspace_taking(void*,int, void*,int, void*,int, void*,int);

//gl41 helper
int gl41data_create(void*, void*, int, u8**);
int gl41data_delete(void*);
int gl41data_linkup(void*, void*);
int gl41data_discon(void*, void*);
int gl41data_taking(void*,int, void*,int, void*,int, void*,int);
int gl41data_giving(void*,int, void*,int, void*,int, void*,int);
int gl41coop_create(void*, void*, int, u8**);
int gl41coop_delete(void*);
int gl41coop_linkup(void*, void*);
int gl41coop_discon(void*, void*);
int gl41coop_taking(void*,int, void*,int, void*,int, void*,int);
int gl41coop_giving(void*,int, void*,int, void*,int, void*,int);

//event
int cam1rd_create(void*, void*, int, u8**);
int cam1rd_delete(void*);
int cam1rd_linkup(void*, void*);
int cam1rd_discon(void*, void*);
int cam1rd_taking(void*,int, void*,int, void*,int, void*,int);
int cam1rd_giving(void*,int, void*,int, void*,int, void*,int);
int cam3rd_create(void*, void*, int, u8**);
int cam3rd_delete(void*);
int cam3rd_linkup(void*, void*);
int cam3rd_discon(void*, void*);
int cam3rd_taking(void*,int, void*,int, void*,int, void*,int);
int cam3rd_giving(void*,int, void*,int, void*,int, void*,int);
int camrts_create(void*, void*, int, u8**);
int camrts_delete(void*);
int camrts_linkup(void*, void*);
int camrts_discon(void*, void*);
int camrts_taking(void*,int, void*,int, void*,int, void*,int);
int camrts_giving(void*,int, void*,int, void*,int, void*,int);
int clickray_create(void*, void*, int, u8**);
int clickray_delete(void*);
int clickray_linkup(void*, void*);
int clickray_discon(void*, void*);
int clickray_taking(void*,int, void*,int, void*,int, void*,int);
int clickray_giving(void*,int, void*,int, void*,int, void*,int);
int touchobj_create(void*, void*, int, u8**);
int touchobj_delete(void*);
int touchobj_linkup(void*, void*);
int touchobj_discon(void*, void*);
int touchobj_taking(void*,int, void*,int, void*,int, void*,int);
int touchobj_giving(void*,int, void*,int, void*,int, void*,int);
int follow_create(void*, void*, int, u8**);
int follow_delete(void*);
int follow_linkup(void*, void*);
int follow_discon(void*, void*);
int follow_taking(void*,int, void*,int, void*,int, void*,int);
int follow_giving(void*,int, void*,int, void*,int, void*,int);
int lookat_create(void*, void*, int, u8**);
int lookat_delete(void*);
int lookat_linkup(void*, void*);
int lookat_discon(void*, void*);
int lookat_taking(void*,int, void*,int, void*,int, void*,int);
int lookat_giving(void*,int, void*,int, void*,int, void*,int);
int wander_create(void*, void*, int, u8**);
int wander_delete(void*);
int wander_linkup(void*, void*);
int wander_discon(void*, void*);
int wander_taking(void*,int, void*,int, void*,int, void*,int);
int wander_giving(void*,int, void*,int, void*,int, void*,int);




static struct entity* entity = 0;
static int actlen = 0;
static struct style* style = 0;
static int stylen = 0;
void* allocentity()
{
	int j,max;
	struct entity* act;

	max = 0x100000 / sizeof(struct entity);
	for(j=0;j<max;j++){
		act = &entity[j];
		if(0 == act->fmt)return act;
	}
	return 0;
}
void* allocstyle()
{
#define maxlen (sizeof(struct style))
	int j;
	u8* buf;

	buf = (void*)style + stylen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	stylen += maxlen;
	return buf;
}
void* entitycreate_clone(u64 fmt, u8* arg, int argc, u8** argv)
{
	int j;
	u8* src;
	u8* dst;
	struct entity* ent;
	struct entity* tmp = 0x100000 + (void*)entity;

	for(j=1;j<256;j++){
		tmp = &tmp[-1];		//prev
		if(fmt == tmp->fmt)goto found;
	}
	return 0;

found:
	ent = allocentity();
	dst = (void*)ent;
	for(j=0;j<sizeof(struct entity);j++)dst[j] = 0;

	ent->tier = tmp->tier;
	ent->type = tmp->type;
	ent->fmt  = tmp->fmt;
	ent->vfmt = tmp->vfmt;

	ent->oncreate = tmp->oncreate;
	ent->ondelete = tmp->ondelete;
	ent->onsearch = tmp->onsearch;
	ent->onmodify = tmp->onmodify;
	ent->onlinkup = tmp->onlinkup;
	ent->ondiscon = tmp->ondiscon;
	ent->ontaking = tmp->ontaking;
	ent->ongiving = tmp->ongiving;

	ent->oncreate(ent, arg, argc, argv);
	return ent;
}



/*
void entityinput_touch(struct supply* win, struct event* ev)
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




int entityread(_ent* act,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(act->type){
	case _cam1rd_:return cam1rd_taking(act,foot, stack,sp, arg,key, buf,len);
	case _cam3rd_:return cam3rd_taking(act,foot, stack,sp, arg,key, buf,len);
	case _camrts_:return camrts_taking(act,foot, stack,sp, arg,key, buf,len);
	case _follow_:return follow_taking(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_taking(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_taking(act,foot, stack,sp, arg,key, buf,len);
	case _touchobj_:return touchobj_taking(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_taking(act,foot, stack,sp, arg,key, buf,len);

	case _gl41data_:return gl41data_taking(act,foot, stack,sp, arg,key, buf,len);
	case _gl41coop_:return gl41coop_taking(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_taking(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_taking(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_taking(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_taking(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_taking(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_taking(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_taking(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_taking(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_taking(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_taking(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_taking(act,foot, stack,sp, arg,key, buf,len);
	case _flycon_:return flycon_taking(act,foot, stack,sp, arg,key, buf,len);

	case _axis3d_:return axis3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_taking(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_taking(act,foot, stack,sp, arg,key, buf,len);

	case _reality_:return reality_taking(act,foot, stack,sp, arg,key, buf,len);
	case _virtual_:return virtual_taking(act,foot, stack,sp, arg,key, buf,len);

	case _htmlroot_:return htmlroot_taking(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_taking(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_taking(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_taking(act,foot, stack,sp, arg,key, buf,len);

	case _step_:return stepcar_taking(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_taking(act,foot, stack,sp, arg,key, buf,len);

	case _test_:return test_taking(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_taking(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->ontaking)return 0;
	return act->ontaking(act,foot, stack,sp, arg,key, buf,len);
}
int entitywrite(_ent* act,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(act->type){
	case _cam1rd_:return cam1rd_giving(act,foot, stack,sp, arg,key, buf,len);
	case _cam3rd_:return cam3rd_giving(act,foot, stack,sp, arg,key, buf,len);
	case _camrts_:return camrts_giving(act,foot, stack,sp, arg,key, buf,len);

	case _follow_:return follow_giving(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_giving(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_giving(act,foot, stack,sp, arg,key, buf,len);
	case _touchobj_:return touchobj_giving(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_giving(act,foot, stack,sp, arg,key, buf,len);

	case _gl41data_:return gl41data_giving(act,foot, stack,sp, arg,key, buf,len);
	case _gl41coop_:return gl41coop_giving(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_giving(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_giving(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_giving(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_giving(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_giving(act,foot, stack,sp, arg,key, buf,len);

	case _sch_:return schematic_giving(act,foot, stack,sp, arg,key, buf,len);
	case _pcb_:return printboard_giving(act,foot, stack,sp, arg,key, buf,len);
	case _analog_:return analog_giving(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_giving(act,foot, stack,sp, arg,key, buf,len);

	case _virtimu_:return virtimu_giving(act,foot, stack,sp, arg,key, buf,len);
	case _carcon_:return carcon_giving(act,foot, stack,sp, arg,key, buf,len);
	case _flycon_:return flycon_giving(act,foot, stack,sp, arg,key, buf,len);

	case _axis3d_:return axis3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_giving(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_giving(act,foot, stack,sp, arg,key, buf,len);

	case _reality_:return reality_giving(act,foot, stack,sp, arg,key, buf,len);
	case _virtual_:return virtual_giving(act,foot, stack,sp, arg,key, buf,len);

	case _htmlroot_:return htmlroot_giving(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_giving(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_giving(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_giving(act,foot, stack,sp, arg,key, buf,len);

	case _step_:return stepcar_giving(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_giving(act,foot, stack,sp, arg,key, buf,len);

	case _test_:return test_giving(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_giving(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->ongiving)return 0;
	return act->ongiving(act,foot, stack,sp, arg,key, buf,len);
}
int entitydiscon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_discon\n");
	switch(act->type){
	case _cam1rd_:return cam1rd_discon(self, peer);
	case _cam3rd_:return cam3rd_discon(self, peer);
	case _camrts_:return camrts_discon(self, peer);

	case _follow_:return follow_discon(self, peer);
	case _lookat_:return lookat_discon(self, peer);
	case _wander_:return wander_discon(self, peer);
	case _touchobj_:return touchobj_discon(self, peer);
	case _clickray_:return clickray_discon(self, peer);

	case _gl41data_:return gl41data_discon(self, peer);
	case _gl41coop_:return gl41coop_discon(self, peer);

	//case _field_:return field_discon(self, peer);
	//case _fluid_:return fluid_discon(self, peer);
	case _force_:return force_discon(self, peer);
	case _graveasy_:return graveasy_discon(self, peer);
	case _gravtest_:return gravtest_discon(self, peer);

	case _sch_:return schematic_discon(self, peer);
	case _pcb_:return printboard_discon(self, peer);
	case _analog_:return analog_discon(self, peer);
	case _digital_:return digital_discon(self, peer);

	case _virtimu_:return virtimu_discon(self, peer);
	case _carcon_:return carcon_discon(self, peer);
	case _flycon_:return flycon_discon(self, peer);

	case _axis3d_:return axis3d_discon(self, peer);
	case _guide3d_:return guide3d_discon(self, peer);
	case _border2d_:return border2d_discon(self, peer);
	case _border3d_:return border3d_discon(self, peer);
	case _scene3d_:return scene3d_discon(self, peer);

	case _reality_:return reality_discon(self, peer);
	case _virtual_:return virtual_discon(self, peer);

	case _htmlroot_:return htmlroot_discon(self, peer);
	case _xamlroot_:return xamlroot_discon(self, peer);
	case _mmio_:return mmiospace_discon(self, peer);
	case _port_:return portspace_discon(self, peer);

	case _step_:return stepcar_discon(self, peer);
	case _bdc_:return toycar_discon(self, peer);

	case _test_:return test_discon(self, peer);
	case _baby_:return baby_discon(self, peer);
	}

	if(0 == act->ondiscon)return 0;
	return act->ondiscon(self, peer);
}
int entitylinkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_linkup\n");
	switch(act->type){
	case _cam1rd_:return cam1rd_linkup(self, peer);
	case _cam3rd_:return cam3rd_linkup(self, peer);
	case _camrts_:return camrts_linkup(self, peer);

	case _follow_:return follow_linkup(self, peer);
	case _lookat_:return lookat_linkup(self, peer);
	case _wander_:return wander_linkup(self, peer);
	case _touchobj_:return touchobj_linkup(self, peer);
	case _clickray_:return clickray_linkup(self, peer);

	case _gl41data_:return gl41data_linkup(self, peer);
	case _gl41coop_:return gl41coop_linkup(self, peer);

	//case _field_:return field_linkup(self, peer);
	//case _fluid_:return fluid_linkup(self, peer);
	case _force_:return force_linkup(self, peer);
	case _graveasy_:return graveasy_linkup(self, peer);
	case _gravtest_:return gravtest_linkup(self, peer);

	case _sch_:return schematic_linkup(self, peer);
	case _pcb_:return printboard_linkup(self, peer);
	case _analog_:return analog_linkup(self, peer);
	case _digital_:return digital_linkup(self, peer);

	case _virtimu_:return virtimu_linkup(self, peer);
	case _carcon_:return carcon_linkup(self, peer);
	case _flycon_:return flycon_linkup(self, peer);

	case _axis3d_:return axis3d_linkup(self, peer);
	case _guide3d_:return guide3d_linkup(self, peer);
	case _border2d_:return border2d_linkup(self, peer);
	case _border3d_:return border3d_linkup(self, peer);
	case _scene3d_:return scene3d_linkup(self, peer);

	case _reality_:return reality_linkup(self, peer);
	case _virtual_:return virtual_linkup(self, peer);

	case _htmlroot_:return htmlroot_linkup(self, peer);
	case _xamlroot_:return xamlroot_linkup(self, peer);
	case _mmio_:return mmiospace_linkup(self, peer);
	case _port_:return portspace_linkup(self, peer);

	case _step_:return stepcar_linkup(self, peer);
	case _bdc_:return toycar_linkup(self, peer);

	case _test_:return test_linkup(self, peer);
	case _baby_:return baby_linkup(self, peer);
	}

	if(0 == act->onlinkup)return 0;
	return act->onlinkup(self, peer);
}




int entitydelete(void* p)
{
	if(0 == p)return 0;
	struct entity* act = p;

	switch(act->type){
		case _orig_:return 0;
		case _copy_:return 0;
		case _ORIG_:act->type = _orig_;
		case _COPY_:act->type = _copy_;
	}
	act->ondelete(act);

	return 0;
}
void* entitycreate(u64 type, void* buf, int argc, u8** argv)
{
	struct entity* act;
	//say("%llx,%llx\n", type, buf);

	switch(type){
//----------------world----------------
	case _reality_:
	{
		act = allocentity();
		act->fmt = act->type = _reality_;
		reality_create(act, buf, argc, argv);
		return act;
	}
	case _virtual_:
	{
		act = allocentity();
		act->fmt = act->type = _virtual_;
		virtual_create(act, buf, argc, argv);
		return act;
	}
	case _htmlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _htmlroot_;
		htmlroot_create(act, buf, argc, argv);
		return act;
	}
	case _xamlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _xamlroot_;
		xamlroot_create(act, buf, argc, argv);
		return act;
	}
	case _mmio_:
	{
		act = allocentity();
		act->fmt = act->type = _mmio_;
		mmiospace_create(act, buf, argc, argv);
		return act;
	}
	case _port_:
	{
		act = allocentity();
		act->fmt = act->type = _port_;
		portspace_create(act, buf, argc, argv);
		return act;
	}

	//circuit
	case _sch_:
	{
		act = allocentity();
		act->fmt = act->type = _sch_;
		schematic_create(act, buf, argc, argv);
		return act;
	}
	case _pcb_:
	{
		act = allocentity();
		act->fmt = act->type = _pcb_;
		printboard_create(act, buf, argc, argv);
		return act;
	}
	case _analog_:
	{
		act = allocentity();
		act->fmt = act->type = _analog_;
		analog_create(act, buf, argc, argv);
		return act;
	}
	case _digital_:
	{
		act = allocentity();
		act->fmt = act->type = _digital_;
		digital_create(act, buf, argc, argv);
		return act;
	}

	case _axis3d_:
	{
		act = allocentity();
		act->fmt = act->type = _axis3d_;
		axis3d_create(act, buf, argc, argv);
		return act;
	}
	case _guide3d_:
	{
		act = allocentity();
		act->fmt = act->type = _guide3d_;
		guide3d_create(act, buf, argc, argv);
		return act;
	}
	case _border2d_:
	{
		act = allocentity();
		act->fmt = act->type = _border2d_;
		border2d_create(act, buf, argc, argv);
		return act;
	}
	case _border3d_:
	{
		act = allocentity();
		act->fmt = act->type = _border3d_;
		border3d_create(act, buf, argc, argv);
		return act;
	}
	case _scene3d_:
	{
		act = allocentity();
		act->fmt = act->type = _scene3d_;
		scene3d_create(act, buf, argc, argv);
		return act;
	}

//----------------mind----------------
 	case _baby_:
	{
		act = allocentity();
		act->fmt = act->type = _baby_;
		baby_create(act, buf, argc, argv);
		return act;
	}
	case _test_:
	{
		act = allocentity();
		act->fmt = act->type = _test_;
		test_create(act, buf, argc, argv);
		return act;
	}

	//event
	case _cam1rd_:
	{
		act = allocentity();
		act->fmt = act->type = _cam1rd_;
		cam1rd_create(act, buf, argc, argv);
		return act;
	}
	case _cam3rd_:
	{
		act = allocentity();
		act->fmt = act->type = _cam3rd_;
		cam3rd_create(act, buf, argc, argv);
		return act;
	}
	case _camrts_:
	{
		act = allocentity();
		act->fmt = act->type = _camrts_;
		camrts_create(act, buf, argc, argv);
		return act;
	}

	case _follow_:
	{
		act = allocentity();
		act->fmt = act->type = _follow_;
		follow_create(act, buf, argc, argv);
		return act;
	}
	case _lookat_:
	{
		act = allocentity();
		act->fmt = act->type = _lookat_;
		lookat_create(act, buf, argc, argv);
		return act;
	}
	case _wander_:
	{
		act = allocentity();
		act->fmt = act->type = _wander_;
		wander_create(act, buf, argc, argv);
		return act;
	}
	case _touchobj_:
	{
		act = allocentity();
		act->fmt = act->type = _touchobj_;
		touchobj_create(act, buf, argc, argv);
		return act;
	}
	case _clickray_:
	{
		act = allocentity();
		act->fmt = act->type = _clickray_;
		clickray_create(act, buf, argc, argv);
		return act;
	}

//---------------robot----------------
	case _virtimu_:
	{
		act = allocentity();
		act->fmt = act->type = _virtimu_;
		virtimu_create(act, buf, argc, argv);
		return act;
	}
	case _carcon_:
	{
		act = allocentity();
		act->fmt = act->type = _carcon_;
		carcon_create(act, buf, argc, argv);
		return act;
	}
	case _flycon_:
	{
		act = allocentity();
		act->fmt = act->type = _flycon_;
		flycon_create(act, buf, argc, argv);
		return act;
	}

//----------------rule----------------

	//physic
	case _force_:
	{
		act = allocentity();
		act->fmt = act->type = _force_;
		force_create(act, buf, argc, argv);
		return act;
	}
	case _graveasy_:
	{
		act = allocentity();
		act->fmt = act->type = _graveasy_;
		graveasy_create(act, buf, argc, argv);
		return act;
	}
	case _gravtest_:
	{
		act = allocentity();
		act->fmt = act->type = _gravtest_;
		gravtest_create(act, buf, argc, argv);
		return act;
	}

//----------------other----------------
	case _bdc_:
	{
		act = allocentity();
		act->fmt = act->type = _bdc_;
		toycar_create(act, buf, argc, argv);
		return act;
	}
	case _step_:
	{
		act = allocentity();
		act->fmt = act->type = _step_;
		stepcar_create(act, buf, argc, argv);
		return act;
	}

	case _gl41data_:
	{
		act = allocentity();
		act->fmt = act->type = _gl41data_;
		gl41data_create(act, buf, argc, argv);
		return act;
	}
	case _gl41coop_:
	{
		act = allocentity();
		act->fmt = act->type = _gl41coop_;
		gl41coop_create(act, buf, argc, argv);
		return act;
	}
	}//switch

	//
	return entitycreate_clone(type, buf, argc, argv);
}
void* entitymodify(int argc, u8** argv)
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
		entitycreate(name, argv[3], argc-3, &argv[3]);
	}

	return 0;
}
void* entitysearch(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct entity* act;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			act = &entity[j];
			if(0 == act->fmt)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->fmt, &act->fmt);
		}
		if(0 == j)say("empty entity\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == entity[j].fmt)break;
			p = (void*)(&entity[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &entity[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freeentity()
{
	say("[e,f):entity freeing\n");

	mind_free();
	rule_free();
	thing_free();
	world_free();

	forcpu_free();
	formem_free();

	style = 0;
	entity = 0;

	say("[e,f):entity freeed\n");
}
void initentity(u8* addr)
{
	say("[e,f):entity initing\n");

	int j;
	entity = (void*)(addr+0x000000);
	style = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct entity))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)entity[j].tier = _ent_;

	formem_init(addr);
	forcpu_init(addr);

	world_init(addr);
	thing_init(addr);
	rule_init(addr);
	mind_init(addr);

	say("[e,f):entity inited\n");
}
