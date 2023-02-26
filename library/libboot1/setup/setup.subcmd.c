#include "libboot.h"
#define _term_ hex32('t','e','r','m')




int subcmd_create(struct item* wrk, void* arg, int argc, u8** argv)
{
	void* thr = 0;
	if(argc <= 1){
		say("./a.out myml xxxx.myml\n");
		say("./a.out mython xxxx.my\n");
		say("./a.out ...\n");
	}
	else if(0 == ncmp(argv[1], "myml", 4)){
		say("mode=myml\n");
		thr = bootup_create(_myml_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "mython", 6)){
		say("mode=mython\n");
		thr = bootup_create(_mython_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "term", 4)){
		say("mode=term\n");
		thr = bootup_create(_term_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "guiapp", 6)){
		say("mode=guiapp\n");
		thr = bootup_create(_guiapp_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else{
		int j;
		u8* ptr = 0;
		for(j=0;j<256;j++){
			if(argv[1][j] < 0x20)break;
			if('.' == argv[1][j])ptr = argv[1]+j;
		}
		if(0 == ptr){
			say("type=unknown\n");
			return 0;
		}

		say("type=%.8s\n", ptr);
		if(0 == ncmp(ptr, ".myml", 5)){
			thr = bootup_create(_myml_, 0, argc, &argv[0]);
			bootup_delete(thr);
		}
	}
	return 0;
}



void subcmd_init()
{
}
void subcmd_exit()
{
}
