#include "libuser.h"
void baby_free();
void baby_init(void*);
void test_free();
void test_init(void*);
void mine_free();
void mine_init(void*);

//
int baby_create(void*, void*, int, u8**);
int baby_delete(void*, void*);
int baby_linkup(void*, void*);
int baby_discon(void*, void*);
int baby_read(void*, void*, void*, int, void*, int);
int baby_write(void*, void*, void*, int, void*, int);
int test_create(void*, void*, int, u8**);
int test_delete(void*, void*);
int test_linkup(void*, void*);
int test_discon(void*, void*);
int test_read(void*, void*, void*, int, void*, int);
int test_write(void*, void*, void*, int, void*, int);

//
int analog_create(void*, void*, int, u8**);
int analog_delete(void*, void*);
int analog_linkup(void*, void*);
int analog_discon(void*, void*);
int analog_write(void*, void*, void*, int, void*, int);
int analog_read(void*, void*, void*, int, void*, int);
int digital_create(void*, void*, int, u8**);
int digital_delete(void*, void*);
int digital_linkup(void*, void*);
int digital_discon(void*, void*);
int digital_write(void*, void*, void*, int, void*, int);
int digital_read(void*, void*, void*, int, void*, int);

//
int force_create(void*, void*, int, u8**);
int force_delete(void*, void*);
int force_linkup(void*, void*);
int force_discon(void*, void*);
int force_write(void*, void*, void*, int, void*, int);
int force_read(void*, void*, void*, int, void*, int);
int gravity_create(void*, void*, int, u8**);
int gravity_delete(void*, void*);
int gravity_linkup(void*, void*);
int gravity_discon(void*, void*);
int gravity_write(void*, void*, void*, int, void*, int);
int gravity_read(void*, void*, void*, int, void*, int);

//scene
int border2d_create(void*, void*, int, u8**);
int border2d_delete(void*, void*);
int border2d_linkup(void*, void*);
int border2d_discon(void*, void*);
int border2d_write(void*, void*, void*, int, void*, int);
int border2d_read(void*, void*, void*, int, void*, int);
int frame3d_create(void*, void*, int, u8**);
int frame3d_delete(void*, void*);
int frame3d_linkup(void*, void*);
int frame3d_discon(void*, void*);
int frame3d_write(void*, void*, void*, int, void*, int);
int frame3d_read(void*, void*, void*, int, void*, int);
int guide3d_create(void*, void*, int, u8**);
int guide3d_delete(void*, void*);
int guide3d_linkup(void*, void*);
int guide3d_discon(void*, void*);
int guide3d_write(void*, void*, void*, int, void*, int);
int guide3d_read(void*, void*, void*, int, void*, int);
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_linkup(void*, void*);
int scene3d_discon(void*, void*);
int scene3d_write(void*, void*, void*, int, void*, int);
int scene3d_read(void*, void*, void*, int, void*, int);

//
int reality_create(void*, void*, int, u8**);
int reality_delete(void*);
int reality_linkup(void*, void*);
int reality_discon(void*, void*);
int reality_write(void*, void*, void*, int, void*, int);
int reality_read( void*, void*, void*, int, void*, int);
int virtual_create(void*, void*, int, u8**);
int virtual_delete(void*, void*);
int virtual_linkup(void*, void*);
int virtual_discon(void*, void*);
int virtual_write(void*, void*, void*, int, void*, int);
int virtual_read(void*, void*, void*, int, void*, int);
int htmlroot_create(void*, void*, int, u8**);
int htmlroot_delete(void*);
int htmlroot_linkup(void*, void*);
int htmlroot_discon(void*, void*);
int htmlroot_write(void*, void*, void*, int, void*, int);
int htmlroot_read( void*, void*, void*, int, void*, int);
int uiuxroot_create(void*, void*, int, u8**);
int uiuxroot_delete(void*, void*);
int uiuxroot_linkup(void*, void*);
int uiuxroot_discon(void*, void*);
int uiuxroot_write(void*, void*, void*, int, void*, int);
int uiuxroot_read(void*, void*, void*, int, void*, int);

//gl41 helper
int gl41data_create(void*, void*, int, u8**);
int gl41data_delete(void*);
int gl41data_linkup(void*, void*);
int gl41data_discon(void*, void*);
int gl41data_read( void*, void*, void*, int, void*, int);
int gl41data_write(void*, void*, void*, int, void*, int);
int gl41coop_create(void*, void*, int, u8**);
int gl41coop_delete(void*);
int gl41coop_linkup(void*, void*);
int gl41coop_discon(void*, void*);
int gl41coop_read( void*, void*, void*, int, void*, int);
int gl41coop_write(void*, void*, void*, int, void*, int);

