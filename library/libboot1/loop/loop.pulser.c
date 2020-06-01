#include "libboot.h"
void give_data_into_them(void*,int, void*,int, void*,int, void*,int);




void pulser(struct worker* pulser)
{
	u64 t0;
	u64 dt;
	struct halfrel stack[0x80];

	while(1){
		t0 = timeread();

		//say("@pulser: %llx\n", t0);
		give_data_into_them(pulser, _clk_, stack,0, 0,0, &t0,8);

		dt = timeread() - t0;
		if(dt < 16000)sleep_us(16000-dt);
	}
}




void pulser_delete(struct worker* wrk, u8* arg)
{
}
void pulser_create(struct worker* wrk, u8* arg, int argc, u8** argv)
{
}
