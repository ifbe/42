#include "libuser.h"




//
int cmp(void*, void*);
int ncmp(void*, void*, int);
//
void initjoy(void*);
void freejoy();
int joycreate(void*, void*, int, u8**);
int joydelete(void*);
//
void initstd(void*);
void freestd();
int stdcreate(void*, void*, int, u8**);
int stddelete(void*);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*, int, u8**);
int traydelete(void*);
//
int gravity_create(void*, void*, int, u8**);
int gravity_delete(void*);
int gravity_read(void*, void*, void*, int, void*, int);
int gravity_write(void*, void*, void*, int, void*, int);
//
int collide_create(void*, void*, int, u8**);
int collide_delete(void*);
int collide_read(void*, void*, void*, int, void*, int);
int collide_write(void*, void*, void*, int, void*, int);
//
int ahrs_create(void*, void*, int, u8**);
int ahrs_delete(void*);
int ahrs_read(void*, void*, void*, int, void*, int);
int ahrs_write(void*, void*, void*, int, void*, int);
//
int slam_create(void*, void*, int, u8**);
int slam_delete(void*);
int slam_read(void*, void*, void*, int, void*, int);
int slam_write(void*, void*, void*, int, void*, int);
//
int toycar_create(void*, void*, int, u8**);
int toycar_delete(void*);
int toycar_read(void*, void*, void*, int, void*, int);
int toycar_write(void*, void*, void*, int, void*, int);
//
int stepcar_create(void*, void*, int, u8**);
int stepcar_delete(void*, void*);
int stepcar_read(void*, void*, void*, int, void*, int);
int stepcar_write(void*, void*, void*, int, void*, int);
//sound.usbmic
void initmicphone(void*);
void freemicphone();
int micphonecreate(void*, void*, int, u8**);
int micphonedelete(void*);
int micphonestart(void*);
int micphonestop(void*);
int micphoneread(void*, void*, void*, int, void*, int);
int micphonewrite(void*, void*, void*, int, void*, int);
int micphonelist();
int micphonechoose();
//sound.speaker
void initspeaker(void*);
void freespeaker();
int speakercreate(void*, void*, int, u8**);
int speakerdelete(void*);
int speakerstart(void*);
int speakerstop(void*);
int speakerread(void*, void*, void*, int, void*, int);
int speakerwrite(void*, void*, void*, int, void*, int);
int speakerlist();
int speakerchoose();
//light.usbcam
void initcam(void*);
void freecam();
int videocreate(void*, void*, int, u8**);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void*, void*, void*, int, void*, int);
int videowrite(void*, void*, void*, int, void*, int);
int videolist();
int videochoose();
//light.window
void initwindow(void*);
void freewindow();
int windowcreate(void*, void*, int, u8**);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void*, void*, void*, int, void*, int);
int windowwrite(void*, void*, void*, int, void*, int);
int windowlist();
int windowchoose();




//
static struct supply* supply = 0;
static int winlen = 0;
static struct style* pinid = 0;
static int pinlen = 0;
void* allocsupply()
{
	int j;
	struct supply* win;
	for(j=0;j<0x100;j++)
	{
		if(0 == supply[j].type)break;
	}
	if(j >= 0x100)return 0;

	win = &supply[j];
	win->irel0 = win->ireln = 0;
	win->orel0 = win->oreln = 0;
	return win;
}
void* allocpinid()
{
#define maxlen 0x200
	int j;
	u8* buf;

	buf = (void*)pinid + pinlen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	pinlen += maxlen;
	return buf;
}




int supplyread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* win = self->pchip;

	switch(win->type){
		case _wnd_:windowread(self, peer, arg, idx, buf, len);return 0;
		case _spk_:speakerread(self, peer, arg, idx, buf, len);return 0;
	}
	switch(win->fmt){
		case _collide_:return collide_read(self, peer, arg, idx, buf, len);
		case _gravity_:return gravity_read(self, peer, arg, idx, buf, len);
		case _cam_:return videoread(self, peer, arg, idx, buf, len);
		case _bdc_:return toycar_read(self, peer, arg, idx, buf, len);
		case _step_:return stepcar_read(self, peer, arg, idx, buf, len);
		case _gl41fboc_:
		case _gl41fbod_:
		case _gl41fbog_:
		case _gl41wnd0_:return windowread(self,peer,arg,idx,buf,len);
	}

	return 0;
}
int supplywrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* win = self->pchip;

	switch(win->type){
		case _wnd_:windowwrite(self, peer, arg, idx, buf, len);return 0;
		case _spk_:speakerwrite(self, peer, arg, idx, buf, len);return 0;
	}
	switch(win->fmt){
		case _collide_:return collide_write(self, peer, arg, idx, buf, len);
		case _gravity_:return gravity_write(self, peer, arg, idx, buf, len);
		case _ahrs_:return ahrs_write(self, peer, arg, idx, buf, len);
		case _slam_:return slam_write(self, peer, arg, idx, buf, len);
		case _bdc_:return toycar_write(self, peer, arg, idx, buf, len);
		case _step_:return stepcar_write(self, peer, arg, idx, buf, len);
		case _gl41fboc_:
		case _gl41fbod_:
		case _gl41fbog_:
		case _gl41wnd0_:return windowwrite(self,peer,arg,idx,buf,len);
	}

	return 0;
}
int supplydiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int supplylinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@supplylinkup\n");

	struct supply* win;
	if(0 == self)return 0;

	win = (void*)(self->chip);
	if(0 == win)return 0;

	return 0;
}




