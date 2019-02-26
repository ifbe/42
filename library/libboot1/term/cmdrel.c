#include "libboot.h"
void* arenalist(u8* buf, int len);
void* actorlist(u8* buf, int len);
int arenaactor(void*, void*);
//
int ncmp(void*, void*, int);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);




//rel win@12 -> 2048@7
void relation(u8* buf, int len)
{
	int j,k;
	struct arena* win;
	struct actor* act;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 4)return;

	if(0 == ncmp(argv[1], "add", 3))
	{
		win = arenalist(argv[2], 0);
		act = actorlist(argv[3], 0);
		arenaactor(win, act);
		return;
	}

	if(0 == ncmp(argv[1], "del", 33))
	{
		return;
	}
}