//event
int event3rd_create(void*, void*, int, u8**);
int event3rd_delete(void*);
int event3rd_linkup(void*, void*);
int event3rd_discon(void*, void*);
int event3rd_read( void*, void*, void*, int, void*, int);
int event3rd_write(void*, void*, void*, int, void*, int);
int eventrts_create(void*, void*, int, u8**);
int eventrts_delete(void*);
int eventrts_linkup(void*, void*);
int eventrts_discon(void*, void*);
int eventrts_read( void*, void*, void*, int, void*, int);
int eventrts_write(void*, void*, void*, int, void*, int);
int clickray_create(void*, void*, int, u8**);
int clickray_delete(void*);
int clickray_linkup(void*, void*);
int clickray_discon(void*, void*);
int clickray_read( void*, void*, void*, int, void*, int);
int clickray_write(void*, void*, void*, int, void*, int);




static struct entity* entity = 0;
static int actlen = 0;
static struct style* style = 0;
static int stylen = 0;
void* allocentity()
{
	int j,max;
	struct entity* act;

	max = 0x100000 / sizeof(struct entity);
	for(j=0;j<max;j++)
	{
		act = &entity[j];
		if(0 == act->type)return act;
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
	ent->onread   = tmp->onread;
	ent->onwrite  = tmp->onwrite;

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




int entityread(struct halfrel* self,struct halfrel* peer, void* arg,int idx, void* buf,int len)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	switch(act->type){
	case _clickray_:return clickray_read(self, peer, arg, idx, buf, len);
	case _event3rd_:return event3rd_read(self, peer, arg, idx, buf, len);
	case _eventrts_:return eventrts_read(self, peer, arg, idx, buf, len);

	case _gl41data_:return gl41data_read(self, peer, arg, idx, buf, len);
	case _gl41coop_:return gl41coop_read(self, peer, arg, idx, buf, len);

	//case _field_:return field_read(self, peer, arg, idx, buf, len);
	//case _fluid_:return fluid_read(self, peer, arg, idx, buf, len);
	case _force_:return force_read(self, peer, arg, idx, buf, len);
	case _gravity_:return gravity_read(self, peer, arg, idx, buf, len);

	case _analog_:return analog_read(self, peer, arg, idx, buf, len);
	case _digital_:return digital_read(self, peer, arg, idx, buf, len);

	case _border2d_:return border2d_read(self, peer, arg, idx, buf, len);
	case _frame3d_:return frame3d_read(self, peer, arg, idx, buf, len);
	case _guide3d_:return guide3d_read(self, peer, arg, idx, buf, len);
	case _scene3d_:return scene3d_read(self, peer, arg, idx, buf, len);

	case _reality_:return reality_read(self, peer, arg, idx, buf, len);
	case _virtual_:return virtual_read(self, peer, arg, idx, buf, len);
	case _htmlroot_:return htmlroot_read(self, peer, arg, idx, buf, len);
	//case _xamlroot_:return xamlroot_read(self, peer, arg, idx, buf, len);
	case _uiuxroot_:return uiuxroot_read(self, peer, arg, idx, buf, len);

	case _test_:return test_read(self, peer, arg, idx, buf, len);
	case _baby_:return baby_read(self, peer, arg, idx, buf, len);
	}

	if(0 == act->onread)return 0;
	return act->onread(self, peer, arg, idx, buf, len);
}
int entitywrite(struct halfrel* self,struct halfrel* peer, void* arg,int idx, void* buf,int len)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	switch(act->type){
	case _clickray_:return clickray_write(self, peer, arg, idx, buf, len);
	case _event3rd_:return event3rd_write(self, peer, arg, idx, buf, len);
	case _eventrts_:return eventrts_write(self, peer, arg, idx, buf, len);

	case _gl41data_:return gl41data_write(self, peer, arg, idx, buf, len);
	case _gl41coop_:return gl41coop_write(self, peer, arg, idx, buf, len);

	//case _field_:return field_write(self, peer, arg, idx, buf, len);
	//case _fluid_:return fluid_write(self, peer, arg, idx, buf, len);
	case _force_:return force_write(self, peer, arg, idx, buf, len);
	case _gravity_:return gravity_write(self, peer, arg, idx, buf, len);

	case _analog_:return analog_write(self, peer, arg, idx, buf, len);
	case _digital_:return digital_write(self, peer, arg, idx, buf, len);

	case _border2d_:return border2d_write(self, peer, arg, idx, buf, len);
	case _frame3d_:return frame3d_write(self, peer, arg, idx, buf, len);
	case _guide3d_:return guide3d_write(self, peer, arg, idx, buf, len);
	case _scene3d_:return scene3d_write(self, peer, arg, idx, buf, len);

	case _reality_:return reality_write(self, peer, arg, idx, buf, len);
	case _virtual_:return virtual_write(self, peer, arg, idx, buf, len);
	case _htmlroot_:return htmlroot_write(self, peer, arg, idx, buf, len);
	//case _xamlroot_:return xamlroot_write(self, peer, arg, idx, buf, len);
	case _uiuxroot_:return uiuxroot_write(self, peer, arg, idx, buf, len);

	case _test_:return test_write(self, peer, arg, idx, buf, len);
	case _baby_:return baby_write(self, peer, arg, idx, buf, len);
	}

	if(0 == act->onwrite)return 0;
	return act->onwrite(self, peer, arg, idx, buf, len);
}
int entitydiscon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_discon\n");
	switch(act->type){
	case _clickray_:return clickray_discon(self, peer);
	case _event3rd_:return event3rd_discon(self, peer);
	case _eventrts_:return eventrts_discon(self, peer);

	case _gl41data_:return gl41data_discon(self, peer);
	case _gl41coop_:return gl41coop_discon(self, peer);

	//case _field_:return field_discon(self, peer);
	//case _fluid_:return fluid_discon(self, peer);
	case _force_:return force_discon(self, peer);
	case _gravity_:return gravity_discon(self, peer);

	case _analog_:return analog_discon(self, peer);
	case _digital_:return digital_discon(self, peer);

	case _border2d_:return border2d_discon(self, peer);
	case _frame3d_:return frame3d_discon(self, peer);
	case _guide3d_:return guide3d_discon(self, peer);
	case _scene3d_:return scene3d_discon(self, peer);

	case _reality_:return reality_discon(self, peer);
	case _virtual_:return virtual_discon(self, peer);
	case _htmlroot_:return htmlroot_discon(self, peer);
	//case _xamlroot_:return xamlroot_discon(self, peer);
	case _uiuxroot_:return uiuxroot_discon(self, peer);

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
	case _clickray_:return clickray_linkup(self, peer);
	case _event3rd_:return event3rd_linkup(self, peer);
	case _eventrts_:return eventrts_linkup(self, peer);

	case _gl41data_:return gl41data_linkup(self, peer);
	case _gl41coop_:return gl41coop_linkup(self, peer);

	//case _field_:return field_linkup(self, peer);
	//case _fluid_:return fluid_linkup(self, peer);
	case _force_:return force_linkup(self, peer);
	case _gravity_:return gravity_linkup(self, peer);

	case _analog_:return analog_linkup(self, peer);
	case _digital_:return digital_linkup(self, peer);

	case _border2d_:return border2d_linkup(self, peer);
	case _frame3d_:return frame3d_linkup(self, peer);
	case _guide3d_:return guide3d_linkup(self, peer);
	case _scene3d_:return scene3d_linkup(self, peer);

	case _reality_:return reality_linkup(self, peer);
	case _virtual_:return virtual_linkup(self, peer);
	case _htmlroot_:return htmlroot_linkup(self, peer);
	//case _xamlroot_:return xamlroot_linkup(self, peer);
	case _uiuxroot_:return uiuxroot_linkup(self, peer);

	case _test_:return test_linkup(self, peer);
	case _baby_:return baby_linkup(self, peer);
	}

	if(0 == act->onlinkup)return 0;
	return act->onlinkup(self, peer);
}




