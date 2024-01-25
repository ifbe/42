#include "libuser.h"
//
void initjoy(void*);
void freejoy();
int joy_create(void*, void*, int, u8**);
int joy_delete(void*);
//
void initstd(void*);
void freestd();
int stdio_create(void*, void*, int, u8**);
int stdio_delete(void*);
int stdio_take(void*,void*, void*,int, p64,int, void*,int);
int stdio_give(void*,void*, void*,int, p64,int, void*,int);
//
void inittray(void*);
void freetray();
int tray_create(void*, void*, int, u8**);
int tray_delete(void*);
//
int ahrs_create(void*, void*, int, u8**);
int ahrs_delete(void*);
int ahrs_take(void*,void*, void*,int, p64,int, void*,int);
int ahrs_give(void*,void*, void*,int, p64,int, void*,int);
//
int slam_create(void*, void*, int, u8**);
int slam_delete(void*);
int slam_take(void*,void*, void*,int, p64,int, void*,int);
int slam_give(void*,void*, void*,int, p64,int, void*,int);
//sound.usbmic
void initmicphone(void*);
void freemicphone();
int micphone_create(void*, void*, int, u8**);
int micphone_delete(void*);
int micphone_take(void*,void*, void*,int, p64,int, void*,int);
int micphone_give(void*,void*, void*,int, p64,int, void*,int);
//sound.speaker
void initspeaker(void*);
void freespeaker();
int speaker_create(void*, void*, int, u8**);
int speaker_delete(void*);
int speaker_take(void*,void*, void*,int, p64,int, void*,int);
int speaker_give(void*,void*, void*,int, p64,int, void*,int);
//light.usbcam
void initcam(void*);
void freecam();
int camera_create(void*, void*, int, u8**);
int camera_delete(void*);
int camera_take(void*,void*, void*,int, p64,int, void*,int);
int camera_give(void*,void*, void*,int, p64,int, void*,int);
//
int screencap_create(void*, void*, int, u8**);
int screencap_delete(void*);
int screencap_take(void*,void*, void*,int, p64,int, void*,int);
int screencap_give(void*,void*, void*,int, p64,int, void*,int);
//
int codecv_create(void*, void*, int, u8**);
int codecv_delete(void*);
int codecv_reader(void*,void*, p64,int, void*,int);
int codecv_writer(void*,void*, p64,int, void*,int);
int codecv_take(void*,void*, void*,int, p64,int, void*,int);
int codecv_give(void*,void*, void*,int, p64,int, void*,int);
//light.window
void initwindow(void*);
void freewindow();
void* window_alloc();
void* window_recycle();
int window_create(void*, void*, int, u8**);
int window_delete(void*);
int window_read(void*);
int window_write(void*);
int window_attach(void*);
int window_detach(void*);
int window_take(void*,void*, void*,int, p64,int, void*,int);
int window_give(void*,void*, void*,int, p64,int, void*,int);




//
static _obj* supply = 0;
static int suplen = 0;
static struct style* pinid = 0;
static int pinlen = 0;




