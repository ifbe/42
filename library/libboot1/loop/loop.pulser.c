#include "libboot.h"
int cleanevent(struct halfrel stack[]);


static _obj* mpulser = 0;


void pulser(struct item* pulser)
{
	u64 t0;
	u64 dt;
	struct halfrel stack[0x80];
	stack[0].pchip = pulser;

	while(1){
		t0 = timeread_us();

		//logtoall("@pulser: %llx\n", t0);
		give_data_into_them(pulser, _clk_, stack,0, 0,_clk_, &t0,8);

		if(0 == cleanevent(stack))break;

		dt = timeread_us() - t0;
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void* pulser_alloc()
{
	void* p = bootup_alloc();
	return p;
}




void pulser_delete(struct item* wrk, u8* arg)
{
}
void pulser_create(struct item* wrk, u8* arg, int argc, u8** argv)
{
	logtoall("@pulser_create\n");
	wrk->kind = _loop_;
	wrk->type = _pulser_;
	wrk->vfmt = _main_;

	int j;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "thread:new", 10)){
			wrk->vfmt = _thread_;
			logtoall("pulser create: thread new\n");
			threadcreate(pulser, wrk);
		}
	}
}
