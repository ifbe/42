#include "libuser.h"
//
void initjoy(void*);
void freejoy();
int joycreate(void*, void*, int, u8**);
int joydelete(void*);
//
void initstd(void*);
void freestd();
int stdio_create(void*, void*, int, u8**);
int stdio_delete(void*);
int stdio_take(void*,void*, void*,int, void*,int, void*,int);
int stdio_give(void*,void*, void*,int, void*,int, void*,int);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*, int, u8**);
int traydelete(void*);
//
int ahrs_create(void*, void*, int, u8**);
int ahrs_delete(void*);
int ahrs_take(void*,void*, void*,int, void*,int, void*,int);
int ahrs_give(void*,void*, void*,int, void*,int, void*,int);
//
int slam_create(void*, void*, int, u8**);
int slam_delete(void*);
int slam_take(void*,void*, void*,int, void*,int, void*,int);
int slam_give(void*,void*, void*,int, void*,int, void*,int);
//sound.usbmic
void initmicphone(void*);
void freemicphone();
int micphonecreate(void*, void*, int, u8**);
int micphonedelete(void*);
int micphone_take(void*,void*, void*,int, void*,int, void*,int);
int micphone_give(void*,void*, void*,int, void*,int, void*,int);
//sound.speaker
void initspeaker(void*);
void freespeaker();
int speakercreate(void*, void*, int, u8**);
int speakerdelete(void*);
int speaker_take(void*,void*, void*,int, void*,int, void*,int);
int speaker_give(void*,void*, void*,int, void*,int, void*,int);
//light.usbcam
void initcam(void*);
void freecam();
int videocreate(void*, void*, int, u8**);
int videodelete(void*);
int video_take(void*,void*, void*,int, void*,int, void*,int);
int video_give(void*,void*, void*,int, void*,int, void*,int);
//light.window
void initwindow(void*);
void freewindow();
int windowcreate(void*, void*, int, u8**);
int windowdelete(void*);
int window_take(void*,void*, void*,int, void*,int, void*,int);
int window_give(void*,void*, void*,int, void*,int, void*,int);




//
static struct supply* supply = 0;
static int winlen = 0;
static struct style* pinid = 0;
static int pinlen = 0;
void* supply_alloc()
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
void supply_recycle()
{
}


void* pinid_alloc()
{
#define maxlen 0x200
	int j;
	u8* buf;

	buf = (void*)pinid + pinlen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	pinlen += maxlen;
	return buf;
}
void pinid_recycle()
{
}




int supply_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_take(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speaker_take(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:return video_take(sup,foot, stack,sp, arg, idx, buf, len);
		case _fbo_:
		case _wnd_:return window_take(sup,foot, stack,sp, arg, idx, buf, len);
	}
	return 0;
}
int supply_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_give(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speaker_give(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:break;
		case _fbo_:
		case _wnd_:return window_give(sup,foot, stack,sp, arg, idx, buf, len);
	}
	switch(sup->fmt){
		case _ahrs_:return ahrs_give(sup,foot, stack,sp, arg, idx, buf, len);
		case _slam_:return slam_give(sup,foot, stack,sp, arg, idx, buf, len);
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
	if(0 == win)return 0;

	//1.close
	windowdelete(win);

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

	switch(type){
//-------------------tobe delete--------------
	case _ahrs_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _ahrs_;
		ahrs_create(win, arg, argc, argv);
		return win;
	}
	case _slam_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _slam_;
		slam_create(win, arg, argc, argv);
		return win;
	}

//---------------------gadget-----------------
	case _joy_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _joy_;
		win->fmt = _joy_;
		joycreate(win, arg, argc, argv);
		return win;
	}
	case _std_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _std_;
		win->fmt = _std_;
		stdio_create(win, arg, argc, argv);
		return win;
	}
	case _tray_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _tray_;
		win->fmt = _tray_;
		traycreate(win, arg, argc, argv);
		return win;
	}

//--------------------micphone------------------
	case _mic_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		micphonecreate(win, arg, argc, argv);
		return win;
	}

//--------------------speaker--------------------
	case _spk_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _spk_;
		win->fmt = hex32('p','c','m',0);
		speakercreate(win, arg, argc, argv);
		return win;
	}

//---------------------video-------------------
	case _cam_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videocreate(win, arg, argc, argv);
		return win;
	}
	case _cap_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('h','o','l','o');
		//hologram_create(win, arg, argc, argv);
		return win;
	}

//---------------------window----------------
	case _wnd_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _wnd_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41none_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41none_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41easy_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41easy_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41full_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41full_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41coop_:
	{
		win = supply_alloc();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41coop_;
		windowcreate(win, arg, argc, argv);
		return win;
	}

	}//switch
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
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j, &win->tier, &win->type, &win->fmt, &win->vfmt);
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




void supply_exit()
{
	say("[c,e):supply exiting\n");

	freewindow();
	freetray();
	freestd();
	freejoy();

	say("[c,e):supply exited\n");
}
void supply_init(u8* addr)
{
	say("[c,e):supply initing\n");

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

	say("[c,e):supply inited\n");
}
