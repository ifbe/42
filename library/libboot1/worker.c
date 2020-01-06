#include "libboot.h"
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
int args_delete(void*);
void args_create(int, u8**);
//
void initexiter(void*);
void exiter_create(struct worker*, u8*, int, u8**);
void exiter(void*);
//
void initpulser(void*);
void pulser_create(struct worker*, u8*, int, u8**);
void pulser(void*);
//
void initpoller(void*);
void poller_create(struct worker*, u8*, int, u8**);
void poller(void*);
//
void initrealer(void*);
void realer_create(struct worker*, u8*, int, u8**);
void realer(void*);
//
void initwaiter(void*);
void waiter_create(struct worker*, u8*, int, u8**);
void waiter(void*);




static struct worker* wrk;
static int wrklen = 0;
void* allocworker()
{
	void* addr = &wrk[wrklen];
	wrklen += 1;
	return addr;
}




int workerread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int workerwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int workerstop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int workerstart(struct halfrel* self, struct halfrel* peer)
{
	say("@workerstart\n");
	return 0;
}




int workerdelete(void* addr)
{
	struct worker* tmp;
	if(0 == addr)return 0;

	tmp = addr;
	say("workerdelete:%.8s\n", &tmp->type);

	return 0;
}
void* workercreate(u64 type, void* url, int argc, u8** argv)
{
	struct worker* tmp;

	if(0 == type){
		args_create(argc, argv);
		if(0 == wrk->type){
			tmp = allocworker();
			tmp->type = _waiter_;
		}

		switch(wrk->type){
			case _exiter_:exiter(wrk);break;
			case _pulser_:pulser(wrk);break;
			case _poller_:poller(wrk);break;
			case _realer_:realer(wrk);break;
			case _waiter_:waiter(wrk);break;
		}
		return 0;
	}

	if(_exiter_ == type)
	{
		tmp = allocworker();
		tmp->type = _exiter_;

		exiter_create(tmp, url, argc, argv);
		if(tmp != wrk)threadcreate(exiter, wrk);
		return tmp;
	}
	if(_pulser_ == type)
	{
		tmp = allocworker();
		tmp->type = _pulser_;

		pulser_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_poller_ == type){
		tmp = allocworker();
		tmp->type = _poller_;

		poller_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_realer_ == type)
	{
		tmp = allocworker();
		tmp->type = _realer_;

		realer_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_waiter_ == type)
	{
		tmp = allocworker();
		tmp->type = _waiter_;

		waiter_create(tmp, url, argc, argv);
		return tmp;
	}

	return 0;
}
int workermodify(int argc, u8** argv)
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
		workercreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
int workersearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == wrk[j].type)continue;
		say("[%04x]: %.8s\n", j, &wrk[j].type);
		k++;
	}

	if(0 == k)say("empth worker\n");
	return 0;
}




void freeworker()
{
	//say("[2,4):freeing worker\n");

	freestdev();
	freestdrel();
}
void initworker(u8* addr)
{
	int j;
	wrk = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct worker))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)wrk[j].tier = _wrk_;

	initstdev( addr+0x100000);
	initstdrel(addr+0x180000);

	initpoller(addr - 0x200000);
	initrealer(addr - 0x200000);

	//say("[2,4):inited worker\n");
}