int entitydelete(struct entity* act)
{
	if(0 == act)return 0;
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
	case _uiuxroot_:
	{
		act = allocentity();
		act->fmt = act->type = _uiuxroot_;
		uiuxroot_create(act, buf, argc, argv);
		return act;
	}
	case _htmlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _htmlroot_;
		htmlroot_create(act, buf, argc, argv);
		return act;
	}
/*	case _xamlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _xamlroot_;
		xamlroot_create(act, buf, argc, argv);
		return act;
	}*/

	case _border2d_:
	{
		act = allocentity();
		act->fmt = act->type = _border2d_;
		border2d_create(act, buf, argc, argv);
		return act;
	}
	case _frame3d_:
	{
		act = allocentity();
		act->fmt = act->type = _frame3d_;
		frame3d_create(act, buf, argc, argv);
		return act;
	}
	case _guide3d_:
	{
		act = allocentity();
		act->fmt = act->type = _guide3d_;
		guide3d_create(act, buf, argc, argv);
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
	case _clickray_:
	{
		act = allocentity();
		act->fmt = act->type = _clickray_;
		clickray_create(act, buf, argc, argv);
		return act;
	}
	case _event3rd_:
	{
		act = allocentity();
		act->fmt = act->type = _event3rd_;
		event3rd_create(act, buf, argc, argv);
		return act;
	}
	case _eventrts_:
	{
		act = allocentity();
		act->fmt = act->type = _eventrts_;
		eventrts_create(act, buf, argc, argv);
		return act;
	}

//----------------rule----------------
	//circuit
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

	//physic
	case _force_:
	{
		act = allocentity();
		act->fmt = act->type = _force_;
		force_create(act, buf, argc, argv);
		return act;
	}
	case _gravity_:
	{
		act = allocentity();
		act->fmt = act->type = _gravity_;
		gravity_create(act, buf, argc, argv);
		return act;
	}

//----------------other----------------
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
	say("[e,f):freeing entity\n");
	mine_free();
	test_free();
	baby_free();

	style = 0;
	entity = 0;
}
void initentity(u8* addr)
{
	int j;
	entity = (void*)(addr+0x000000);
	style = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct entity))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)entity[j].tier = _ent_;

	baby_init(addr);
	test_init(addr);
	mine_init(addr);
	say("[e,f):inited entity\n");
}
