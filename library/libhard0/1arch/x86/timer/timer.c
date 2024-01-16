#include "libhard.h"
void* acpi_hpet_addr();
int acpi_hpet_replacepit();
//
int inithpet(void*);
u64 hpet_gettime_ms();
u64 hpet_gettime_us();
u64 hpet_gettime_ns();
//
int init825x();
u64 pit8254_ms();
u64 pit8254_us();
u64 pit8254_ns();
//
int initrtc();


static int timer_type = 0;




u64 archtimer_ms()
{
	if('h' == timer_type){
		return hpet_gettime_ms();
	}
	return pit8254_ms();
}
u64 archtimer_us()
{
	if('h' == timer_type){
		return hpet_gettime_us();
	}
	return pit8254_us();
}
u64 archtimer_ns()
{
	if('h' == timer_type){
		return hpet_gettime_ns();
	}
	return pit8254_ns();
}




void inittimer()
{
	logtoall("inittimer\n");

	int ret = 0;
	void* hpet_mmio = acpi_hpet_addr();
	int hpet_prefer = acpi_hpet_replacepit();

	if(0 == timer_type){
		if(hpet_mmio && hpet_prefer){
			logtoall("try hpet@%p\n", hpet_mmio);
			ret = inithpet(hpet_mmio);
			if(ret)timer_type = 'h';
		}
	}

	if(0 == timer_type){
		logtoall("try rtc+pit\n");
		initrtc();
		ret = init825x();
		if(ret)timer_type = 'p';
	}

	if(0 == timer_type){
		logtoall("no more to try, forever hlt\n");
		while(1)asm("hlt");		//no more way, timer must work
		logtoall("try tsc\n");
	}

	logtoall("\n");
}