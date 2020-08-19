#include "libboot.h"
#define _name_ hex32('2','0','4','8')
void* allocstyle();




int kernel_create(struct worker* wrk, void* url, int argc, u8** argv)
{
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	struct entity* ent = entitycreate(_name_,0, 0, 0);
	struct style* aaa = allocstyle();
	struct style* bbb = allocstyle();

	aaa->fshape.vc[0] = 256;
	aaa->fshape.vc[1] = 256;
	aaa->fshape.vr[0] = 256;
	aaa->fshape.vr[1] = 0;
	aaa->fshape.vf[0] = 0;
	aaa->fshape.vf[1] = 256;

	struct relation* rel = relationcreate(ent, bbb, _ent_, 0, wnd, aaa, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	struct event* ev;
	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;

	//forever
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