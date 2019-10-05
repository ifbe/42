#include "libuser.h"




void initjoy(void*);
void freejoy();
int joycreate(void*, void*);
int joydelete(void*);
//
void initstd(void*);
void freestd();
int stdcreate(void*, void*);
int stddelete(void*);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*);
int traydelete(void*);
//micphone
void initmicphone(void*);
void freemicphone();
int micphonecreate(void*, void*);
int micphonedelete(void*);
int micphonestart(void*);
int micphonestop(void*);
int micphoneread(void*, void*, void*, int, void*, int);
int micphonewrite(void*, void*, void*, int, void*, int);
int micphonelist();
int micphonechoose();
//speaker
void initspeaker(void*);
void freespeaker();
int speakercreate(void*, void*);
int speakerdelete(void*);
int speakerstart(void*);
int speakerstop(void*);
int speakerread(void*, void*, void*, int, void*, int);
int speakerwrite(void*, void*, void*, int, void*, int);
int speakerlist();
int speakerchoose();
//cam
void initcam(void*);
void freecam();
int videocreate(void*, void*);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void*, void*, void*, int, void*, int);
int videowrite(void*, void*, void*, int, void*, int);
int videolist();
int videochoose();
//window
void initwindow(void*);
void freewindow();
int windowcreate(void*);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void*, void*, void*, int, void*, int);
int windowwrite(void*, void*, void*, int, void*, int);
int windowlist();
int windowchoose();
//
int ahrs_create(void*, void*);
int ahrs_delete(void*);
int ahrs_read(void*, void*, void*, int, void*, int);
int ahrs_write(void*, void*, void*, int, void*, int);
//
int toycar_create(void*, void*);
int toycar_delete(void*);
int toycar_read(void*, void*, void*, int, void*, int);
int toycar_write(void*, void*, void*, int, void*, int);
//
int stepcar_create(void*, void*);
int stepcar_delete(void*, void*);
int stepcar_read(void*, void*, void*, int, void*, int);
int stepcar_write(void*, void*, void*, int, void*, int);
//
int khala_create(void*, void*);
//
int loopback_create(void*, void*);
//
int actorevent(struct event* ev);
int input(void*, int);
//
int cmp(void*, void*);
int ncmp(void*, void*, int);
int parsexml_detail(void*, int, void*, void*, void*, void*);




//
static struct arena* arena = 0;
static int winlen = 0;
static struct style* style = 0;
static int stylen = 0;
void* allocarena()
{
	int j;
	struct arena* win;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
	}
	if(j >= 0x100)return 0;

	win = &arena[j];
	win->irel0 = win->ireln = 0;
	win->orel0 = win->oreln = 0;
	return win;
}
void* allocstyle()
{
#define maxlen 0x200
	int j;
	u8* buf;

	buf = (void*)style + stylen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	stylen += maxlen;
	return buf;
}




int arenaread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct arena* win = self->pchip;

	switch(win->type){
		case _cam_:return videoread(self, peer, arg, idx, buf, len);
		case _bdc_:return toycar_read(self, peer, arg, idx, buf, len);
		case _step_:return stepcar_read(self, peer, arg, idx, buf, len);
	}

	return 0;
}
int arenawrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct arena* win = self->pchip;

	switch(win->fmt){
		case _bdc_:return toycar_write(self, peer, arg, idx, buf, len);
		case _step_:return stepcar_write(self, peer, arg, idx, buf, len);
		case _ahrs_:return ahrs_write(self, peer, arg, idx, buf, len);
	}

	return 0;
}
int arenastop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int arenastart(struct halfrel* self, struct halfrel* peer)
{
	struct arena* win;
	if(0 == self)return 0;

	win = (void*)(self->chip);
	if(0 == win)return 0;

	say("@arena_start\n");
	return 0;
}