#define maxitem (0x100000/sizeof(_obj))
void supply_init(u8* addr)
{
	logtoall("[c,e):supply initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	supply = (void*)(addr+0x000000);
	suplen = maxitem-1;
	for(j=0;j<maxitem;j++)supply[j].tier = _sup_;

//#define maxfoot 
	pinid = (void*)(addr+0x100000);

	initstd(supply);
	initwindow(supply);
	inittray(supply);
	initjoy(supply);

	logtoall("[c,e):supply inited\n");
}
void supply_exit()
{
	logtoall("[c,e):supply exiting\n");

	freewindow();
	freetray();
	freestd();
	freejoy();

	logtoall("[c,e):supply exited\n");
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




void supply_recycle()
{
}
void* supply_alloc()
{
	int j;
	_obj* obj;
	for(j=0;j<0x100;j++)
	{
		if(0 == supply[j].type)break;
	}
	if(j >= 0x100)return 0;

	obj = &supply[j];
	obj->irel0 = obj->ireln = 0;
	obj->orel0 = obj->oreln = 0;
	return obj;
}




void* supply_prep(_obj* obj, int len, u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	if(len < sizeof(_obj))return 0;

	switch(type){
//---------------------gadget-----------------
	case _joy_:
		obj->type = _joy_;
		obj->hfmt = _joy_;
		return obj;
	case _std_:
		obj->type = _std_;
		obj->hfmt = _std_;
		return obj;
	case _tray_:
		obj->type = _tray_;
		obj->hfmt = _tray_;
		return obj;

//--------------------micphone------------------
	case _mic_:
		obj->type = _mic_;
		obj->hfmt = hex32('p','c','m',0);
		return obj;

//--------------------speaker--------------------
	case _spk_:
		obj->type = _spk_;
		obj->hfmt = hex32('p','c','m',0);
		return obj;

//---------------------camera-------------------
	case _cam_:
		obj->type = _cam_;
		obj->hfmt = hex32('y','u','v',0);
		return obj;
	case _wndcap_:
		obj->type = _wndcap_;
		return obj;

//---------------------window----------------
	case _wnd_:
		obj->type = _wnd_;
		return obj;

	case _dec_:
		obj->type = _dec_;
		obj->hfmt = _h264_;
		return obj;
	case _enc_:
		obj->type = _enc_;
		obj->hfmt = _h264_;
		return obj;

//---------------------3dinput-------------------
	case _cap_:
		obj->type = _cap_;
		obj->hfmt = hex32('h','o','l','o');
		return obj;

//---------------------3doutput-------------------
	case _gpu_:
		obj->type = _gpu_;
		obj->hfmt = _gpu_;
		return obj;
	case _gl41none_:
		obj->type = _wnd_;
		obj->hfmt = _gl41none_;
		return obj;
	case _gl41easy_:
		obj->type = _wnd_;
		obj->hfmt = _gl41easy_;
		return obj;
	case _gl41list_:
		obj->type = _wnd_;
		obj->hfmt = _gl41list_;
		return obj;
	case _gl41cmdq_:
		obj->type = _wnd_;
		obj->hfmt = _gl41cmdq_;
		return obj;
	}//switch

	return obj;
}




int supply_create(_obj* obj, void* arg, int argc, u8** argv)
{
	logtoall("supply_create:obj=%p,arg=%p,argc=%d,argv=%p\n", obj, arg, argc, argv);
	switch(obj->type){
	case _joy_:
		joy_create(obj, arg, argc, argv);
		break;
	case _std_:
		stdio_create(obj, arg, argc, argv);
		break;
	case _tray_:
		tray_create(obj, arg, argc, argv);
		break;
	case _mic_:
		micphone_create(obj, arg, argc, argv);
		break;
	case _spk_:
		speaker_create(obj, arg, argc, argv);
		break;
	case _cam_:
		camera_create(obj, arg, argc, argv);
		break;
	case _wndcap_:
		screencap_create(obj, arg, argc, argv);
		break;
	case _wnd_:
		window_create(obj, arg, argc, argv);
		break;
	case _dec_:
	case _enc_:
		codecv_create(obj, arg, argc, argv);
		break;
	case _cap_:
		//hologram_create(obj, arg, argc, argv);
		break;
	case _gpu_:
		//gpulib_create(obj, arg, argc, argv);
		break;
	case _gl41none_:
		window_create(obj, arg, argc, argv);
		break;
	case _gl41easy_:
		window_create(obj, arg, argc, argv);
		break;
	case _gl41list_:
		window_create(obj, arg, argc, argv);
		break;
	case _gl41cmdq_:
		window_create(obj, arg, argc, argv);
		break;
	}//switch
	return 0;
}
int supply_delete(_obj* obj)
{
	logtoall("supply_delete\n");
	if(0 == obj)return 0;

	//1.unlink
	obj->irel0 = 0;
	obj->ireln = 0;
	obj->orel0 = 0;
	obj->oreln = 0;

	//2.close
	switch(obj->type){
	case _wnd_:
		window_delete(obj);
		break;
	case _cam_:
		camera_delete(obj);
		break;
	case _spk_:
		speaker_delete(obj);
		break;
	case _mic_:
		micphone_delete(obj);
		break;
	}

	//3.cleanup
	obj->type = 0;
	obj->hfmt = 0;
	return 0;
}
int supply_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
	switch(sup->type){
	case _dec_:
	case _enc_:
		return codecv_reader(sup,foot, arg, idx, buf, len);
	}
	return 0;
}
int supply_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
	return 0;
}




