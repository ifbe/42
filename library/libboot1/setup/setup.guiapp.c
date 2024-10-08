#include "libuser.h"
#define _2048_ hex32('2','0','4','8')
#define _tree_ hex32('t','r','e','e')
#define _term_ hex32('t','e','r','m')
#define _texball_ hex64('t','e','x','b','a','l','l', 0)
#define _terrain_ hex64('t','e','r','r','a','i','n', 0)
//
#define _corner_ hex64('c','o','r','n','e','r', 0, 0)
//
#define _freecam_ hex64('f','r','e','e','c','a','m', 0)
#define _orthcam_ hex64('o','r','t','h','c','a','m', 0)
#define _vrbox_   hex64('v','r','b','o','x', 0 , 0 , 0)
#define _vrglass_ hex64('v','r','g','l','a','s','s', 0)
//
#define _virtual_ hex64('v','i','r','t','u','a','l', 0)
//
#define _in_ hex16('i','n')
void* style_alloc();
void* style_recycle();
//
void* poller_alloc();
void poller(void*);
//
void decide_loop_or_exit();




static void guiapp_mgr_to_ent(_obj* mgr)
{
	_obj* term = entity_alloc_fromtype(_term_);
	entity_create(term, 0, 0, 0);

	struct style* term_to_mgr = style_alloc();
	struct style* mgr_to_term = style_alloc();
	mgr_to_term->fshape.vc[0] = 0.0;
	mgr_to_term->fshape.vc[1] = 0.0;
	mgr_to_term->fshape.vq[0] = 1.0;
	mgr_to_term->fshape.vq[1] = 1.0;
	struct relation* rel1 = relationcreate(term,term_to_mgr, _ent_,0, mgr,mgr_to_term, _ent_,0);
	relationattach((void*)&rel1->srcchip, (void*)&rel1->dstchip);
/*
	_obj* game = entity_alloc_prepobj_create(_ent_, _2048_,0, 0, 0, 0);
	struct style* game_to_mgr = style_alloc();
	struct style* mgr_to_game = style_alloc();
	mgr_to_game->fshape.vc[0] = 0.0;
	mgr_to_game->fshape.vc[1] = 0.0;
	mgr_to_game->fshape.vq[0] = 1.0;
	mgr_to_game->fshape.vq[1] = 1.0;
	struct relation* rel2 = relationcreate(game,game_to_mgr, _ent_,0, mgr,mgr_to_game, _ent_,0);
	relationattach((void*)&rel2->srcchip, (void*)&rel2->dstchip);
*/
}
static void guiapp_wnd_to_mgr(_obj* wnd)
{
	_obj* mgr = entity_alloc_fromtype(_corner_);
	entity_create(mgr, 0, 0, 0);
	struct style* mgr_to_wnd = style_alloc();
	struct style* wnd_to_mgr = style_alloc();
	wnd_to_mgr->fshape.vc[0] = 0.0;
	wnd_to_mgr->fshape.vc[1] = 0.0;
	wnd_to_mgr->fshape.vq[0] = 1.0;
	wnd_to_mgr->fshape.vq[1] = 1.0;
	struct relation* rel0 = relationcreate(mgr,mgr_to_wnd, _ent_,0, wnd,wnd_to_mgr, _sup_,0);
	relationattach((void*)&rel0->srcchip, (void*)&rel0->dstchip);

	guiapp_mgr_to_ent(mgr);
}
static void guiapp_wnd_to_3d(_obj* wnd)
{
	//_obj* abc = entity_alloc_prepobj_create(_ent_, _texball_,"datafile/jpg/texball-grassland.jpg", 0, 0);
	_obj* abc = entity_alloc_fromtype(_texball_);
	entity_create(abc, "datafile/jpg/360d-park.jpg", 0, 0);
	_obj* wrd = entity_alloc_fromtype(_virtual_);
	entity_create(wrd, 0, 0 ,0);
	_obj* cam = entity_alloc_fromtype(_vrbox_);
	entity_create(cam, 0, 0 ,0);

	struct style* wrd_to_cam = style_alloc();
	struct style* cam_to_wrd = style_alloc();
	wrd_to_cam->fshape.vc[0] = 0.0;
	wrd_to_cam->fshape.vc[1] = 0.0;
	wrd_to_cam->fshape.vc[2] = 1.0;		//1m
	wrd_to_cam->fshape.vq[0] = 0.0;
	wrd_to_cam->fshape.vq[1] =-0.2;		//20cm
	wrd_to_cam->fshape.vq[2] = 0.0;		//1m
	wrd_to_cam->fshape.vr[0] = 0.04;
	wrd_to_cam->fshape.vr[1] = 0.0;
	wrd_to_cam->fshape.vr[2] = 0.0;
	wrd_to_cam->fshape.vr[3] = 0.04;	//8cm/2
	wrd_to_cam->fshape.vf[0] = 0.0;
	wrd_to_cam->fshape.vf[1] = 0.005;
	wrd_to_cam->fshape.vf[2] = 0.0;
	wrd_to_cam->fshape.vf[3] = 0.005;	//1cm/2
	wrd_to_cam->fshape.vt[0] = 0.0;
	wrd_to_cam->fshape.vt[1] = 0.0;
	wrd_to_cam->fshape.vt[2] = 0.08;
	wrd_to_cam->fshape.vt[3] = 0.08;	//16cm/2
	wrd_to_cam->frus.vl[3] =-0.001;
	wrd_to_cam->frus.vr[3] = 0.001;
	wrd_to_cam->frus.vb[3] =-0.001;
	wrd_to_cam->frus.vt[3] = 0.001;
	wrd_to_cam->frus.vn[3] = 0.001;
	wrd_to_cam->frus.vf[3] = 1000.0;
	struct relation* rel2 = relationcreate(cam,cam_to_wrd, _ent_,_in_, wrd,wrd_to_cam, _ent_,0);
	relationattach((void*)&rel2->srcchip, (void*)&rel2->dstchip);

	struct style* wrd_to_abc = style_alloc();
	struct style* abc_to_wrd = style_alloc();
	wrd_to_abc->fshape.vc[0] = 0.0;
	wrd_to_abc->fshape.vc[1] = 0.0;
	wrd_to_abc->fshape.vc[2] = 0.0;
	wrd_to_abc->fshape.vr[0] = 1000.0;		//10km
	wrd_to_abc->fshape.vr[1] = 0.0;
	wrd_to_abc->fshape.vr[2] = 0.0;
	wrd_to_abc->fshape.vf[0] = 0.0;
	wrd_to_abc->fshape.vf[1] = 1000.0;		//10km
	wrd_to_abc->fshape.vf[2] = 0.0;
	wrd_to_abc->fshape.vt[0] = 0.0;
	wrd_to_abc->fshape.vt[1] = 0.0;
	wrd_to_abc->fshape.vt[2] = 1000.0;		//10km
	struct relation* rel3 = relationcreate(abc,abc_to_wrd, _ent_,0, wrd,wrd_to_abc, _ent_,0);
	relationattach((void*)&rel3->srcchip, (void*)&rel3->dstchip);

	struct style* cam_to_wnd = style_alloc();
	struct style* wnd_to_cam = style_alloc();
	wnd_to_cam->fshape.vc[0] = 0.0;
	wnd_to_cam->fshape.vc[1] = 0.0;
	wnd_to_cam->fshape.vq[0] = 1.0;
	wnd_to_cam->fshape.vq[1] = 1.0;
	struct relation* rel1 = relationcreate(cam,cam_to_wnd, _ent_,0, wnd,wnd_to_cam, _sup_,0);
	relationattach((void*)&rel1->srcchip, (void*)&rel1->dstchip);

}




void guiapp_delete(struct item* wrk, u8* arg)
{
}
int guiapp_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	_obj* obj = bootup_alloc_fromtype(_myml_);
	bootup_create(obj, "datafile/myml/index.myml", 0, 0);

	decide_loop_or_exit();
/*
	_obj* wnd = supply_create(_wnd_, 0, 0, 0);
	wrk->priv_ptr = wnd;

	_obj* mic = supply_create(_mic_, 0, 0, 0);

	guiapp_wnd_to_3d(wnd);
	guiapp_wnd_to_mgr(wnd);

	void* mpoller = poller_alloc();
	if(mpoller)poller(mpoller);

	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;

	struct event* ev;
	while(1){
		supply_takeby(wnd,0, stack,2, 0,0, 0,0);

		//cleanup events
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return 0;

			supply_giveby(wnd,0, stack,2, 0,0, ev,0);
		}
	}*/
	return 0;
}




void guiapp_init()
{
}
void guiapp_exit(void* addr)
{
}
