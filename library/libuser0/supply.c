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
int stdio_read(void*,int, void*,int, void*,int, void*,int);
int stdio_write(void*,int, void*,int, void*,int, void*,int);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*, int, u8**);
int traydelete(void*);
//
int ahrs_create(void*, void*, int, u8**);
int ahrs_delete(void*);
int ahrs_read(void*,int, void*,int, void*,int, void*,int);
int ahrs_write(void*,int, void*,int, void*,int, void*,int);
//
int slam_create(void*, void*, int, u8**);
int slam_delete(void*);
int slam_read(void*,int, void*,int, void*,int, void*,int);
int slam_write(void*,int, void*,int, void*,int, void*,int);
//sound.usbmic
void initmicphone(void*);
void freemicphone();
int micphonecreate(void*, void*, int, u8**);
int micphonedelete(void*);
int micphonestart(void*);
int micphonestop(void*);
int micphoneread(void*,int, void*,int, void*,int, void*,int);
int micphonewrite(void*,int, void*,int, void*,int, void*,int);
int micphonelist();
int micphonechoose();
//sound.speaker
void initspeaker(void*);
void freespeaker();
int speakercreate(void*, void*, int, u8**);
int speakerdelete(void*);
int speakerstart(void*);
int speakerstop(void*);
int speakerread(void*,int, void*,int, void*,int, void*,int);
int speakerwrite(void*,int, void*,int, void*,int, void*,int);
int speakerlist();
int speakerchoose();
//light.usbcam
void initcam(void*);
void freecam();
int videocreate(void*, void*, int, u8**);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void*,int, void*,int, void*,int, void*,int);
int videowrite(void*,int, void*,int, void*,int, void*,int);
int videolist();
int videochoose();
//light.window
void initwindow(void*);
void freewindow();
int windowcreate(void*, void*, int, u8**);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void*,int, void*,int, void*,int, void*,int);
int windowwrite(void*,int, void*,int, void*,int, void*,int);
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




int supplyread(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_read(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speakerread(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:return videoread(sup,foot, stack,sp, arg, idx, buf, len);
		case _fbo_:
		case _wnd_:return windowread(sup,foot, stack,sp, arg, idx, buf, len);
	}
	return 0;
}
int supplywrite(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_write(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speakerwrite(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:break;
		case _fbo_:
		case _wnd_:return windowwrite(sup,foot, stack,sp, arg, idx, buf, len);
	}
	switch(sup->fmt){
		case _ahrs_:return ahrs_write(sup,foot, stack,sp, arg, idx, buf, len);
		case _slam_:return slam_write(sup,foot, stack,sp, arg, idx, buf, len);
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
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _ahrs_;
		ahrs_create(win, arg, argc, argv);
		return win;
	}
	case _slam_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _sensor_;
		win->fmt = _slam_;
		slam_create(win, arg, argc, argv);
		return win;
	}

//---------------------gadget-----------------
	case _joy_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _joy_;
		win->fmt = _joy_;
		joycreate(win, arg, argc, argv);
		return win;
	}
	case _std_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _std_;
		win->fmt = _std_;
		stdio_create(win, arg, argc, argv);
		return win;
	}
	case _tray_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _tray_;
		win->fmt = _tray_;
		traycreate(win, arg, argc, argv);
		return win;
	}

//--------------------micphone------------------
	case _mic_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		micphonecreate(win, arg, argc, argv);
		return win;
	}

//--------------------speaker--------------------
	case _spk_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _spk_;
		win->fmt = hex32('p','c','m',0);
		speakercreate(win, arg, argc, argv);
		return win;
	}

//---------------------video-------------------
	case _cam_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videocreate(win, arg, argc, argv);
		return win;
	}
	case _cap_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _cam_;
		win->fmt = hex32('h','o','l','o');
		//hologram_create(win, arg, argc, argv);
		return win;
	}

//---------------------window----------------
	case _wnd_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41none_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41none_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41easy_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41easy_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41full_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41full_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41coop_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _wnd_;
		win->fmt = _gl41coop_;
		windowcreate(win, arg, argc, argv);
		return win;
	}

//------------------framebuffer-------------------
	case _gl41fbo6_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _fbo_;
		win->fmt = _gl41fbo6_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41fboc_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _fbo_;
		win->fmt = _gl41fboc_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41fbod_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _fbo_;
		win->fmt = _gl41fbod_;
		windowcreate(win, arg, argc, argv);
		return win;
	}
	case _gl41fbog_:
	{
		win = allocsupply();
		if(0 == win)return 0;

		win->type = _fbo_;
		win->fmt = _gl41fbog_;
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




void freesupply()
{
	say("[c,e):supply freeing\n");

	freewindow();
	freetray();
	freestd();
	freejoy();

	say("[c,e):supply freeed\n");
}
void initsupply(u8* addr)
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
