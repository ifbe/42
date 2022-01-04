#include "libboot.h"
#define _term_ hex32('t','e','r','m')




int subcmd_create(struct item* wrk, void* url, int argc, u8** argv)
{
	void* thr = 0;
	if(argc <= 1){
		say("./a.out myml xxxx.myml\n");
		say("./a.out mython xxxx.my\n");
		say("./a.out ...\n");
	}
	else if(0 == ncmp(argv[1], "myml", 4)){
		say("mode=myml\n");
		bootupcreate(_myml_, 0, argc-1, &argv[1]);
		bootupdelete(thr);
	}
	else if(0 == ncmp(argv[1], "mython", 6)){
		say("mode=mython\n");
		bootupcreate(_mython_, 0, argc-1, &argv[1]);
		bootupdelete(thr);
	}
	else if(0 == ncmp(argv[1], "term", 4)){
		say("mode=term\n");
		bootupcreate(_term_, 0, argc-1, &argv[1]);
		bootupdelete(thr);
	}
	else if(0 == ncmp(argv[1], "guiapp", 6)){
		say("mode=guiapp\n");
		bootupcreate(_guiapp_, 0, argc-1, &argv[1]);
		bootupdelete(thr);
	}
	return 0;
}



void subcmd_init()
{
}
void subcmd_exit()
{
}