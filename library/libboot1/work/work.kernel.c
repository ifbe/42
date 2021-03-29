#include "libboot.h"
#define _2048_ hex32('2','0','4','8')
#define _term_ hex32('t','e','r','m')
#define _clock_ hex64('c','l','o','c','k', 0, 0, 0)
#define _mmioedit_ hex64('m','m','i','o','e','d','i','t')
void* allocstyle();
void inithardware();
void haltwaitforint();




static struct item* origin = 0;
static struct item* worker = 0;
static struct item* device = 0;
static struct item* driver = 0;
volatile static u64 heartbeat_draw = 0;
volatile static u64 heartbeat_poll = 0;




static void kernel_wndctx(struct supply* wnd)
{
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

	struct entity* gamenode = entitycreate(_clock_,0, 0, 0);
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
}




static int kernel_pollloop(struct item* wrk)
{
	int j;
	struct event* ev;
	struct item* dev;
	struct supply* wnd;
	struct halfrel stack[0x80];

	wnd = wrk->priv_ptr;
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	say("pollloop: stack@%p,window@%p\n", stack, wnd);

	while(1){
		for(j=0;j<10;j++){
			dev = &device[j];
			if(_xhci_ == dev->type){
				if(dev->ontaking)dev->ontaking(dev,0, 0,0, 0,0, 0,0);
			}
		}
		//cleanup events
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return 0;

			supply_give(wnd,0, stack,2, 0,0, ev,0);
		}

		heartbeat_poll = timeread();
	}
	return 0;
}
static int kernel_drawloop(struct item* wrk)
{
	int j;
	u64 t0,t1;
	struct supply* wnd;
	struct halfrel stack[0x80];

	wnd = wrk->priv_ptr;
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	say("drawloop: stack@%p,window@%p\n", stack, wnd);

	//loop
	t0 = timeread();
	while(1){
		if(stack[0].pchip != wrk){
			say("what fuck???\n\n\n");
			for(j=0;j<4;j++)say("%d: %p,%p,%x,%x\n", j, stack[j].pchip, stack[j].pfoot, stack[j].type, stack[j].flag);
		}
		//draw frame
		supply_take(wnd,0, stack,2, 0,0, 0,0);

		t1 = timeread();
		//say("dt=%d\n",t1-t0);
		t0 = t1;

		heartbeat_draw = t0;
	}
	return 0;
}
static int kernel_failloop(struct item* wrk)
{
	int j;
	struct item* dev;
	struct supply* wnd = wrk->priv_ptr;

	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;

	//loop
	struct event* ev;
	while(1){
		//draw frame
		supply_take(wnd,0, stack,2, 0,0, 0,0);

		//poll all
		for(j=0;j<10;j++){
			dev = &device[j];
			if(_xhci_ == dev->type){
				if(dev->ontaking)dev->ontaking(dev,0, 0,0, 0,0, 0,0);
			}
		}

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




int kernel_create(struct item* wrk, void* url, int argc, u8** argv)
{
	//debug device
	//struct supply* tty = supplycreate(_tty_, 0, 0, 0);
	struct supply* wnd = supplycreate(_wnd_, 0, 0, 0);
	wrk->priv_ptr = wnd;

	//hardware prepare
	inithardware();

	//
	//kernel_ttyctx(tty);
	kernel_wndctx(wnd);

	//kernel thread
	threadcreate(kernel_drawloop, wrk);
	threadcreate(kernel_pollloop, wrk);

	//check if thread ok, wait at most 10s
	u64 haha = 1;
	u64 curr;
	u64 time = timeread();
	while(1){
		if((0 != heartbeat_draw)&&(0 != heartbeat_poll))break;

		curr = timeread();
		if(0 == curr){
			haha = 100;
			break;
		}

		if(curr > time + haha*1000*1000){
			if(0 == heartbeat_draw){
				say("drawloop: thread fail(%ds/10s)\n", haha);
			}
			if(0 == heartbeat_poll){
				say("pollloop: thread fail(%ds/10s)\n", haha);
			}

			haha += 1;
		}
	}

	//still fail after 10s
	if(haha > 10){
		say("bspcpu: run into 1cpu_0irq mode\n");
		kernel_failloop(wrk);
	}

	//everything ok
	say("haltloop(core=0), sleep wait for int\n");
	while(1){
		time = timeread();
		if(time > heartbeat_draw + 1000*1000*10){
			say("drawloop: stall >10s\n");
		}
		if(time > heartbeat_poll + 1000*1000*10){
			say("pollloop: stall >10s\n");
		}
		haltwaitforint();
	}
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
