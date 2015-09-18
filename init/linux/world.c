#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>




static unsigned char* world;




__attribute__((constructor)) void initeverything()
{
	initscreen();
	world = (unsigned char*)malloc(0x800000);		//8M

	//
	initlog(world+0x600000);
	say("beforemain(){\n");
	say("inited memory\n");

	//只是拿地址
	initdisk(world+0x700000);
	enumeratedisk();

	//initarg
	explainarg();

	say("}\n");
}
__attribute__((destructor)) void killeverything()
{
	killscreen();

	killdisk();
	free(world);
}




unsigned long long whereisworld()
{
	return (unsigned long long)world;
}
