#include "libboot.h"
#define _2048_ hex32('2','0','4','8')
#define _term_ hex32('t','e','r','m')
void initidt();
void init8259();	//interrupt controller
void init825x();	//timer.pit
void initrtc(); 	//timer.rtc
void* allocstyle();




int kernel_create(struct worker* wrk, void* url, int argc, u8** argv)
{
	asm("cli");
	initidt();
	init8259();
	init825x();
	initrtc();
	asm("sti");

	asm("int3");
	asm("int $0x80");


	//screen
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	struct entity* dbg = entitycreate(_term_,0, 0, 0);
	struct entity* ent = entitycreate(_2048_,0, 0, 0);

	struct style* aaa = allocstyle();
	struct style* bbb = allocstyle();
	aaa->fshape.vc[0] = 512;
	aaa->fshape.vc[1] = 384;
	aaa->fshape.vr[0] = 512;
	aaa->fshape.vr[1] = 0;
	aaa->fshape.vf[0] = 0;
	aaa->fshape.vf[1] = 384;

	struct style* ccc = allocstyle();
	struct style* ddd = allocstyle();
	ccc->fshape.vc[0] = 768;
	ccc->fshape.vc[1] = 256;
	ccc->fshape.vr[0] = 256;
	ccc->fshape.vr[1] = 0;
	ccc->fshape.vf[0] = 0;
	ccc->fshape.vf[1] = 256;

	struct relation* rel = relationcreate(dbg, bbb, _ent_, 0, wnd, aaa, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(ent, ddd, _ent_, 0, wnd, ccc, _ent_, 0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);


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
