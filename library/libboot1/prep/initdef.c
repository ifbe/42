#include "libuser.h"
#define _std_  hex32('s','t','d', 0)
#define _bdc_  hex32('b','d','c', 0)
#define _step_ hex32('s','t','e','p')
void* arenalist(void*, int);
void* actorlist(void*, int);
int arenaactor(struct arena* root, struct arena* twig, struct actor* leaf, struct actor* temp);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);


void* allocarena();
void* allocstyle();
void* allocpinid();
void* vbonode_create(u64, u64);
int vbonode_start(struct arena* twig, void* tf, struct arena* root, void* rf);


#define other_COUNT 5
static u64 other_want[other_COUNT] = {
	hex64('o','v','e','r','v','i','e','w'),
	hex32('v','j','o','y'),
	hex32('v','k','b','d'),
	hex64('c','o','r','n','e','r', 0, 0),
	hex64('p','o','i','n','t','e','r', 0)
};
static u8 other_flag[other_COUNT] = {
	0,
	'#',
	'#',
	0,
	0
};
void other_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<other_COUNT;j++)
	{
		act = actorcreate(other_want[j], 0);
		if(0 == act)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;
		sty->uc[3] = other_flag[j];

		sty->vr[0] = 1.0;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1.0;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1.0;

		relationcreate(act, pin, _act_, 0, win, sty, _win_, _ui_);
	}
}


void aider_create(struct arena* win, void* str)
{
#define _aid_ hex32('a','i','d',0)
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	act = actorcreate(hex64('a','i','d','f','o','n','t', 0), 0);
	sty = allocstyle();
	pin = allocpinid();

	sty->uc[3] = '#';
	relationcreate(act, pin, _act_, 0, win, sty, _win_, _aid_);


	act = actorcreate(hex64('a','i','d','g','e','o','m', 0), 0);
	sty = allocstyle();
	pin = allocpinid();

	sty->uc[3] = '#';
	relationcreate(act, pin, _act_, 0, win, sty, _win_, _aid_);
}


#define bg_COUNT 5
static u64 bg_want[bg_COUNT] = {
	hex64('t','e','x','b','a','l','l', 0),
	hex64('t','e','r','r','a','i','n', 0)
};
static char* bg_file[bg_COUNT] = {
	"datafile/jpg/skysphere.jpg",
	"datafile/jpg/cartoon.jpg"
};
void bg_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<bg_COUNT;j++)
	{
		act = actorcreate(bg_want[j], bg_file[j]);
		if(0 == act)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;

		sty->vr[0] = 10.0*1000.0*1000.0;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 10.0*1000.0*1000.0;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 10.0*1000.0*1000.0;

		relationcreate(act, pin, _act_, 0, win, sty, _win_, _3d_);
	}
}


void cam_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	act = actorcreate(hex64('f','r','e','e','c','a','m', 0), 0);
	if(0 == act)return;

	sty = allocstyle();
	if(0 == sty)return;

	pin = allocpinid();
	if(0 == pin)return;

	sty->vc[0] = 0;
	sty->vc[1] = 0;
	sty->vc[2] = 0;

	sty->vr[0] = 1.0;
	sty->vr[1] = 0;
	sty->vr[2] = 0;

	sty->vf[0] = 0;
	sty->vf[1] = 1.0;
	sty->vf[2] = 0;

	sty->vu[0] = 0;
	sty->vu[1] = 0;
	sty->vu[2] = 1.0;

	relationcreate(act, pin, _act_, _cam_, win, sty, _win_, _3d_);
}


void viewportvertex1(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

#define sin34 0.55919290
#define cos34 0.8290
	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = -cos34;
	win->camera.vb[2] = -sin34;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = cos34;
	win->camera.vu[2] = sin34;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = sin34;
	win->camera.vn[2] =-cos34;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] =-2000.0 * sin34;
	win->camera.vc[2] = 2000.0 * cos34;
}
void viewportvertex2(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = -1.0;
	win->camera.vb[2] = 0.0;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 1.0;
	win->camera.vu[2] = 0.0;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.0;
	win->camera.vn[2] = -1.0;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = 0.0;
	win->camera.vc[2] = 10000.0;
}




static int roletype = 0;
void role_delete()
{
}
void role_create()
{
	if(roletype)return;

	//+libhard0
	//devicecreate(_ahci_, 0);
	//devicecreate(_xhci_, 0);

	//+libhard1
	//drivercreate(_sata_, 0);
	//drivercreate(_usb_, 0);

	//+libsoft0
	//systemcreate(_uart_, "/dev/ptmx");
	//systemcreate(_uart_, "/dev/ttyACM0");

	//+libsoft1
	arterycreate(0,   "HACK://0.0.0.0:2222");
	arterycreate(0,   "QUIC://0.0.0.0:4444");
	arterycreate(0,    "SSH://0.0.0.0:2022");
	arterycreate(0, "TELNET://0.0.0.0:2023");
	arterycreate(0,   "HTTP://0.0.0.0:2080");
	arterycreate(0,    "TLS://0.0.0.0:2443");
	arterycreate(0,  "SERVE://0.0.0.0:2099");

	//+libuser0
	arenacreate(_std_, 0);
	arenacreate(_tray_, 0);
	struct arena* win = arenacreate(_win_, 0);


	struct arena* _3d = vbonode_create(_vbo_, 0);
	struct style* win_vp2 = allocstyle();
	struct pinid* ctx_pl2 = allocpinid();
	win_vp2->vc[0] = 0.0;
	win_vp2->vc[1] = 0.0;
	win_vp2->vq[0] = 1.0;
	win_vp2->vq[1] = 1.0;
	relationcreate(
		_3d, ctx_pl2, _win_, 0,
		win, win_vp2, _win_, _vp_);

	bg_create(_3d, 0);
	cam_create(_3d, 0);
	vbonode_start(_3d, 0, win, 0);


	struct arena* _ui = vbonode_create(_vbo_, 0);
	struct style* win_vp1 = allocstyle();
	struct pinid* ctx_pl1 = allocpinid();
	win_vp1->vc[0] = 0.0;
	win_vp1->vc[1] = 0.0;
	win_vp1->vq[0] = 1.0;
	win_vp1->vq[1] = 1.0;
	relationcreate(
		_ui, ctx_pl1, _win_, 0,
		win, win_vp1, _win_, _vp_);

	aider_create(_ui, 0);
	other_create(_ui, 0);
	vbonode_start(_ui, 0, win, 0);
}




void role_toycar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_bdc_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_stepcar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_step_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_control(int argc, u8** argv)
{
}
void role_uarthelp(int argc, u8** argv)
{
	void* sys;
	void* act;
	void* win;
	if(argc < 2)return;

	sys = systemcreate(_uart_, argv[1]);
	if(0 == sys)return;

	act = actorcreate(_std_, 0);
	if(act)relationcreate(act, 0, _act_, 0, sys, 0, _fd_, 0);

	win = arenalist("std", 0);
	if(win)relationcreate(act, 0, _act_, 0, win, 0, _win_, 0);

	win = arenacreate(_win_, 0);
	if(win)arenaactor(win, 0, act, 0);
}




void role_test(int, u8**);
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(argc < 2)return;

	if(0 == ncmp(argv[1], "test", 4))
	{
		role_test(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "toycar", 6))
	{
		roletype = 1;
		role_toycar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "stepcar", 6))
	{
		roletype = 1;
		role_stepcar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "control", 7))
	{
		roletype = 2;
		role_control(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "uart", 4))
	{
		roletype = 3;
		role_uarthelp(argc-1, &argv[1]);
	}
}
