#include "libboot.h"
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
//
void deleteserial();
void createserial();
void boarddelete();
void boardcreate();
//
int memorydelete(void*);
void* memorycreate(int size, int flag);
void death();
void birth(void*);
//
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);




static struct pwrclk* pwr;
static int pwrlen = 0;
void* allocpwrclk()
{
	void* addr = &pwr[pwrlen];
	pwrlen += 1;
	return addr;
}
void freepwrclk()
{
	//say("[0,2):freeing pwrclk\n");
	freestdout();
	freestdin();
}
void initpwrclk(u8* addr)
{
	int j;
	pwr = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct pwrclk))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)pwr[j].tier = _pwr_;

	createserial();
	boardcreate();

	initstdin( addr+0x100000);
	initstdout(addr+0x180000);

	//say("[0,2):inited pwrclk\n");
}




int pwrclkdelete(void* addr)
{
	struct pwrclk* tmp;
	if(0 == addr)return 0;

	tmp = addr;
	say("workerdelete:%.8s\n", &tmp->type);

	switch(tmp->type){
		case _main_:
		case _win32_:{
			openwriteclose("universe.bin", 0, pwr, 0x1000000);
			death();
		}
	}
	return 0;
}
void* pwrclkcreate(u64 type, void* name, int argc, char** argv)
{
	int j;
	struct pwrclk* tmp;

	//app
	if( (_main_ == type)|(_win32_ == type) ){
		tmp = memorycreate(0x1000000, 0);

		birth(tmp);

		tmp->type = type;
		return tmp;
	}

	//bare
	if( (_start_ == type)|(_efimain_ == type) ){
		tmp = (void*)(0x1000000);

		tmp->type = _start_;
		return tmp;
	}

	//lib
	if(_lib42_ == type){
		return tmp;
	}
	if(_kext_ == type){
		return tmp;
	}

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
		pwrclkcreate(name, argv[3], argc-3, &argv[3]);
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
int pwrclkstart(struct halfrel* self, struct halfrel* peer)
{
	say("@pwrclkstart\n");
	return 0;
}