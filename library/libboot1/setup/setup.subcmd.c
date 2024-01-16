#include "libboot.h"
#define _term_ hex32('t','e','r','m')
//
void* poller_alloc();
void poller(void*);




int subcmd_create(struct item* wrk, void* arg, int argc, u8** argv)
{
	void* thr = 0;
	if(argc <= 1){
		void* std = supply_alloc_prep(0, _std_, 0, 0);
		supply_create(std, 0, 0, 0);

		void* mpoller = poller_alloc();
		if(mpoller)poller(mpoller);
	}
	else if(0 == ncmp(argv[1], "help", 4)){
		logtoall("./a.out myml xxxx.myml\n");
		logtoall("./a.out mython xxxx.my\n");
		logtoall("./a.out ...\n");
	}
	else if(0 == ncmp(argv[1], "myml", 4)){
		logtoall("mode=myml\n");
		thr = bootup_create(_myml_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "mython", 6)){
		logtoall("mode=mython\n");
		thr = bootup_create(_mython_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "term", 4)){
		logtoall("mode=term\n");
		thr = bootup_create(_term_, 0, argc-1, &argv[1]);
		bootup_delete(thr);
	}
	else if(0 == ncmp(argv[1], "guiapp", 6)){
		logtoall("mode=guiapp\n");
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
			logtoall("type=unknown\n");
			return 0;
		}

		logtoall("type=%.8s\n", ptr);
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
