#include "libuser.h"
//libuser1
void freeactor();
void initactor(void*);
//libuser0
void freearena();
void initarena(void*);
//libsoft1
void freeartery();
void initartery(void*);
//libsoft0
void freesystem();
void initsystem(void*);
//libhard1
void freedriver();
void initdriver(void*);
//libhard0
void freedevice();
void initdevice(void*);
//libboot1
void freeworker();
void initworker(void*);
//libboot0
void freepwrclk();
void initpwrclk(void*);
//
int arenaactor(struct arena* root, struct arena* twig, struct actor* leaf, struct actor* temp);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//
int role_fromfile(int, u8**);
int role_default(int, u8**);




void birth(void* addr)
{
	//libboot
	initpwrclk(addr+0x000000);
	initworker(addr+0x200000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x600000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0xa00000);

	//libuser
	initarena( addr+0xc00000);
	initactor( addr+0xe00000);
}
void death()
{
	//libuser
	freeactor();
	freearena();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freeworker();
	freepwrclk();
}




void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[256];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	if('=' == buf[4]){
		argc = 2;
		argv[0] = buf;
		argv[1] = &buf[5];
	}
	else{
		argc = str2arg(buf, len, tmp, 256, argv, 8);
		if(argc <= 1){
			argc = 0;
			argv[0] = 0;
		}
	}
	role_fromfile(argc, argv);
}
void prep()
{
	int ret;
	u8* argv[2];

	//cmdline
	arenacreate(_std_, 0);
/*
	//try load from file
	ret = role_fromfile(0, argv);
	if(ret > 0)return;

	//at least one window
	ret = role_default(0, argv);
*/
}