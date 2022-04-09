#include "libboot.h"
#define _2048_ hex32('2','0','4','8')
#define _term_ hex32('t','e','r','m')
#define _clock_ hex64('c','l','o','c','k', 0, 0, 0)
#define _mmioedit_ hex64('m','m','i','o','e','d','i','t')
void* style_alloc();
void* style_recycle();
void inithardware();
void haltwaitforint();




static struct item* origin = 0;
static struct item* worker = 0;
static struct item* device = 0;
static struct item* driver = 0;
volatile static u64 heartbeat_draw = 0;
volatile static u64 heartbeat_poll = 0;




static void kernel_wndctx(_obj* wnd)
{
	//wnd-> = ;
	struct style* toterm = style_alloc();
	toterm->fshape.vc[0] = wnd->whdf.width/2;
	toterm->fshape.vc[1] = wnd->whdf.height/2;
	toterm->fshape.vc[2] = 0;
	toterm->fshape.vc[3] = 9999.0;		//event owner
	toterm->fshape.vr[0] = wnd->whdf.width/2;
	toterm->fshape.vr[1] = 0;
	toterm->fshape.vf[0] = 0;
	toterm->fshape.vf[1] = wnd->whdf.height/2;
	struct style* togame = style_alloc();
	togame->fshape.vc[0] = wnd->whdf.width*3/4;
	togame->fshape.vc[1] = wnd->whdf.height/4;
	togame->fshape.vc[2] = 0;
	togame->fshape.vc[3] = 0;
	togame->fshape.vr[0] = wnd->whdf.width/5;
	togame->fshape.vr[1] = 0;
	togame->fshape.vf[0] = 0;
	togame->fshape.vf[1] = wnd->whdf.height/5;
	struct style* toedit = style_alloc();
	toedit->fshape.vc[0] = wnd->whdf.width*3/4;
	toedit->fshape.vc[1] = wnd->whdf.height*3/4;
	toedit->fshape.vc[2] = 0;
	toedit->fshape.vc[3] = 0;
	toedit->fshape.vr[0] = wnd->whdf.width/5;
	toedit->fshape.vr[1] = 0;
	toedit->fshape.vf[0] = 0;
	toedit->fshape.vf[1] = wnd->whdf.height/5;


	//things
	_obj* termnode = entity_create(_term_,0, 0, 0);
	struct style* termfoot = style_alloc();

	_obj* gamenode = entity_create(_clock_,0, 0, 0);
	struct style* gamefoot = style_alloc();

	_obj* editnode = entity_create(_mmioedit_,0, 0, 0);
	struct style* editfoot = style_alloc();


	//relation
	struct relation* rel;
	rel = relationcreate(termnode,termfoot, _ent_,0, wnd,toterm, _ent_,0);
	relationattach((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(gamenode,gamefoot, _ent_,0, wnd,togame, _ent_,0);
	relationattach((void*)&rel->srcchip, (void*)&rel->dstchip);

	rel = relationcreate(editnode,editfoot, _ent_,0, wnd,toedit, _ent_,0);
	relationattach((void*)&rel->srcchip, (void*)&rel->dstchip);
}




static int kernel_pollloop(struct item* wrk)
{
	int j;
	u64 t0,t1;
	struct event* ev;
	struct item* dev;
	struct halfrel stack[0x80];

	_obj* wnd = wrk->priv_ptr;
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	say("pollloop: stack@%p,window@%p\n", stack, wnd);

	while(1){
		t0 = timeread_us();
		heartbeat_poll = t0+1;

		for(j=0;j<10;j++){
			dev = &device[j];
			if( (_xhci_ == dev->type) | (_eth_ == dev->type) ){
				if(dev->ontaking)dev->ontaking(dev,0, 0,0, 0,0, 0,0);
			}
		}
		//cleanup events
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what){
				say("exit event, ignore it@%p\n", ev);
				break;
			}

			supply_giveby(wnd,0, stack,2, 0,0, ev,0);
		}

		//200 time per sec
		while(1){
			t1 = timeread_us();
			if(t0+5000 < t1)break;
			haltwaitforint();
		}
	}
	return 0;
}
static int kernel_drawloop(struct item* wrk)
{
	int j;
	u64 t0,t1;
	struct halfrel stack[0x80];

	_obj* wnd = wrk->priv_ptr;
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	say("drawloop: stack@%p,window@%p\n", stack, wnd);

	//loop
	while(1){
		t0 = timeread_us();
		heartbeat_draw = t0+1;

		if(stack[0].pchip != wrk){
			say("what fuck???\n\n\n");
			for(j=0;j<4;j++)say("%d: %p,%p,%x,%x\n", j, stack[j].pchip, stack[j].pfoot, stack[j].nodetype, stack[j].foottype);
		}

		//draw frame
		supply_takeby(wnd,0, stack,2, 0,0, 0,0);

		//30 frame per sec
		while(1){
			t1 = timeread_us();
			if(t0+30000 < t1)break;
			haltwaitforint();
		}
	}
	return 0;
}



