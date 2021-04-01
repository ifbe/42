#include "libboot.h"
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
//
void freerunenv();
void initrunenv();
void freeserial();
void initserial();
//
void death();
void birth(void*);
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);




static struct item* ori;
static int orilen = 0;
void* origin_alloc()
{
	void* addr = &ori[orilen];
	orilen += 1;
	return addr;
}
void origin_recycle()
{
}
void origin_exit()
{
	say("[0,2):origin exiting\n");

	freerunenv();
	freeserial();

	freestdin();
	freestdout();
}
void origin_init(u8* addr)
{
	int j;
	ori = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct item))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)ori[j].tier = _ori_;

	initstdout(addr+0x180000);
	initstdin( addr+0x100000);

	initserial();
	initrunenv();

	say("[0,2):origin inited\n");
}




int origindelete(void* addr)
{
	struct item* tmp;
	if(0 == addr)return 0;

	tmp = addr;
	say("origindelete:%.8s\n", &tmp->type);

	switch(tmp->type){
	case _start_:
	case _efimain_:{
		death();
		break;
	}
	case _main_:
	case _win32_:
	case _ndkmain_:{
		death();

		openwriteclose("universe.bin", 0, ori, 0x1000000);
		memorydelete(ori);
	}
	}
	return 0;
}
void* origincreate(u64 type, void* func, int argc, u8** argv)
{
	int j;
	struct item* tmp;

	switch(type){
	//app
	case _main_:
	case _win32_:
	case _ndkmain_:{
		tmp = memorycreate(0x1000000, 0);
		//openreadclose("universe.bin", 0, ori, 0x1000000);
		birth(tmp);
		say("type=%.8s, func@%p, argc=%d, argv@%p\n", &type, func, argc, argv);
		tmp->type = type;
		return tmp;
	}

	//bare
	case _start_:
	case _efimain_:{
		tmp = (void*)(0x1000000);
		birth(tmp);
		say("type=%.8s, func@%p, argc=%d, argv@%p\n", &type, func, argc, argv);

		tmp->type = type;
		tmp->priv_ptr = argv;
		return tmp;
	}

	//kmod
	case _kext_:{
		tmp->type = type;
		return tmp;
	}

	//lib
	case _lib42_:{
		tmp->type = type;
		return tmp;
	}
	}
	return 0;
}
int originmodify(int argc, u8** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		origincreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
int originsearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == ori[j].type)continue;
		say("[%04x]: %.8s\n", j, &ori[j].type);
		k++;
	}

	if(0 == k)say("empth origin\n");
	return 0;
}




int origin_take(struct item* ori,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int origin_give(struct item* ori,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int origindiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int originlinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@originlinkup\n");
	return 0;
}
