#include "libuser.h"
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
void lib4d_create(void*, void*);
void lib4d_delete();
//pre
int preprocess(struct arena* win);
//back
int bg3d_read( void*, void*, struct arena* win, struct style* sty);
int bg3d_write(struct arena* win, struct style* sty, struct event* ev);
//fore
int fg3d_read( void*, void*, struct arena* win, struct style* sty);
int fg3d_write(struct arena* win, struct style* sty, struct event* ev);
//temp
int menu_read( void*, void*, struct arena* win, struct style* sty);
int menu_write(struct arena* win, struct style* sty, struct event* ev);
//vkbd
int vkbd_read( void*, void*, struct arena* win, struct style* sty);
int vkbd_write(struct arena* win, struct style* sty, struct event* ev);
//post
int postprocess(struct arena* win);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
static int actlen = 0;
static int pinlen = 0;
void* allocactor()
{
	int j,max;
	struct actor* act;

	max = 0x100000 / sizeof(struct actor);
	for(j=0;j<max;j++)
	{
		act = &actor[j];
		if(0 == act->type)return act;
	}
	return 0;
}
void* allocpinid()
{
	int j,len;
	u8* buf;

	len = 0x100;	//sizeof(struct pinid);
	buf = (void*)pinid + pinlen;
	pinlen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}




int actorinput_special(struct arena* win, struct style* sty, struct event* ev)
{
	int val;
	short* t;

	val = 0;
	if(_char_ == ev->what)
	{
		if(0x1b == ev->why)val = 'r';
	}
	else if(_kbd_ == ev->what)
	{
		if(0xfb == ev->why)val = 'l';
		else if(0xfc == ev->why)val = 'r';
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyl_select)val = 'l';
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyr_start)val = 'r';
	}

	if(('l' == val)|('r' == val))
	{
		//win->forew = 0x80;
		return 1;
	}
	return 0;
}
void actorinput_touch(struct arena* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->input[btn].xn = x;
		win->input[btn].yn = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->input[btn].x0 = win->input[btn].xn = x;
		win->input[btn].y0 = win->input[btn].yn = y;
		win->input[btn].z0 = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->input[btn].z0 = 0;
	}
}




int actor_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct actor* act = dc;
	return act->onswrite(dc, df, sc, sf, buf, len);
}
int actor_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct actor* act = dc;
	return act->onsread(dc, df, sc, sf, buf, len);
}
int actor_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct actor* act = dc;
	return act->oncwrite(dc, df, sc, sf, buf, len);
}
int actor_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct actor* act = dc;
	return act->oncread(dc, df, sc, sf, buf, len);
}
int actorstop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	if(0 == leaf)return 0;
	leaf->onstop(leaf, lf, twig, tf, root, rf);
	return 0;
}
int actorstart(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	if(0 == leaf)return 0;
	leaf->onstart(leaf, lf, twig, tf, root, rf);
	return 0;
}
int actordelete(struct actor* act)
{
	if(0 == act)return 0;
	if(_orig_ == act->type)return 0;
	if(_copy_ == act->type)return 0;
	act->ondelete(act);

	if(_ORIG_ == act->type)act->type = _orig_;
	else if(_COPY_ == act->type)act->type = _copy_;
	return 0;
}
void* actorcreate(u64 type, void* buf)
{
	int j,k;
	u8* src;
	u8* dst;
	struct actor* act;

	if(0 == type)
	{
		act = buf;
		if(0 == act)return 0;
		if(_ORIG_ == act->type)return 0;
		if(_COPY_ == act->type)return 0;

		act->oncreate(act, 0);
	}
	else
	{
		k = 0;
		for(j=0;j<256;j++)
		{
			if(0 == actor[j].type)
			{
				if(0 == k)return 0;
				break;
			}
			if(type == actor[j].fmt)
			{
				k = j;
				if(_orig_ == actor[j].type)
				{
					act = &actor[j];
					break;
				}
			}
		}

		if(j != k)
		{
			src = (void*)&actor[k];
			dst = (void*)&actor[j];
			for(j=0;j<sizeof(struct actor);j++)dst[j] = src[j];

			act = (void*)dst;
			act->irel0 = act->ireln = 0;
			act->orel0 = act->oreln = 0;

			act->fd = 0;
			act->dc = 0;
			act->idx = 0;
			act->buf = 0;

			//act->tier
			act->type = _copy_;
			//act->fmt
		}

		act->oncreate(act, buf);
	}

	if(_orig_ == act->type)act->type = _ORIG_;
	else if(_copy_ == act->type)act->type = _COPY_;
	return act;
}




int actorevent(struct event* ev)
{/*
	int ret;
	struct arena* win = (void*)(ev->where);

	if(_drag_ == ev->what)
	{
		say("%s\n", (void*)(ev->why));
		return 0;
	}

	//vkbd
	ret = vkbd_write(win, 0, ev);
	if(ret)goto theend;

	//special
	ret = actorinput_special(win, 0, ev);
	if(ret)goto theend;

	//temp
	ret = menu_write(win, 0, ev);
	if(ret)goto theend;

	//fore
	ret = fg3d_write(win, 0, ev);
	if(ret)goto theend;

	//back
	ret = bg3d_write(win, 0, ev);
	if(ret)goto theend;

theend:
	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
*/	return 0;
}
void* actorcommand(int argc, char** argv)
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
		actorcreate(name, argv[3]);
	}

	return 0;
}




int actorread_all(struct arena* win)
{/*
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	if(_cli_ == win->fmt)return 0;

	//bg
	preprocess(win);

	if(	(_json_ == win->fmt) |
		(_html_ == win->fmt) )
	{
		sty = 0;
		orel = win->orel0;
		while(1)
		{
			if(orel == 0)break;

			if(_act_ == orel->dsttype)
			{
				act = (void*)(orel->dstchip);
				pin = (void*)(orel->dstfoot);
				sty = (void*)(orel->srcfoot);
				act->onread(win, sty, act, pin);
			}

			orel = samesrcnextdst(orel);
		}
	}
	else
	{
		//foreground
		fg3d_read(0, 0, win, 0);
	}

	//fg
	postprocess(win);
*/
	return 0;
}
void* actorlist(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct actor* act;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			act = &actor[j];
			if(0 == act->fmt)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->fmt, &act->fmt);
		}
		if(0 == j)say("empty actor\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == actor[j].fmt)break;
			p = (void*)(&actor[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &actor[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freeactor()
{
	//say("[c,f):deleteing actor\n");

	lib4d_delete();
	lib3d_delete();
	lib2d_delete();
	lib1d_delete();

	pinid = 0;
	style = 0;
	actor = 0;
	arena = 0;
}
void initactor(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	pinid = (void*)(addr+0x300000);

#define max (0x100000/sizeof(struct actor))
	for(j=0;j<max;j++)actor[j].tier = _act_;

	lib1d_create(addr, 0);
	lib2d_create(addr, 0);
	lib3d_create(addr, 0);
	lib4d_create(addr, 0);

	//say("[c,f):createed actor\n");
}
