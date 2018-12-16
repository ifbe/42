#include "libuser.h"
#define _std_ hex32('s','t','d', 0)
#define _rccar_ hex64('r','c','c','a','r',0,0,0)
void* arenalist(void*, int);
void* actorlist(void*, int);
int arenaactor(struct arena* win, struct actor* act);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);



static int roletype = 0;
void role_delete()
{
}
void role_create()
{
	if(roletype)return;

	//+libhard
	//ahci = devicecreate()
	//xhci = devicecreate()

	//+libsoft
	//hack = arterycreate("HACK://127.0.0.1:2222");
	//hack = arterycreate("CHAT://127.0.0.1:4444");
	//sshd = arterycreate( "SSH://127.0.0.1:2222");
	//http = arterycreate("HTTP://127.0.0.1:4444");

	//+libuser
	arenacreate(_win_,  0);
}




void role_rccar(int argc, u8** argv)
{
	void* win;
	void* act;
	act = actorcreate(_rccar_, 0);

	win = arenalist("std", 0);
	relationcreate(act, 0, _act_, win, 0, _win_);

	win = arenalist("joy", 0);
	relationcreate(act, 0, _act_, win, 0, _win_);
	return;
}
void role_control(int argc, u8** argv)
{
}
void role_uarthelp(int argc, u8** argv)
{
	if(argc < 2)return;
	void* sys = systemcreate(_uart_, argv[1]);
	void* act = actorcreate(_std_, 0);
	relationcreate(act, 0, _act_, sys, 0, _fd_);

	void* win = arenalist("std", 0);
	relationcreate(act, 0, _act_, win, 0, _win_);

	win = arenacreate(_win_, 0);
	arenaactor(win, act);
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(argc < 2)return;

	if(0 == ncmp(argv[1], "rccar", 5))
	{
		roletype = 1;
		role_rccar(argc-1, &argv[1]);
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
