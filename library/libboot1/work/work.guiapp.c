#include "libuser.h"
#define _term_ hex32('t','e','r','m')
void* style_alloc();
void* style_recycle();




static void guiapp_wndctx(struct supply* wnd)
{
	//wnd-> = ;
	struct style* toterm = style_alloc();
	toterm->fshape.vc[0] = 0.0;
	toterm->fshape.vc[1] = 0.0;
	toterm->fshape.vq[0] = 1.0;
	toterm->fshape.vq[1] = 1.0;


	//things
	struct entity* termnode = entitycreate(_term_,0, 0, 0);
	struct style* termfoot = style_alloc();


	//relation
	struct relation* rel;
	rel = relationcreate(termnode,termfoot, _ent_,0, wnd,toterm, _ent_,0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);
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