int arenadelete(struct arena* win)
{
	if(win == 0)return 0;

	//1.close
	windowstop(win);

	//2.unlink
	win->irel0 = 0;
	win->ireln = 0;
	win->orel0 = 0;
	win->oreln = 0;

	//3.cleanup
	win->type = 0;
	win->fmt = 0;
	return 0;
}
void* arenacreate(u64 type, void* addr, int argc, char** argv)
{
	int j = 0;
	struct arena* win;
	struct arena* sub;

	//default
	if(0 == type)
	{
		if(arena[0].type)return 0;
		type = _win_;
	}

	//0: system object
	if(_joy_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _joy_;
		win->fmt = _joy_;
		joycreate(win, addr);
		return win;
	}
	else if(_std_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _std_;
		win->fmt = _std_;
		stdcreate(win, addr);
		return win;
	}
	else if(_tray_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _tray_;
		win->fmt = _tray_;
		traycreate(win, addr);
		return win;
	}

	//
	else if(_ahrs_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _ahrs_;
		win->fmt = _ahrs_;
		ahrs_create(win, addr);
		return win;
	}

	//
	else if(_bdc_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _bdc_;
		win->fmt = _bdc_;
		toycar_create(win, addr);
		return win;
	}

	//micphone
	else if(_mic_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		micphonecreate(win, addr);
		return win;
	}

	//speaker
	else if(_spk_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _spk_;
		win->fmt = hex32('p','c','m',0);
		speakercreate(win, addr);
		return win;
	}

	//video
	else if(_cam_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videocreate(win, addr);
		return win;
	}
	else if(_cap_ == type)
	{
		return win;
	}

	//window
	else if(_none_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _win_;
		win->fmt = _none_;
		windowcreate(win);
		return win;
	}
	else if(_easy_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _win_;
		win->fmt = _easy_;
		windowcreate(win);
		return win;
	}
	else if(_win_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _win_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowcreate(win);
		return win;
	}
	else if(_coop_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		if(addr)relationcreate(addr, 0, _win_, 0, win, 0, _win_, 0);
		win->type = _win_;
		win->fmt = _coop_;
		windowcreate(win);
		return win;
	}

	else if(_khala_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _w2w_;
		win->fmt = _khala_;
		khala_create(win, addr);
		return win;
	}
	else if(_loopback_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _w2w_;
		win->fmt = _loopback_;
		loopback_create(win, addr);
		return win;
	}

	return 0;
}
void* arenamodify(int argc, char** argv)
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
		arenacreate(name, argv[3], argc-3, &argv[3]);
	}

	return 0;
}
void* arenasearch(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct arena* win;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			win = &arena[j];
			if(0 == win->type)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&win->tier, &win->type, &win->fmt, &win->vfmt);
		}
		if(0 == j)say("empty arena\n");
	}
	else
	{
/*
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
*/
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].fmt)break;
			p = (void*)(&arena[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &arena[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




int arenaevent(struct event* e)
{
	int j;
	struct event ev;
	struct arena* win;

	ev.why = e->why;
	ev.what = e->what;
	ev.where = e->where;
	ev.when = e->when;

	if(0 == ev.where)
	{
		//from cmd
		if(_char_ == ev.what)
		{
			input(&ev.why, 0);
			return 0;
		}

		//maybe gamepad
		for(j=0;j<16;j++)
		{
			win = &arena[j];
			if(_win_ == win->type)
			{
				ev.where = (u64)win;
				break;
			}
		}
	}

	win = (void*)(ev.where);
	if(0 == win)return 0;

	switch(win->fmt)
	{
		case _none_:
		case _easy_:
		case _full_:
		case _coop_:
		default:windowwrite(win, 0, 0, 0, &ev, 0);break;
	}
	return 0;
}
int arenaread_all()
{
	int j;
	struct arena* win;
	struct halfrel self;

	for(j=31;j>=0;j--)
	{
		win = &arena[j];
		if(0 == win->type)continue;

		if(_win_ == win->type){
			windowread(win, 0, 0, 0, 0, 0);
		}
		if(_spk_ == win->type){
			self.chip = (u64)win;
			speakerread(&self, 0, 0, 0, 0, 0);
		}
	}
	return 0;
}
void freearena()
{
	//say("[c,e):freeing arena\n");

	freewindow();
	freetray();
	freestd();
	freejoy();
}
void initarena(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct arena))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)arena[j].tier = _win_;

	initstd(arena);
	initwindow(arena);
	inittray(arena);
	initjoy(arena);

	//say("[c,e):inited arena\n");
}
