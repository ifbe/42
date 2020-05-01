#include<iostream>
#define u64 unsigned long long
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define _main_ hex32('m','a','i','n')

extern "C" {
	void* origincreate(u64 type, void* name, int argc, char** argv);
	void origindelete(void*);
	void say(const void*, ...);
}

int main(int argc, char** argv)
{
	void* all = origincreate(_main_, 0, argc, argv);
	say("here\n");
	origindelete(all);
	return 0;
}
