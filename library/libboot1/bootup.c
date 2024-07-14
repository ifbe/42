#include "libboot.h"
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//
void exiter_init(void*);
void exiter_create(struct item*, u8*, int, u8**);
void exiter(void*);
//
void pulser_init(void*);
void pulser_create(struct item*, u8*, int, u8**);
void pulser(void*);
//
void poller_init(void*);
void poller_create(struct item*, u8*, int, u8**);
void poller(void*);
//
void waiter_init(void*);
void waiter_create(struct item*, u8*, int, u8**);
void waiter(void*);
//
void compiler_init(void*);
void compiler_create(struct item*, u8*, int, u8**);
//
void kernel_init(void*);
void kernel_create(struct item*, u8*, int, u8**);
//
void myml_init(void*);
void myml_create(struct item*, u8*, int, u8**);
void myml_delete(struct item*);
//
void mython_init(void*);
void mython_create(struct item*, u8*, int, u8**);
//
void term_init(void*);
void term_create(struct item*, u8*, int, u8**);
//
void guiapp_init(void*);
void guiapp_create(struct item*, u8*, int, u8**);
//
void subcmd_init(void*);
void subcmd_create(struct item*, u8*, int, u8**);




static struct item* wrk;
static int wrklen = 0;




#define maxitem (0x100000/sizeof(struct item))
void bootup_init(u8* addr, int size)
{
	logtoall("[2,4):bootup initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	wrk = (void*)(addr+0x000000);
	wrklen = maxitem-1;
	for(j=0;j<maxitem;j++)wrk[j].tier = _wrk_;

	initstdev( addr+0x100000);
	initstdrel(addr+0x180000);

	subcmd_init(wrk);

	kernel_init(addr - 0x200000);
	mython_init(addr - 0x200000);

	poller_init(addr - 0x200000);
	waiter_init(addr - 0x200000);

	logtoall("[2,4):bootup inited\n");
}
void bootup_exit()
{
	logtoall("[2,4):bootup exiting\n");

	freestdev();
	freestdrel();

	logtoall("[2,4):bootup exited\n");
}




void bootup_recycle()
{
}
void* bootup_alloc()
{
	void* addr = &wrk[wrklen];
	wrklen -= 1;
	return addr;
}
void* bootup_alloc_fromtype(u64 type)
{
	_obj* obj = bootup_alloc();
	if(0 == obj)return 0;

	//obj->tier = tier;		//should be tier: bootup
	//obj->kind = kind;		//should be class: usb
	obj->type = type;		//should be type: xhci
	//obj->vfmt = vfmt;		//should be model: intelxhci
	return obj;
}




int bootup_create(_obj* tmp, void* arg, int argc, u8** argv)
{
	//logtoall("type=%.8s\n",&type);
	switch(tmp->type){
	case _subcmd_:
		subcmd_create(tmp, arg, argc, argv);
		break;
	case _compiler_:
		compiler_create(tmp, arg, argc, argv);
		break;
	case _kernel_:
		kernel_create(tmp, arg, argc, argv);
		break;
	case _myml_:
		myml_create(tmp, arg, argc, argv);
		break;
	case _mython_:
		mython_create(tmp, arg, argc, argv);
		break;
	case _term_:
		term_create(tmp, arg, argc, argv);
		break;
	case _guiapp_:
		guiapp_create(tmp, arg, argc, argv);
		break;

	//
	case _exiter_:
		exiter_create(tmp, arg, argc, argv);
		//threadcreate(exiter, tmp);
		break;
	case _pulser_:
		pulser_create(tmp, arg, argc, argv);
		//threadcreate(pulser, tmp);
		break;
	case _poller_:
		poller_create(tmp, arg, argc, argv);
		//threadcreate(poller, tmp);
		break;
	case _waiter_:
		waiter_create(tmp, arg, argc, argv);
		//threadcreate(waiter, tmp);
		break;
	}

	return 0;
}
int bootup_delete(struct item* tmp)
{
	if(0 == tmp)return 0;
	logtoall("bootup_delete:%.8s\n", &tmp->type);

	switch(tmp->type){
	case _myml_:
		myml_delete(tmp);
		break;
	}
	return 0;
}
int bootup_reader(struct item* wrk,void* foot, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int bootup_writer(struct item* wrk,void* foot, p64 arg,int idx, void* buf,int len)
{
	return 0;
}




int bootup_attach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	logtoall("@bootupattach\n");
	return 0;
}
int bootup_detach(_obj* ent,void* foot, struct halfrel* self, struct halfrel* peer)
{
	logtoall("@bootupdetach\n");
	return 0;
}
int bootup_takeby(struct item* wrk,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int bootup_giveby(struct item* wrk,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}




int bootupcommand_insert(u8* name, u8* arg)
{
	return 0;
}
int bootupcommand_remove(u8* name)
{
	return 0;
}
int bootupcommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxitem;j++){
			act = &wrk[j];
			if(0 == act->type)continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->kind, &act->type, &act->vfmt);
		}
		if(0 == j)logtoall("empty bootup\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == wrk[j].type)break;
			if(0 == cmp(&wrk[j].type, name))logtoall("name=%d,node=%p\n", name, &wrk[j]);
			break;
		}
	}
	return 0;
}
int bootupcommand_modify(int argc, u8** argv)
{
	return 0;
}
void* bootupcommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("bootup insert name arg\n");
		logtoall("bootup search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//bootup create name arg
		bootupcommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//bootup search <name>
		bootupcommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