int supplydelete(struct supply* win)
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
void* supplycreate(u64 type, void* arg, int argc, u8** argv)
{
	int j = 0;
	struct supply* win;
	struct supply* sub;

	//default
	if(0 == type)
	{
		if(supply[0].type)return 0;
		type = _sup_;
	}

	//0: system object
	if(_joy_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _joy_;
		win->fmt = _joy_;
		joycreate(win, arg, argc, argv);
		return win;
	}
	else if(_std_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _std_;
		win->fmt = _std_;
		stdcreate(win, arg, argc, argv);
		return win;
	}
	else if(_tray_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _tray_;
		win->fmt = _tray_;
		traycreate(win, arg, argc, argv);
		return win;
	}

	//phys
	else if(_gravity_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _phys_;
		win->fmt = _gravity_;
		gravity_create(win, arg, argc, argv);
		return win;
	}
	else if(_collide_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _phys_;
		win->fmt = _collide_;
		collide_create(win, arg, argc, argv);
		return win;
	}

	//sensor
	else if(_ahrs_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _ahrs_;
		ahrs_create(win, arg, argc, argv);
		return win;
	}
	else if(_slam_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _slam_;
		slam_create(win, arg, argc, argv);
		return win;
	}

	//motor
	else if(_bdc_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _car_;
		win->fmt = _bdc_;
		toycar_create(win, arg, argc, argv);
		return win;
	}
	else if(_step_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _car_;
		win->fmt = _step_;
		stepcar_create(win, arg, argc, argv);
		return win;
	}

	//micphone
	else if(_mic_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		micphonecreate(win, arg, argc, argv);
		return win;
	}

	//speaker
	else if(_spk_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _spk_;
		win->fmt = hex32('p','c','m',0);
		speakercreate(win, arg, argc, argv);
		return win;
	}

	//video
	else if(_cam_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videocreate(win, arg, argc, argv);
		return win;
	}
	else if(_cap_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('h','o','l','o');
		//hologram_create(win, arg, argc, argv);
		return win;
	}

	//window
	else if(_none_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _none_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_easy_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _easy_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_wnd_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_coop_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _coop_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_gl41fboc_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->fmt = win->type = _gl41fboc_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_gl41fbod_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->fmt = win->type = _gl41fbod_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_gl41fbog_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->fmt = win->type = _gl41fbog_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	else if(_gl41wnd0_ == type)
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->fmt = win->type = _gl41wnd0_;
		windowcreate(win, arg, argc, argv);
		return win;
	}

	return 0;
}
void* supplymodify(int argc, u8** argv)
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
		supplycreate(name, argv[3], argc-3, &argv[3]);
	}

	return 0;
}
void* supplysearch(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct supply* win;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			win = &supply[j];
			if(0 == win->type)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&win->tier, &win->type, &win->fmt, &win->vfmt);
		}
		if(0 == j)say("empty supply\n");
	}
	else
	{
/*
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == supply[j].type)return 0;
		return &supply[j];
*/
		for(j=0;j<0x100;j++)
		{
			if(0 == supply[j].fmt)break;
			p = (void*)(&supply[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &supply[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freesupply()
{
	//say("[c,e):freeing supply\n");

	freewindow();
	freetray();
	freestd();
	freejoy();
}
void initsupply(u8* addr)
{
	int j;
	supply = (void*)(addr+0x000000);
	pinid = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct supply))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)supply[j].tier = _sup_;

	initstd(supply);
	initwindow(supply);
	inittray(supply);
	initjoy(supply);

	//say("[c,e):inited supply\n");
}
