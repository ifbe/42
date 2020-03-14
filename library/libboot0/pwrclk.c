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
void death();
void birth(void*);
void args_delete(void*);
void args_create(int, u8**);
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
	say("[0,2):freeing pwrclk\n");
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

	say("[0,2):inited pwrclk\n");
}




int pwrclkdelete(void* addr)
{
	struct pwrclk* tmp;
	if(0 == addr)return 0;

	tmp = addr;
	say("pwrclkdelete:%.8s\n", &tmp->type);

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

		openwriteclose("universe.bin", 0, pwr, 0x1000000);
		memorydelete(pwr);
	}
	}
	return 0;
}
void* pwrclkcreate(u64 type, void* name, int argc, u8** argv)
{
	int j;
	struct pwrclk* tmp;

	switch(type){
	//app
	case _main_:
	case _win32_:
	case _ndkmain_:{
		tmp = memorycreate(0x1000000, 0);
		//openreadclose("universe.bin", 0, pwr, 0x1000000);

		birth(tmp);
		args_create(argc, argv);

		tmp->type = type;
		return tmp;
	}
	//bare
	case _start_:
	case _efimain_:{
		tmp = (void*)(0x1000000);

		birth(tmp);
		supplycreate(_std_, 0, 0, 0);

		tmp->type = type;
		return tmp;
	}
	//lib
	case _lib42_:
	case _kext_:{
		tmp->type = type;
		return tmp;
	}
	}
	return 0;
}
int pwrclkmodify(int argc, u8** argv)
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
int pwrclkdiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pwrclklinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@pwrclklinkup\n");
	return 0;
}
