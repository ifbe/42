#include "libuser.h"
void* allocstyle();
void* allocpinid();
void* actorstart(void*, void*, void*, void*);
void* actorstop(void*, void*, void*, void*);
void* arenalist(u8* buf, int len);
void* actorlist(u8* buf, int len);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);




void defaultstyle_2d(struct style* sty, int w, int h, int d)
{
	sty->vc[0] = w/2;
	sty->vc[1] = h/2;
	sty->vc[2] = 0.0;

	sty->vr[0] = w/2;
	sty->vr[1] = 0;
	sty->vr[2] = 0;

	sty->vf[0] = 0;
	sty->vf[1] = h/2;
	sty->vf[2] = 0;

	sty->vu[0] = 0;
	sty->vu[1] = 0;
	sty->vu[2] = (w+h)/4;
}
void defaultstyle_3d(struct style* sty, struct style* tar)
{
	sty->vc[0] = tar->vc[0];
	sty->vc[1] = tar->vc[1];
	sty->vc[2] = tar->vc[2];

	sty->vr[0] = tar->vr[0];
	sty->vr[1] = tar->vr[1];
	sty->vr[2] = tar->vr[2];

	sty->vf[0] = tar->vf[0];
	sty->vf[1] = tar->vf[1];
	sty->vf[2] = tar->vf[2];

	sty->vu[0] = tar->vu[0];
	sty->vu[1] = tar->vu[1];
	sty->vu[2] = tar->vu[2];
}
int arenaactor(struct arena* ccc, struct actor* act)
{
	int w,h;
	struct style* sty;
	struct pinid* pin;
	struct arena* win;
	struct relation* rel;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocpinid();
	if(0 == pin)return 0;

	win = ccc;
	switch(ccc->fmt)
	{
		case _bg3d_:
		case _fg3d_:
		{
			rel = ccc->irel0;
			if(0 == rel)return 0;

			win = (void*)(rel->srcchip);
			defaultstyle_3d(sty, &win->target);

			break;
		}
		case _bg2d_:
		case _fg2d_:
		{
			rel = ccc->irel0;
			if(0 == rel)return 0;

			win = (void*)(rel->srcchip);
			w = win->width;
			h = win->height;
			defaultstyle_2d(sty, w, h, (w+h)/2);

			break;
		}
		case _vbo_:
		{
			defaultstyle_3d(sty, &win->target);
			break;
		}
		default:
		{
			w = win->width;
			h = win->height;
			defaultstyle_2d(sty, w, h, (w+h)/2);
		}
	}

	actorcreate(0, act);
	actorstart(win, sty, act, pin);

	relationcreate(act, pin, _act_, ccc, sty, _win_);
	return 0;
}




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