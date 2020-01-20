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
int htmlnode_create(void*, void*, int, u8**);
int htmlnode_delete(void*);
int htmlnode_linkup(void*, void*);
int htmlnode_discon(void*, void*);
int htmlnode_write(void*, void*, void*, int, void*, int);
int htmlnode_read( void*, void*, void*, int, void*, int);

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
int pcb_create(void*, void*, int, u8**);
int pcb_delete(void*, void*);
int pcb_linkup(void*, void*);
int pcb_discon(void*, void*);
int pcb_write(void*, void*, void*, int, void*, int);
int pcb_read(void*, void*, void*, int, void*, int);
int eeworld_create(void*, void*, int, u8**);
int eeworld_delete(void*, void*);
int eeworld_linkup(void*, void*);
int eeworld_discon(void*, void*);
int eeworld_write(void*, void*, void*, int, void*, int);
int eeworld_read(void*, void*, void*, int, void*, int);

//
int force_create(void*, void*, int, u8**);
int force_delete(void*, void*);
int force_linkup(void*, void*);
int force_discon(void*, void*);
int force_write(void*, void*, void*, int, void*, int);
int force_read(void*, void*, void*, int, void*, int);
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_linkup(void*, void*);
int scene3d_discon(void*, void*);
int scene3d_write(void*, void*, void*, int, void*, int);
int scene3d_read(void*, void*, void*, int, void*, int);
int world3d_create(void*, void*, int, u8**);
int world3d_delete(void*, void*);
int world3d_linkup(void*, void*);
int world3d_discon(void*, void*);
int world3d_write(void*, void*, void*, int, void*, int);
int world3d_read(void*, void*, void*, int, void*, int);
int reality_create(void*, void*, int, u8**);
int reality_delete(void*);
int reality_linkup(void*, void*);
int reality_discon(void*, void*);
int reality_write(void*, void*, void*, int, void*, int);
int reality_read( void*, void*, void*, int, void*, int);

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
	ent->onlinkup  = tmp->onlinkup;
	ent->ondiscon   = tmp->ondiscon;
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

	case _gl41data_:return gl41data_read(self, peer, arg, idx, buf, len);
	case _gl41coop_:return gl41coop_read(self, peer, arg, idx, buf, len);

	case _force_:return force_read(self, peer, arg, idx, buf, len);
	case _scene3d_:return scene3d_read(self, peer, arg, idx, buf, len);
	case _world3d_:return world3d_read(self, peer, arg, idx, buf, len);
	case _reality_:return reality_read(self, peer, arg, idx, buf, len);

	case _analog_:return analog_read(self, peer, arg, idx, buf, len);
	case _digital_:return digital_read(self, peer, arg, idx, buf, len);
	case _pcb_:return pcb_read(self, peer, arg, idx, buf, len);
	case _eeworld_:return eeworld_read(self, peer, arg, idx, buf, len);

	case _html_:return htmlnode_read(self, peer, arg, idx, buf, len);
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

	case _gl41data_:return gl41data_write(self, peer, arg, idx, buf, len);
	case _gl41coop_:return gl41coop_write(self, peer, arg, idx, buf, len);

	case _force_:return force_write(self, peer, arg, idx, buf, len);
	case _scene3d_:return scene3d_write(self, peer, arg, idx, buf, len);
	case _world3d_:return world3d_write(self, peer, arg, idx, buf, len);
	case _reality_:return reality_write(self, peer, arg, idx, buf, len);

	case _analog_:return analog_write(self, peer, arg, idx, buf, len);
	case _digital_:return digital_write(self, peer, arg, idx, buf, len);
	case _pcb_:return pcb_write(self, peer, arg, idx, buf, len);
	case _eeworld_:return eeworld_write(self, peer, arg, idx, buf, len);

	case _html_:return htmlnode_write(self, peer, arg, idx, buf, len);
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

	case _gl41data_:return gl41data_discon(self, peer);
	case _gl41coop_:return gl41coop_discon(self, peer);

	case _scene3d_:return scene3d_discon(self, peer);
	case _world3d_:return world3d_discon(self, peer);
	case _reality_:return reality_discon(self, peer);

	case _analog_:return analog_discon(self, peer);
	case _digital_:return digital_discon(self, peer);
	case _pcb_:return pcb_discon(self, peer);
	case _eeworld_:return eeworld_discon(self, peer);

	case _html_:return htmlnode_discon(self, peer);
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

	case _gl41data_:return gl41data_linkup(self, peer);
	case _gl41coop_:return gl41coop_linkup(self, peer);

	case _scene3d_:return scene3d_linkup(self, peer);
	case _world3d_:return world3d_linkup(self, peer);
	case _reality_:return reality_linkup(self, peer);

	case _analog_:return analog_linkup(self, peer);
	case _digital_:return digital_linkup(self, peer);
	case _pcb_:return pcb_linkup(self, peer);
	case _eeworld_:return eeworld_linkup(self, peer);

	case _html_:return htmlnode_linkup(self, peer);
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
	say("%llx,%llx\n", type, buf);
/*
	if(0 == type)
	{
		act = buf;
		if(0 == act)return 0;
		if(_ORIG_ == act->type)return 0;
		if(_COPY_ == act->type)return 0;

		act->oncreate(act, 0, argc, argv);
	}
*/
	//test
	if(_baby_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _baby_;
		baby_create(act, buf, argc, argv);
		return act;
	}
	else if(_test_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _test_;
		test_create(act, buf, argc, argv);
		return act;
	}
	else if(_html_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _html_;
		htmlnode_create(act, buf, argc, argv);
		return act;
	}

	//circuit
	else if(_analog_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _analog_;
		analog_create(act, buf, argc, argv);
		return act;
	}
	else if(_digital_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _digital_;
		digital_create(act, buf, argc, argv);
		return act;
	}
	else if(_pcb_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _pcb_;
		pcb_create(act, buf, argc, argv);
		return act;
	}
	else if(_eeworld_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _eeworld_;
		eeworld_create(act, buf, argc, argv);
		return act;
	}

	//world
	else if(_force_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _force_;
		force_create(act, buf, argc, argv);
		return act;
	}
	else if(_scene3d_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _scene3d_;
		scene3d_create(act, buf, argc, argv);
		return act;
	}
	else if(_world3d_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _world3d_;
		world3d_create(act, buf, argc, argv);
		return act;
	}
	else if(_reality_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _reality_;
		reality_create(act, buf, argc, argv);
		return act;
	}


	//render
	else if(_gl41data_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _gl41data_;
		gl41data_create(act, buf, argc, argv);
		return act;
	}
	else if(_gl41coop_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _gl41coop_;
		gl41coop_create(act, buf, argc, argv);
		return act;
	}

	//event
	else if(_clickray_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _clickray_;
		clickray_create(act, buf, argc, argv);
		return act;
	}
	else if(_event3rd_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _event3rd_;
		event3rd_create(act, buf, argc, argv);
		return act;
	}

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
	//say("[e,f):deleteing entity\n");
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
	//say("[e,f):createed entity\n");
}
