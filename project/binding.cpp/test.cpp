#include<iostream>
#define u64 unsigned long long
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define _main_ hex32('m','a','i','n')
#define _wnd_ hex32('w','n','d', 0)

extern "C" {
	void* origin_create(u64 type, void* name, int argc, char** argv);
	void origin_delete(void*);

	void* supply_alloc_prep(u64,u64,u64,u64);
	void supply_create(void*, void* name, int argc, char** argv);
	void supply_delete(void*);

	void sleep_us(int);
	void say(const void*, ...);
}

int main(int argc, char** argv)
{
	void* all = origin_create(_main_, 0, argc, argv);

	void* wnd = supply_alloc_prep(0, _wnd_, 0, 0);
	supply_create(wnd, 0, argc, argv);

	say("sleep 3s\n");
	sleep_us(3*1000*1000);

	supply_delete(wnd);

	origin_delete(all);

	return 0;
}
