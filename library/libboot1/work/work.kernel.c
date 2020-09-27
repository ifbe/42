#include "libboot.h"
#define _2048_ hex32('2','0','4','8')
#define _term_ hex32('t','e','r','m')
#define _mmioedit_ hex64('m','m','i','o','e','d','i','t')
void* allocstyle();




int kernel_create(struct worker* wrk, void* url, int argc, u8** argv)
{
	//screen
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	//wnd-> = ;
	//wnd-> = ;
	struct style* aaa = allocstyle();
	aaa->fshape.vc[0] = wnd->width/2;
	aaa->fshape.vc[1] = wnd->height/2;
	aaa->fshape.vr[0] = wnd->width/2;
	aaa->fshape.vr[1] = 0;
	aaa->fshape.vf[0] = 0;
	aaa->fshape.vf[1] = wnd->height/2;
	struct style* bbb = allocstyle();
	bbb->fshape.vc[0] = wnd->width*3/4;
	bbb->fshape.vc[1] = wnd->height/4;
	bbb->fshape.vr[0] = wnd->width/5;
	bbb->fshape.vr[1] = 0;
	bbb->fshape.vf[0] = 0;
	bbb->fshape.vf[1] = wnd->height/5;
	struct style* ccc = allocstyle();
	ccc->fshape.vc[0] = wnd->width*3/4;
	ccc->fshape.vc[1] = wnd->height*3/4;
	ccc->fshape.vr[0] = wnd->width/4;
	ccc->fshape.vr[1] = 0;
	ccc->fshape.vf[0] = 0;
	ccc->fshape.vf[1] = wnd->height/4;


	//things
	struct entity* termnode = entitycreate(_term_,0, 0, 0);
	struct style* termfoot = allocstyle();

	struct entity* gamenode = entitycreate(_2048_,0, 0, 0);
	struct style* gamefoot = allocstyle();

	struct entity* editnode = entitycreate(_mmioedit_,0, 0, 0);
	struct style* editfoot = allocstyle();


	//relation
	struct relation* rel;
	rel = relationcreate(termnode, termfoot, _ent_, 0, wnd, aaa, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);
/*
	rel = relationcreate(gamenode, gamefoot, _ent_, 0, wnd, bbb, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(editnode, editfoot, _ent_, 0, wnd, ccc, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);
*/

	//loop
	struct event* ev;
	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	while(1)
	{
		//draw frame
		supplyread(wnd,0, stack,2, 0,0, 0,0);

		//cleanup events
		while(1)
		{
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return 0;

			supplywrite(wnd,0, stack,2, 0,0, ev,0);
		}
	}
	return 0;
}
