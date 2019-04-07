#include "libuser.h"
#define _bdc_  hex32('b','d','c',0)
#define _step_ hex32('s','t','e','p')
#define _pin_  hex32('p','i','n',0)
#define _chip_ hex32('c','h','i','p')




void initjoy(void*);
void freejoy();
int joycreate(void*, void*);
int joydelete(void*);
//
void initstd(void*);
void freestd();
int stdcreate(void*, void*);
int stddelete(void*);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*);
int traydelete(void*);
//cam
void initcam(void*);
void freecam();
int videocreate(void*);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void* win, void* sty, void* act, void* pin);
int videowrite(void*);
int videolist();
int videochoose();
//mic
void initmic(void*);
void freemic();
int soundcreate(void*);
int sounddelete(void*);
int soundstart(void*);
int soundstop(void*);
int soundread(void* win, void* sty, void* act, void* pin);
int soundwrite(void*);
int soundlist();
int soundchoose();
//
int toycar_create(void*, void*);
int toycar_delete(void*);
int toycar_rootread(void*,void*,void*,void*,void*,int);
int toycar_rootwrite(void*,void*,void*,void*,void*,int);
//
int stepcar_create(void*, void*);
int stepcar_delete(void*, void*);
int stepcar_rootread(void*,void*,void*,void*,void*,int);
int stepcar_rootwrite(void*,void*,void*,void*,void*,int);
//window
void initwindow(void*);
void freewindow();
int windowcreate(void*);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void*);
int windowwrite(void*);
int windowlist();
int windowchoose();
//
void* vbonode_create(u64, void*);
int vbonode_delete(void*);
int vbonode_start(void*, void*, void*, void*);
int vbonode_stop(void*, void*);
int vbonode_sread(struct arena* win, struct style* sty);
int vbonode_swrite(struct arena* win, struct style* sty, struct event* ev);
//
void* rgbanode_create(u64, void*);
int rgbanode_delete(void*);
int rgbanode_start(void*, void*, void*, void*);
int rgbanode_stop(void*, void*);
int rgbanode_sread(struct arena* win, struct style* sty);
int rgbanode_swrite(struct arena* win, struct style* sty, struct event* ev);
//
int schnode_create(u64, void*);
int schnode_delete(void*);
int schnode_start(void*, void*, void*, void*);
int schnode_stop(void*, void*);
//
int pcbnode_create(void*, void*);
int pcbnode_delete(void*);
//
int htmlnode_create(void*, void*);
int htmlnode_delete(void*);
int htmlnode_rootread(void*,void*,void*,void*,void*,int);
int htmlnode_rootwrite(void*,void*,void*,void*,void*,int);
int jsonnode_create(void*, void*);
int jsonnode_delete(void*);
int jsonnode_rootread(void*,void*,void*,void*,void*,int);
int jsonnode_rootwrite(void*,void*,void*,void*,void*,int);
//
int xmlnode_create(void*, void*);
int xmlnode_delete(void*);
//
int funcnode_create(void*, void*);
int funcnode_delete(void*);
//
int actorevent(struct event* ev);
int input(void*, int);
//
int cmp(void*, void*);
int ncmp(void*, void*, int);
int parsexml_detail(void*, int, void*, void*, void*, void*);




//
static struct arena* arena = 0;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
void* allocarena()
{
	int j;
	struct arena* win;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
	}
	if(j >= 0x100)return 0;

	win = &arena[j];
	win->irel0 = win->ireln = 0;
	win->orel0 = win->oreln = 0;
	return win;
}
void* allocstyle()
{
	int j,len;
	u8* buf;

	len = 0x100;	//sizeof(struct style);
	buf = (void*)style + stylen;
	stylen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}
void arenavertex(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] =-0.70710678118655;
	win->camera.vb[2] =-0.70710678118655;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 0.70710678118655;
	win->camera.vu[2] = 0.70710678118655;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.70710678118655;
	win->camera.vn[2] =-0.70710678118655;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = -1000.0;
	win->camera.vc[2] = 1000.0;
}




int arena_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct arena* win = dc;
	if(0 == win)return 0;

//say("arenawrite@{\n");
	switch(win->fmt)
	{
		case _html_:htmlnode_rootwrite(dc, df, sc, sf, buf, len);break;
		case _json_:jsonnode_rootwrite(dc, df, sc, sf, buf, len);break;
		case _bdc_ :  toycar_rootwrite(dc, df, sc, sf, buf, len);break;
		case _step_: stepcar_rootwrite(dc, df, sc, sf, buf, len);break;
		default: printmemory(buf, len);
	}
//say("}@arenawrite\n");
	return 0;
}
int arena_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct relation* rel;
	struct arena* win = dc;
	if(0 == win)return 0;

	switch(win->fmt)
	{
		case _html_:htmlnode_rootread(dc, df, sc, sf, buf, len);break;
		case _json_:jsonnode_rootread(dc, df, sc, sf, buf, len);break;
		case  _vbo_:vbonode_sread(win, 0);break;
		default:rgbanode_sread(win, 0);break;
	}
	return 0;
}
int arena_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int arena_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	int j;
	struct arena* win;
