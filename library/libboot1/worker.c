#include "libboot.h"




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
int workerstart(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	say("@workerstart\n");
	return 0;
}




int workerdelete()
{
	return 0;
}
void* workercreate(u64 type, void* name)
{
	return 0;
}
int workermodify(int argc, char** argv)
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
		workercreate(name, argv[3]);
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
	//say("[0,4):freeing worker\n");
}
void initworker(u8* addr)
{
	int j;
	wrk = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct worker))
	//for(j=0;j<0x400000;j++)addr[j]=0;
	//for(j=0;j<max;j++)wrk[j].tier = _wrk_;

	//say("[0,4):inited worker\n");
}
