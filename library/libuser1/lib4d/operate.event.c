#include "libuser.h"
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);




//ev kbd 0xd
void event(u8* buf, int len)
{
	int j,k;
	u8* p;
	u8* argv[8];
	u8 tmp[0x1000];
	struct event ev;

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 3)return;

	//clean
	ev.why = ev.what = ev.where = ev.when = 0;

	//why
	p = (u8*)&ev.why;
	for(j=0;j<8;j++)
	{
		if(0 == argv[1][j])break;
		p[j] = argv[1][j];
	}

	//what
	p = (u8*)&ev.what;
	for(j=0;j<8;j++)
	{
		if(0 == argv[2][j])break;
		p[j] = argv[2][j];
	}

	say("ev:%llx,%llx,%llx,%llx\n", ev.why, ev.what, ev.where, ev.when);
	eventwrite(ev.why, ev.what, 0, 0);
}