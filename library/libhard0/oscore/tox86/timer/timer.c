#include "libhard.h"
void* acpi_gethpet();
void inithpet(void*);
void init825x();        //timer.pit
void initrtc();         //timer.rtc


static int timer_type = 0;


void inittimer()
{
	say("inittimer\n");

	if(0 == timer_type){
		void* hpet = acpi_gethpet();
		if(hpet){
			say("try hpet@%p\n", hpet);
			inithpet(hpet);
			//timer_type = 'h';			//debug
		}
	}

	if(0 == timer_type){
		say("try rtc+pit\n");
		initrtc();
		init825x();
		timer_type = 'p';
	}

	if(0 == timer_type){
		say("try tsc\n");
	}

	say("\n");
}