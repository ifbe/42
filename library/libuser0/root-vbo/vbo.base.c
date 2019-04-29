#include "libuser.h"
void* allocarena();
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);
//
int aid3d_create(void*, void*);
int aid3d_start(void*, void*, void*, void*);
int aid3d_sread(void*, void*, void*, void*);
int aid3d_swrite(void*, void*, void*, void*, void*);
//
int cam3d_create(void*, void*);
int cam3d_start(void*, void*, void*, void*);
int cam3d_sread(void*, void*, void*, void*);
int cam3d_swrite(void*, void*, void*, void*, void*);
//
int lit3d_create(void*, void*);
int lit3d_start(void*, void*, void*, void*);
int lit3d_sread(void*, void*, void*, void*);
int lit3d_swrite(void*, void*, void*, void*, void*);
//
int mic3d_create(void*, void*);
int mic3d_start(void*, void*, void*, void*);
int mic3d_sread(void*, void*, void*, void*);
int mic3d_swrite(void*, void*, void*, void*, void*);
//
int bg3d_create(void*, void*);
int bg3d_start(void*, void*, void*, void*);
int bg3d_sread(void*, void*, void*, void*);
int bg3d_swrite(void*, void*, void*, void*, void*);
//
int fg3d_create(void*, void*);
int fg3d_start(void*, void*, void*, void*);
int fg3d_sread(void*, void*, void*, void*);
int fg3d_swrite(void*, void*, void*, void*, void*);
//
int ui3d_create(void*, void*);
int ui3d_start(void*, void*, void*, void*);
int ui3d_sread(void*, void*, void*, void*);
int ui3d_swrite(void*, void*, void*, void*, void*);
//
int xx3d_create(void*, void*);
int xx3d_start(void*, void*, void*, void*);
int xx3d_sread(void*, void*, void*, void*);
int xx3d_swrite(void*, void*, void*, void*, void*);




void defaultvertex(struct arena* win)
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
	win->camera.vc[1] = -2000.0;
	win->camera.vc[2] = 2000.0;
}




int vbonode_sread(struct arena* win, struct style* stack)
{
	struct style* sty;
	struct pinid* pin;
	struct arena* tmp;
	struct relation* rel;
//say("@vbonode_sread.begin\n");
	preprocess(win);

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			tmp = (void*)(rel->dstchip);
			switch(tmp->fmt)
			{
				case _aid3d_:aid3d_sread(tmp, pin, win, sty);break;
				case _mic3d_:mic3d_sread(tmp, pin, win, sty);break;
				case _cam3d_:cam3d_sread(tmp, pin, win, sty);break;
				case _lit3d_:lit3d_sread(tmp, pin, win, sty);break;
				case _bg3d_:bg3d_sread(tmp, pin, win, sty);break;
				case _fg3d_:fg3d_sread(tmp, pin, win, sty);break;
				case _ui3d_:ui3d_sread(tmp, pin, win, sty);break;
				case _xx3d_:xx3d_sread(tmp, pin, win, sty);break;
			}
		}

		rel = samesrcnextdst(rel);
	}

	postprocess(win);
//say("@vbonode_sread.end\n");
	return 0;
}
int vbonode_swrite(struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct relation* rel;
	struct style* sty;
	struct pinid* pin;
	struct arena* tmp;
	struct actor* act;
	//printmemory(ev, 16);

	ret = 0;
	rel = win->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			act = (void*)(rel->dstchip);
			actor_rootwrite(act, pin, win, sty, ev, 0);
		}
		if(_win_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			tmp = (void*)(rel->dstchip);
			switch(tmp->fmt)
			{
				case _aid3d_:ret = aid3d_swrite(tmp, pin, win, sty, ev);break;
				case _mic3d_:ret = mic3d_swrite(tmp, pin, win, sty, ev);break;
				case _cam3d_:ret = cam3d_swrite(tmp, pin, win, sty, ev);break;
				case _lit3d_:ret = lit3d_swrite(tmp, pin, win, sty, ev);break;
				case _bg3d_:ret = bg3d_swrite(tmp, pin, win, sty, ev);break;
				case _fg3d_:ret = fg3d_swrite(tmp, pin, win, sty, ev);break;
				case _ui3d_:ret = ui3d_swrite(tmp, pin, win, sty, ev);break;
				case _xx3d_:ret = xx3d_swrite(tmp, pin, win, sty, ev);break;
			}
			if(ret)break;
		}

		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}




int vbonode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int vbonode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	if(0 == twig)return 0;
	switch(twig->fmt)
	{
		case _aid3d_:aid3d_start(twig, tf, root, rf);return 1;
		case _mic3d_:mic3d_start(twig, tf, root, rf);return 1;
		case _cam3d_:cam3d_start(twig, tf, root, rf);return 1;
		case _lit3d_:lit3d_start(twig, tf, root, rf);return 1;
		case _bg3d_:bg3d_start(twig, tf, root, rf);return 1;
		case _fg3d_:fg3d_start(twig, tf, root, rf);return 1;
		case _ui3d_:ui3d_start(twig, tf, root, rf);return 1;
		case _xx3d_:xx3d_start(twig, tf, root, rf);return 1;
	}
	return 0;
}




int vbonode_delete(struct arena* win)
{
	return 0;
}
void* vbonode_create(u64 type, u64 flag)
{
	int j;
	u8* buf;
	struct arena* win;
	struct arena* tmp;

	if(_aid3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _aid3d_;
		aid3d_create(win, 0);
		return win;
	}

	if(_mic3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _mic3d_;
		mic3d_create(win, 0);
		return win;
	}

	if(_cam3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _cam3d_;
		cam3d_create(win, 0);
		return win;
	}

	if(_lit3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _lit3d_;
		lit3d_create(win, 0);
		return win;
	}

	if(_bg3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _bg3d_;
		bg3d_create(win, 0);
		return win;
	}

	if(_fg3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _fg3d_;
		fg3d_create(win, 0);
		return win;
	}

	if(_ui3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _ui3d_;
		ui3d_create(win, 0);
		return win;
	}

	if(_xx3d_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _twig_;
		win->fmt = _xx3d_;
		xx3d_create(win, 0);
		return win;
	}

	if(_vbo_ == type)
	{
		win = allocarena();
		if(0 == win)return 0;

		win->type = _root_;
		win->fmt = _vbo_;
		win->width = win->fbwidth = 1024;
		win->height = win->fbheight = 1024;
		win->depth = win->fbdepth = 1024;
		defaultvertex(win);

		win->gl_solid = buf = memorycreate(0x10000);
		for(j=0;j<0x10000;j++)buf[j] = 0;

		win->gl_opaque = buf = memorycreate(0x10000);
		for(j=0;j<0x10000;j++)buf[j] = 0;

		if(_ui_ == flag)
		{
			//aid3d
			tmp = vbonode_create(_aid3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//ui3d
			tmp = vbonode_create(_ui3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//xx3d
			tmp = vbonode_create(_xx3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}
		}

		if(_3d_ == flag)
		{
/*
			//mic3d
			tmp = vbonode_create(_mic3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//cam3d
			tmp = vbonode_create(_cam3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//lit3d
			tmp = vbonode_create(_lit3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}
*/
			//bg3d
			tmp = vbonode_create(_bg3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//fg3d
			tmp = vbonode_create(_fg3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}
		}

		return win;
	}

	return 0;
}
