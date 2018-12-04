#include "libuser.h"
#define _rccar_ hex64('r','c','c','a','r',0,0,0)
void* arenalist(void*, int);
int arenaactor(struct arena* win, struct actor* act);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);



static int roletype = 0;
void role_create()
{
	void* win;
	void* act;
	if(roletype)
	{
		win = arenacreate(_win_, 0);
		act = actorcreate(_rccar_, 0);
		arenaactor(win, act);

		win = arenalist("term", 0);
		relationcreate(act, 0, _act_, win, 0, _win_);
		return;
	}

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
void role_delete()
{
}
void role(u8* buf, int len)
{
	int j,k;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 2)return;

	if(0 == ncmp(argv[1], "rccar", 5))roletype = 1;
}