int supply_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@supplyattach\n");

	if(0 == self)return 0;

	_obj* obj = (void*)(self->chip);
	if(0 == obj)return 0;

	return 0;
}
int supply_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@supplydetach\n");
	return 0;
}
int supply_takeby(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_take(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speaker_take(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:return camera_take(sup,foot, stack,sp, arg, idx, buf, len);
		case _wnd_:return window_take(sup,foot, stack,sp, arg, idx, buf, len);
		case _wndcap_:return screencap_take(sup,foot, stack,sp, arg, idx, buf, len);

		case _dec_:
		case _enc_:
			return codecv_take(sup,foot, stack,sp, arg, idx, buf, len);
	}
	return 0;
}
int supply_giveby(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	switch(sup->type){
		case _std_:return stdio_give(sup,foot, stack,sp, arg,idx, buf,len);

		case _mic_:break;
		case _spk_:return speaker_give(sup,foot, stack,sp, arg, idx, buf, len);

		case _cam_:return camera_give(sup,foot, stack,sp, arg, idx, buf, len);
		case _wnd_:return window_give(sup,foot, stack,sp, arg, idx, buf, len);
		case _wndcap_:return screencap_take(sup,foot, stack,sp, arg, idx, buf, len);

		case _dec_:
		case _enc_:
			return codecv_give(sup,foot, stack,sp, arg, idx, buf, len);
	}
	switch(sup->hfmt){
		case _ahrs_:return ahrs_give(sup,foot, stack,sp, arg, idx, buf, len);
		case _slam_:return slam_give(sup,foot, stack,sp, arg, idx, buf, len);
	}

	return 0;
}




//helper function
void* supply_alloc_prep(u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	//logtoall("%s:%.8s\n", __func__, &type);
	_obj* obj = supply_alloc();
	if(0 == obj)return 0;

	//if(supply_findfmt())supply_prepfromclone(obj, );
	return supply_prep(obj, sizeof(_obj), tier, type, hfmt, vfmt);
}
int supply_unprep_dealloc(_obj* obj)
{
	return 0;
}
void* supply_alloc_prep_create(u64 tier, u64 type, u64 hfmt, u64 vfmt, int argc, u8** argv)
{
	_obj* obj = supply_alloc_prep(tier, type, hfmt, vfmt);
	if(0 == obj)return 0;

	int ret = supply_create(obj, 0, argc, argv);

	return obj;
}
int supply_delete_unprep_dealloc(_obj* obj)
{
	return 0;
}
void* supply_alloc_prep_create_attach()
{
	return 0;
}
int supply_detach_delete_unprep_dealloc()
{
	return 0;
}




//cmdline function
int supplycommand_insert(u8* name, u8* arg)
{
	return 0;
}
int supplycommand_remove(u8* name)
{
	return 0;
}
int supplycommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxitem;j++){
			act = &supply[j];
			if((0 == act->type)&&(0 == act->hfmt))continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->hfmt, &act->hfmt);
		}
		if(0 == j)logtoall("empty supply\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == supply[j].hfmt)break;
			if(0 == cmp(&supply[j].hfmt, name))logtoall("name=%d,node=%p\n", name, &supply[j]);
			break;
		}
	}
	return 0;
}
int supplycommand_modify(int argc, u8** argv)
{
	return 0;
}
void* supplycommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("supply insert name arg\n");
		logtoall("supply search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//supply create name arg
		supplycommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//supply search <name>
		supplycommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
