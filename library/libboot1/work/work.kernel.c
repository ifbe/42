#include "libboot.h"
#define _2048_ hex32('2','0','4','8')
#define _term_ hex32('t','e','r','m')
#define _mmioedit_ hex64('m','m','i','o','e','d','i','t')
void* allocstyle();
void inithardware();




static struct origin* origin = 0;
static struct worker* worker = 0;
static struct device* device = 0;
static struct driver* driver = 0;




int kernel_pollall()
{
	int j;
	struct device* dev;
	for(j=0;j<10;j++){
		dev = &device[j];
		if(_xhci_ == dev->type){
			if(dev->ontaking)dev->ontaking(dev,0, 0,0, 0,0, 0,0);
		}
	}
	return 0;
}
int kernel_create(struct worker* wrk, void* url, int argc, u8** argv)
{
	//screen
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	//wnd-> = ;
	//wnd-> = ;
	struct style* toterm = allocstyle();
	toterm->fshape.vc[0] = wnd->width/2;
	toterm->fshape.vc[1] = wnd->height/2;
	toterm->fshape.vc[2] = 0;
	toterm->fshape.vc[3] = 9999.0;		//event owner
	toterm->fshape.vr[0] = wnd->width/2;
	toterm->fshape.vr[1] = 0;
	toterm->fshape.vf[0] = 0;
	toterm->fshape.vf[1] = wnd->height/2;
	struct style* togame = allocstyle();
	togame->fshape.vc[0] = wnd->width*3/4;
	togame->fshape.vc[1] = wnd->height/4;
	togame->fshape.vc[2] = 0;
	togame->fshape.vc[3] = 0;
	togame->fshape.vr[0] = wnd->width/5;
	togame->fshape.vr[1] = 0;
	togame->fshape.vf[0] = 0;
	togame->fshape.vf[1] = wnd->height/5;
	struct style* toedit = allocstyle();
	toedit->fshape.vc[0] = wnd->width*3/4;
	toedit->fshape.vc[1] = wnd->height*3/4;
	toedit->fshape.vc[2] = 0;
	toedit->fshape.vc[3] = 0;
	toedit->fshape.vr[0] = wnd->width/5;
	toedit->fshape.vr[1] = 0;
	toedit->fshape.vf[0] = 0;
	toedit->fshape.vf[1] = wnd->height/5;


	//things
	struct entity* termnode = entitycreate(_term_,0, 0, 0);
	struct style* termfoot = allocstyle();

	struct entity* gamenode = entitycreate(_2048_,0, 0, 0);
	struct style* gamefoot = allocstyle();

	struct entity* editnode = entitycreate(_mmioedit_,0, 0, 0);
	struct style* editfoot = allocstyle();


	//relation
	struct relation* rel;
	rel = relationcreate(termnode,termfoot, _ent_,0, wnd,toterm, _ent_,0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(gamenode,gamefoot, _ent_,0, wnd,togame, _ent_,0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(editnode,editfoot, _ent_,0, wnd,toedit, _ent_,0);
	relationlinkup((void*)&rel->srcchip, (void*)&rel->dstchip);


	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	supplyread(wnd,0, stack,2, 0,0, 0,0);
	inithardware();


	//loop
	struct event* ev;
	while(1){
		//draw frame
		supplyread(wnd,0, stack,2, 0,0, 0,0);

		kernel_pollall();

		//cleanup events
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return 0;

			supplywrite(wnd,0, stack,2, 0,0, ev,0);
		}
	}
	return 0;
}




void freekernel()
{
}
void initkernel(void* addr)
{
	origin = addr + 0x000000;
	worker = addr + 0x200000;
	device = addr + 0x400000;
	driver = addr + 0x600000;
}