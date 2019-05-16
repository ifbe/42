#include "libuser.h"
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




void role_default()
{
	//+libuser0
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