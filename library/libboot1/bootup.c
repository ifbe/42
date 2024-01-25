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
void bootup_init(u8* addr)
{
	logtoall("[2,4):bootup initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	wrk = (void*)(addr+0x000000);
	wrklen = maxitem-1;
	for(j=0;j<maxitem;j++)wrk[j].tier = _wrk_;

	initstdev( addr+0x100000);
	initstdrel(addr+0x180000);

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
void* bootup_alloc_prep(u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	return 0;
}




void* bootup_create(u64 type, void* arg, int argc, u8** argv)
{
	struct item* tmp;

	//logtoall("type=%.8s\n",&type);
	if(_subcmd_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _subcmd_;
		subcmd_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_compiler_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _compiler_;
		compiler_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_kernel_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _kernel_;
		kernel_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_myml_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _myml_;
		myml_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_mython_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _mython_;
		mython_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_term_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _term_;
		term_create(tmp, arg, argc, argv);
		return tmp;
	}
	if(_guiapp_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _guiapp_;
		guiapp_create(tmp, arg, argc, argv);
		return tmp;
	}

	//
	if(_exiter_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _exiter_;

		exiter_create(tmp, arg, argc, argv);
		threadcreate(exiter, tmp);
		return tmp;
	}
	if(_pulser_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _pulser_;

		pulser_create(tmp, arg, argc, argv);
		threadcreate(pulser, tmp);
		return tmp;
	}
	if(_poller_ == type){
		tmp = bootup_alloc();
		tmp->type = _poller_;

		poller_create(tmp, arg, argc, argv);
		threadcreate(poller, tmp);
		return tmp;
	}
	if(_waiter_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _waiter_;

		waiter_create(tmp, arg, argc, argv);
		threadcreate(waiter, tmp);
		return tmp;
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




int bootup_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@bootupattach\n");
	return 0;
}
int bootup_detach(struct halfrel* self, struct halfrel* peer)
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
			if((0 == act->type)&&(0 == act->hfmt))continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->hfmt, &act->hfmt);
		}
		if(0 == j)logtoall("empty bootup\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == wrk[j].hfmt)break;
			if(0 == cmp(&wrk[j].hfmt, name))logtoall("name=%d,node=%p\n", name, &wrk[j]);
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
