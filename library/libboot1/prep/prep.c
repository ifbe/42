#include "libuser.h"
#define _std_  hex32('s','t','d', 0)
#define _bdc_  hex32('b','d','c', 0)
#define _step_ hex32('s','t','e','p')
void* arenalist(void*, int);
void* actorlist(void*, int);
int arenaactor(struct arena* root, struct arena* twig, struct actor* leaf, struct actor* temp);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//
int role_fromfile(int, u8**);
int role_default(int, u8**);



static int roletype = 0;




void role_toycar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_bdc_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_stepcar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_step_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, 0, src, 0, _win_, 0);
}
void role_control(int argc, u8** argv)
{
}
void role_uarthelp(int argc, u8** argv)
{
	void* sys;
	void* act;
	void* win;
	if(argc < 2)return;

	sys = systemcreate(_uart_, argv[1]);
	if(0 == sys)return;

	act = actorcreate(_std_, 0);
	if(act)relationcreate(act, 0, _act_, 0, sys, 0, _fd_, 0);

	win = arenalist("std", 0);
	if(win)relationcreate(act, 0, _act_, 0, win, 0, _win_, 0);

	win = arenacreate(_win_, 0);
	if(win)arenaactor(win, 0, act, 0);
}




void prep()
{
	int ret;
	u8* argv[2];

	//cmdline
	arenacreate(_std_, 0);
	arenacreate(_tray_, 0);

	//try load from file
	ret = role_fromfile(0, argv);
	if(ret > 0)return;

	//at least one window
	ret = role_default(0, argv);
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[256];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 256, argv, 8);
	if(argc < 2)return;

	if(0 == ncmp(argv[1], "test", 4))
	{
		role_fromfile(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "toycar", 6))
	{
		roletype = 1;
		role_toycar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "stepcar", 6))
	{
		roletype = 1;
		role_stepcar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "control", 7))
	{
		roletype = 2;
		role_control(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "uart", 4))
	{
		roletype = 3;
		role_uarthelp(argc-1, &argv[1]);
	}
}
