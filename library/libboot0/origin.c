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




#define maxitem (0x100000/sizeof(struct item))
void origin_init(u8* addr)
{
	//logtoall("[0,2):origin initing\n");		//dont uncomment, wont output before init stdout

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	ori = (void*)(addr+0x000000);
	orilen = maxitem-1;
	for(j=0;j<maxitem;j++)ori[j].tier = _ori_;

	initstdout(addr+0x180000);
	initstdin( addr+0x100000);

	initserial();
	initrunenv();

	logtoall("[0,2):origin inited\n");
}
void origin_exit()
{
	logtoall("[0,2):origin exiting\n");

	freerunenv();
	freeserial();

	freestdin();
	freestdout();

	//logtoall("[0,2):origin exitex\n");		//dont uncomment, wont output after free stdout
}




void origin_recycle()
{
}
void* origin_alloc()
{
	void* addr = &ori[orilen];
	orilen -= 1;
	return addr;
}
void* origin_alloc_prep(u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	return 0;
}




void* origin_create(u64 type, void* func, int argc, u8** argv)
{
	int j;
	struct item* tmp=0;

	switch(type){
	//app
	case _main_:
	case _win32_:
	case _ndkmain_:{
		tmp = memoryalloc(0x1000000, 0);
		//openreadclose("universe.bin", 0, ori, 0x1000000);
		birth(tmp);
		logtoall("type=%.8s, func@%p, argc=%d, argv@%p\n", &type, func, argc, argv);
		tmp->type = type;
		return tmp;
	}

	//bare
	case _start_:
	case _efimain_:{
		tmp = (void*)(0x1000000);
		birth(tmp);
		logtoall("type=%.8s, func@%p, argc=%d, argv@%p\n", &type, func, argc, argv);

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
int origin_delete(_obj* tmp)
{
	if(0 == tmp)return 0;
	logtoall("origin_delete:%.8s\n", &tmp->type);

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

		//openwriteclose("universe.bin", 0, ori, 0x1000000);
		memoryfree(ori);
	}
	}
	return 0;
}
int origin_reader(struct item* ori,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int origin_writer(struct item* ori,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}




int origin_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@originattach\n");
	return 0;
}
int origin_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@origindetach\n");
	return 0;
}
int origin_takeby(struct item* ori,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int origin_giveby(struct item* ori,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}




int origincommand_insert(u8* name, u8* arg)
{
	return 0;
}
int origincommand_remove(u8* name)
{
	return 0;
}
int origincommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxitem;j++){
			act = &ori[j];
			if((0 == act->type)&&(0 == act->hfmt))continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->hfmt, &act->hfmt);
		}
		if(0 == j)logtoall("empty origin\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == ori[j].hfmt)break;
			if(0 == cmp(&ori[j].hfmt, name))logtoall("name=%d,node=%p\n", name, &ori[j]);
			break;
		}
	}
	return 0;
}
int origincommand_modify(int argc, u8** argv)
{
	return 0;
}
void* origincommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("origin insert name arg\n");
		logtoall("origin search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//origin create name arg
		origincommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//origin search <name>
		origincommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
