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
int baby_read(void*, void*, void*, int, void*, int);
int baby_write(void*, void*, void*, int, void*, int);
//
int test_create(void*, void*, int, u8**);
int test_delete(void*, void*);
int test_read(void*, void*, void*, int, void*, int);
int test_write(void*, void*, void*, int, void*, int);

//
int sch_create(void*, void*, int, u8**);
int sch_delete(void*, void*);
int sch_start(void*, void*);
int sch_stop(void*, void*);
int sch_write(void*, void*, void*, int, void*, int);
int sch_read(void*, void*, void*, int, void*, int);
int pcb_create(void*, void*, int, u8**);
int pcb_delete(void*, void*);
int pcb_start(void*, void*);
int pcb_stop(void*, void*);
int pcb_write(void*, void*, void*, int, void*, int);
int pcb_read(void*, void*, void*, int, void*, int);
int eeworld_create(void*, void*, int, u8**);
int eeworld_delete(void*, void*);
int eeworld_start(void*, void*);
int eeworld_stop(void*, void*);
int eeworld_write(void*, void*, void*, int, void*, int);
int eeworld_read(void*, void*, void*, int, void*, int);

//
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_start(void*, void*);
int scene3d_stop(void*, void*);
int scene3d_write(void*, void*, void*, int, void*, int);
int scene3d_read(void*, void*, void*, int, void*, int);
int world3d_create(void*, void*, int, u8**);
int world3d_delete(void*, void*);
int world3d_start(void*, void*);
int world3d_stop(void*, void*);
int world3d_write(void*, void*, void*, int, void*, int);
int world3d_read(void*, void*, void*, int, void*, int);

//
int htmlnode_create(void*, void*, int, u8**);
int htmlnode_delete(void*);
int htmlnode_write(void*, void*, void*, int, void*, int);
int htmlnode_read( void*, void*, void*, int, void*, int);

//
int reality_create(void*, void*, int, u8**);
int reality_delete(void*);
int reality_write(void*, void*, void*, int, void*, int);
int reality_read( void*, void*, void*, int, void*, int);

//gl41 helper
int gl41data_create(void*, void*, int, u8**);
int gl41data_delete(void*);
int gl41data_read( void*, void*, void*, int, void*, int);
int gl41data_write(void*, void*, void*, int, void*, int);
int gl41coop_create(void*, void*, int, u8**);
int gl41coop_delete(void*);
int gl41coop_read( void*, void*, void*, int, void*, int);
int gl41coop_write(void*, void*, void*, int, void*, int);

//event
int event3rd_create(void*, void*, int, u8**);
int event3rd_delete(void*);
int event3rd_read( void*, void*, void*, int, void*, int);
int event3rd_write(void*, void*, void*, int, void*, int);




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
	ent->onstart  = tmp->onstart;
	ent->onstop   = tmp->onstop;
	ent->onread   = tmp->onread;
	ent->onwrite  = tmp->onwrite;

	ent->oncreate(ent, arg, argc, argv);
	return ent;
}



/*
int entityinput_special(struct supply* win, struct style* sty, struct event* ev)
{
	int val;
	short* t;

	val = 0;
	if(_char_ == ev->what)
	{
		if(0x1b == ev->why)val = 'r';
	}
	else if(_kbd_ == ev->what)
	{
		if(0xfb == ev->why)val = 'l';
		else if(0xfc == ev->why)val = 'r';
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyl_select)val = 'l';
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyr_start)val = 'r';
	}

	if(('l' == val)|('r' == val))
	{
		//win->forew = 0x80;
		return 1;
	}
	return 0;
}
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
		case _event3rd_:return event3rd_read(self, peer, arg, idx, buf, len);
		case _gl41data_:return gl41data_read(self, peer, arg, idx, buf, len);
		case _gl41coop_:return gl41coop_read(self, peer, arg, idx, buf, len);
		case _world3d_:return world3d_read(self, peer, arg, idx, buf, len);
		case _scene3d_:return scene3d_read(self, peer, arg, idx, buf, len);
		case _eeworld_:return eeworld_read(self, peer, arg, idx, buf, len);
		case _sch_:return sch_read(self, peer, arg, idx, buf, len);
		case _pcb_:return pcb_read(self, peer, arg, idx, buf, len);
		case _reality_:return reality_read(self, peer, arg, idx, buf, len);
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
		case _event3rd_:return event3rd_write(self, peer, arg, idx, buf, len);
		case _gl41data_:return gl41data_write(self, peer, arg, idx, buf, len);
		case _gl41coop_:return gl41coop_write(self, peer, arg, idx, buf, len);
		case _world3d_:return world3d_write(self, peer, arg, idx, buf, len);
		case _scene3d_:return scene3d_write(self, peer, arg, idx, buf, len);
		case _eeworld_:return eeworld_write(self, peer, arg, idx, buf, len);
		case _sch_:return sch_write(self, peer, arg, idx, buf, len);
		case _pcb_:return pcb_write(self, peer, arg, idx, buf, len);
		case _reality_:return reality_write(self, peer, arg, idx, buf, len);
		case _html_:return htmlnode_write(self, peer, arg, idx, buf, len);
		case _test_:return test_write(self, peer, arg, idx, buf, len);
		case _baby_:return baby_write(self, peer, arg, idx, buf, len);
	}

	if(0 == act->onwrite)return 0;
	return act->onwrite(self, peer, arg, idx, buf, len);
}
int entitystop(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	if(0 == act->onstop)return 0;
	return act->onstop(self, peer);
}
int entitystart(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	say("@entity_start\n");
	if(0 == act->onstart)return 0;
	return act->onstart(self, peer);
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

	//html
	else if(_html_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _html_;
		htmlnode_create(act, buf, argc, argv);
		return act;
	}

	//reality
	else if(_reality_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _reality_;
		reality_create(act, buf, argc, argv);
		return act;
	}

	//circuit
	else if(_sch_ == type)
	{
		act = allocentity();
		act->fmt = act->type = _sch_;
		sch_create(act, buf, argc, argv);
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
