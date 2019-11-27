#include "libboot.h"
#define _clk_ hex32('c','l','k',0)
u64 timeread();
void sleep_us();




void pulser(struct worker* tmp)
{
	u64 t0;
	u64 dt;
	while(1){
		t0 = timeread();

		say("@pulser: %llx\n", t0);
		relationwrite(tmp, _clk_, 0, 0, &t0, 8);

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