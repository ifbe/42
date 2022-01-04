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




void bootup_init(u8* addr)
{
	say("[2,4):bootup initing\n");

	int j;
	wrk = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct item))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)wrk[j].tier = _wrk_;

	initstdev( addr+0x100000);
	initstdrel(addr+0x180000);

	kernel_init(addr - 0x200000);
	mython_init(addr - 0x200000);

	poller_init(addr - 0x200000);
	waiter_init(addr - 0x200000);

	say("[2,4):bootup inited\n");
}
void bootup_exit()
{
	say("[2,4):bootup exiting\n");

	freestdev();
	freestdrel();

	say("[2,4):bootup exited\n");
}
void* bootup_alloc()
{
	void* addr = &wrk[wrklen];
	wrklen += 1;
	return addr;
}
void bootup_recycle()
{
}




void* bootupcreate(u64 type, void* url, int argc, u8** argv)
{
	struct item* tmp;

	//say("type=%.8s\n",&type);
	if(_subcmd_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _subcmd_;
		subcmd_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_compiler_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _compiler_;
		compiler_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_kernel_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _kernel_;
		kernel_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_myml_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _myml_;
		myml_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_mython_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _mython_;
		mython_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_term_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _term_;
		term_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_guiapp_ == type){
		//self @ 0
		tmp = bootup_alloc();
		tmp->type = _guiapp_;
		guiapp_create(tmp, url, argc, argv);
		return tmp;
	}

	//
	if(_exiter_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _exiter_;

		exiter_create(tmp, url, argc, argv);
		threadcreate(exiter, tmp);
		return tmp;
	}
	if(_pulser_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _pulser_;

		pulser_create(tmp, url, argc, argv);
		threadcreate(pulser, tmp);
		return tmp;
	}
	if(_poller_ == type){
		tmp = bootup_alloc();
		tmp->type = _poller_;

		poller_create(tmp, url, argc, argv);
		threadcreate(poller, tmp);
		return tmp;
	}
	if(_waiter_ == type)
	{
		tmp = bootup_alloc();
		tmp->type = _waiter_;

		waiter_create(tmp, url, argc, argv);
		threadcreate(waiter, tmp);
		return tmp;
	}

	return 0;
}
int bootupdelete(struct item* tmp)
{
	if(0 == tmp)return 0;
	say("bootupdelete:%.8s\n", &tmp->type);

	return 0;
}
int bootupsearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == wrk[j].type)continue;
		say("[%04x]: %.8s\n", j, &wrk[j].type);
		k++;
	}

	if(0 == k)say("empth bootup\n");
	return 0;
}
int bootupmodify(int argc, u8** argv)
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
		bootupcreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}




int bootuplinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@bootuplinkup\n");
	return 0;
}
int bootupdiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int bootup_take(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int bootup_give(struct item* wrk,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}