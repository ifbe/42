#include "libuser.h"
#define _corner_ hex64('c','o','r','n','e','r', 0, 0)
#define _term_ hex32('t','e','r','m')
#define _2048_ hex32('2','0','4','8')
void* style_alloc();
void* style_recycle();




static void guiapp_wndctx(struct supply* wnd)
{
	struct entity* mgr = entitycreate(_corner_,0, 0, 0);
	struct style* mgr_to_wnd = style_alloc();
	struct style* wnd_to_mgr = style_alloc();
	wnd_to_mgr->fshape.vc[0] = 0.0;
	wnd_to_mgr->fshape.vc[1] = 0.0;
	wnd_to_mgr->fshape.vq[0] = 1.0;
	wnd_to_mgr->fshape.vq[1] = 1.0;
	struct relation* rel0 = relationcreate(mgr,mgr_to_wnd, _ent_,0, wnd,wnd_to_mgr, _ent_,0);
	relationlinkup((void*)&rel0->srcchip, (void*)&rel0->dstchip);


	struct entity* term = entitycreate(_term_,0, 0, 0);
	struct style* term_to_mgr = style_alloc();
	struct style* mgr_to_term = style_alloc();
	mgr_to_term->fshape.vc[0] = 0.0;
	mgr_to_term->fshape.vc[1] = 0.0;
	mgr_to_term->fshape.vq[0] = 1.0;
	mgr_to_term->fshape.vq[1] = 1.0;
	struct relation* rel1 = relationcreate(term,term_to_mgr, _ent_,0, mgr,mgr_to_term, _ent_,0);
	relationlinkup((void*)&rel1->srcchip, (void*)&rel1->dstchip);


	struct entity* game = entitycreate(_2048_,0, 0, 0);
	struct style* game_to_mgr = style_alloc();
	struct style* mgr_to_game = style_alloc();
	mgr_to_game->fshape.vc[0] = 0.0;
	mgr_to_game->fshape.vc[1] = 0.0;
	mgr_to_game->fshape.vq[0] = 1.0;
	mgr_to_game->fshape.vq[1] = 1.0;
	struct relation* rel2 = relationcreate(game,game_to_mgr, _ent_,0, mgr,mgr_to_game, _ent_,0);
	relationlinkup((void*)&rel2->srcchip, (void*)&rel2->dstchip);
}




void guiapp_delete(struct item* wrk, u8* arg)
{
}
int guiapp_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	wrk->priv_ptr = wnd;

	guiapp_wndctx(wnd);

	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;

	struct event* ev;
	while(1){
		supply_take(wnd,0, stack,2, 0,0, 0,0);

		//cleanup events
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return 0;

			supply_give(wnd,0, stack,2, 0,0, ev,0);
		}
	}
	return 0;
}




void guiapp_init()
{
}
void guiapp_exit(void* addr)
{
}