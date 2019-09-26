#include "libboot.h"




static struct pwrclk* pwr;
static int pwrlen = 0;
void* allocpwrclk()
{
	void* addr = &pwr[pwrlen];
	pwrlen += 1;
	return addr;
}




int pwrclkread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int pwrclkwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int pwrclkstop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pwrclkstart(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	say("@pwrclkstart\n");
	return 0;
}




int pwrclkdelete()
{
	return 0;
}
void* pwrclkcreate(u64 type, void* name)
{
	return 0;
}
int pwrclkmodify(int argc, char** argv)
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
		pwrclkcreate(name, argv[3]);
	}
	return 0;
}
int pwrclksearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == pwr[j].type)continue;
		say("[%04x]: %.8s\n", j, &pwr[j].type);
		k++;
	}

	if(0 == k)say("empth pwrclk\n");
	return 0;
}




void freepwrclk()
{
	//say("[0,4):freeing pwrclk\n");
}
void initpwrclk(u8* addr)
{
	int j;
	pwr = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct pwrclk))
	//for(j=0;j<0x400000;j++)addr[j]=0;
	//for(j=0;j<max;j++)pwr[j].tier = _pwr_;

	//say("[0,4):inited pwrclk\n");
}
