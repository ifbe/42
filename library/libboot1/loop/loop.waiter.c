#include "libuser.h"
int input(void*, int);




void waiter(void* waiter)
{
	u64 t0;
	u64 dt;
	struct event* ev;

	while(1){
		while(1){
			ev = eventread();
			if(0 == ev)break;
			if(0 == ev->what)return;
			if(_char_ == ev->what)input(ev, 0);
		}
		sleep_us(10*1000);
	}
}