#define STALL_SEC 3
static int kernel_failloop(struct item* wrk)
{
	int j;
	struct item* dev;
	_obj* wnd = wrk->priv_ptr;

	struct halfrel stack[0x80];
	stack[0].pchip = wrk;
	stack[1].pchip = wnd;
	say("failloop: (one core,poll mode)\n");

	//loop
	struct event* ev;
	u64 time;
	while(1){
		time = timeread_us();

		//drawloop fail, i have to draw
		if( (0 == heartbeat_draw) | (time > heartbeat_draw + 1000*1000*STALL_SEC) ){
			//if(curr <= STALL_SEC)say("drawloop fail after %d sec\n", curr);
			supply_takeby(wnd,0, stack,2, 0,0, 0,0);
		}//draw

		//pollloop fail, i have to poll
		if( (0 == heartbeat_poll) | (time > heartbeat_poll + 1000*1000*STALL_SEC) ){
			//if(curr <= STALL_SEC)say("pollloop fail after %d sec\n", curr);

			//poll all
			for(j=0;j<10;j++){
				dev = &device[j];
				if( (_xhci_ == dev->type)|(_eth_ == dev->type) ){
					if(dev->ontaking)dev->ontaking(dev,0, 0,0, 0,0, 0,0);
				}
			}

			//cleanup events
			while(1){
				ev = eventread();
				if(0 == ev)break;
				if(0 == ev->what)break;

				supply_giveby(wnd,0, stack,2, 0,0, ev,0);
			}
		}//poll
	}
	return 0;
}
static int kernel_idleloop(struct item* wrk)
{
	u64 time;
	say("idleloop(core=0), sleep wait for int\n");

	while(1){
		time = timeread_us();
		if(time > heartbeat_draw + 1000*1000*STALL_SEC){
			say("drawloop: stall %ds\n", (time-heartbeat_draw)/1000/1000);
		}
		if(time > heartbeat_poll + 1000*1000*STALL_SEC){
			say("pollloop: stall %ds\n", (time-heartbeat_poll)/1000/1000);
			//break;
		}
		//
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		//
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		//
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		//
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
		haltwaitforint();
	}
	return 0;
}




int kernel_create(struct item* wrk, void* url, int argc, u8** argv)
{
	//debug device
	//_obj* tty = supply_create(_tty_, 0, 0, 0);
	_obj* wnd = supply_create(_wnd_, 0, 0, 0);
	wrk->priv_ptr = wnd;

	//hardware prepare
	inithardware();

	//
	//kernel_ttyctx(tty);
	kernel_wndctx(wnd);

	//start work
	int success = 1;
	if(1){
		//kernel thread
		threadcreate(kernel_drawloop, wrk);
		threadcreate(kernel_pollloop, wrk);

		//wait 1s
		sleep_us(1000*1000);
		if( (0 == heartbeat_draw) | (0 == heartbeat_poll) )success = 0;
	}
	else{
		//processcreate("/init");
	}

	//become idlethread, or faildebug
	if(success)kernel_idleloop(wrk);
	kernel_failloop(wrk);

	return 0;
}




void kernel_exit()
{
}
void kernel_init(void* addr)
{
	origin = addr + 0x000000;
	worker = addr + 0x200000;
	device = addr + 0x400000;
	driver = addr + 0x600000;
}