/*
	//0 == stack[0]: read all
	for(j=0;j<0x100;j++)
	{
		win = &arena[j];
		if(0 == win->type)continue;

		windowread(win);
	}
	return 0;
*/
/*
	//0 != stack[0]: read stack[0].ctx to stack[-1].buf
	win = stack[0];
	return windowread();
*/
	return 0;
}
int arenastop()
{
	return 0;
}
int arenastart(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	int ret;
	if(0 == twig)return 0;

	//try twig-gles
	ret = vbonode_start(twig, tf, root, rf);
	if(ret)return 1;

	//try twig-sch
	ret = schnode_start(twig, tf, root, rf);
	if(ret)return 1;

	//try twig-?
	return 0;
}
int arenadelete(struct arena* win)
{
	if(win == 0)return 0;

	//1.close
	windowstop(win);

	//2.unlink
	win->irel0 = 0;
	win->ireln = 0;
	win->orel0 = 0;
	win->oreln = 0;

	//3.cleanup
	win->type = 0;
	win->fmt = 0;
	return 0;
}
void* arenacreate(u64 type, void* addr)
{
	int j = 0;
	struct arena* win;
	struct arena* sub;

	//default
	if(0 == type)
	{
		if(arena[0].type)return 0;
		type = _win_;
	}

	//0: system object
	if(_joy_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = _joy_;
			joycreate(win, addr);
		}
		return win;
	}
	else if(_std_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = _std_;
			stdcreate(win, addr);
		}
		return win;
	}
	else if(_tray_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = _tray_;
			traycreate(win, addr);
		}
		return win;
	}
	else if(_cam_ == type)
	{
		if(0 == addr)return 0;
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = hex32('y','u','v',0);
			videocreate(win);
		}
		return win;
	}
	else if(_mic_ == type)
	{
		if(0 == addr)return 0;
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = hex32('p','c','m',0);
			soundcreate(win);
		}
		return win;
	}
	else if(_bdc_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = _bdc_;
			toycar_create(win, 0);
		}
		return win;
	}
	else if(_step_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _perm_;
			win->fmt = _step_;
			stepcar_create(win, 0);
		}
		return win;
	}

	//1: gles
	else if(_win_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _win_;
			win->fmt = hex64('b','g','r','a','8','8','8','8');
			windowcreate(win);

			arenavertex(win);
			if(_vbo_ == win->fmt)vbonode_create(_vbo_, win);
			else rgbanode_create(_rgba_, win);
		}

		return win;
	}
	else if(_fbo_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _win_;
			win->fmt = _fbo_;
			windowcreate(win);
			arenavertex(win);
		}
		return win;
	}

	//pcbdoc
	else if(_sch_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _sch_;
			schnode_create(_sch_, win);
		}
		return win;
	}
	else if(_pcb_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _pcb_;
			pcbnode_create(win, addr);
		}
		return win;
	}

	//else
	else if(_func_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _func_;
			funcnode_create(win, addr);
		}
		return win;
	}
	else if(_html_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _html_;
			htmlnode_create(win, addr);
		}
		return win;
	}
	else if(_json_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _json_;
			jsonnode_create(win, addr);
		}
		return win;
	}
	else if(_xml_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _node_;
			win->fmt = _xml_;
			xmlnode_create(win, addr);
		}
		return win;
	}

	return 0;
}




int arenaevent(struct event* e)
{
	int j;
	struct event ev;
	struct arena* win;

	ev.why = e->why;
	ev.what = e->what;
	ev.where = e->where;
	ev.when = e->when;

	if(0 == ev.where)
	{
		//from cmd
		if(_char_ == ev.what)
		{
			input(&ev.why, 0);
			return 0;
		}

		//maybe gamepad
		for(j=0;j<16;j++)
		{
			win = &arena[j];
			if(_win_ == win->type)
			{
				ev.where = (u64)win;
				break;
			}
		}
	}

	win = (void*)(ev.where);
	if(0 == win)return 0;

	if(_vbo_ == win->fmt)vbonode_swrite(win, 0, &ev);
	else rgbanode_swrite(win, 0, &ev);
	return 0;
}
void* arenacommand(int argc, char** argv)
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
		arenacreate(name, argv[3]);
	}

	return 0;
}




int arenaread_all()
{
	int j;
	struct arena* win;

	for(j=31;j>=0;j--)
	{
		win = &arena[j];
		if(0 == win->type)continue;
		if(_win_ == win->type)windowread(win);
	}
	return 0;
}
void* arenalist(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct arena* win;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			win = &arena[j];
			if(0 == win->type)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&win->tier, &win->type, &win->fmt, &win->vfmt);
		}
		if(0 == j)say("empty arena\n");
	}
	else
	{
/*
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
*/
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].fmt)break;
			p = (void*)(&arena[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &arena[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freearena()
{
	//say("[c,f):freeing arena\n");

	freewindow();
	freetray();
	freestd();
	freejoy();
}
void initarena(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x200000);

#define max (0x100000/sizeof(struct arena))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)arena[j].tier = _win_;

	initjoy(arena);
	initstd(arena);
	inittray(arena);
	initwindow(arena);

	arenacreate(_std_, 0);
	arenacreate(_tray_, 0);
	//say("[c,f):inited arena\n");
}
