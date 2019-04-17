#include "libuser.h"
void* allocarena();
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);
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
int ui3d_create(void*, void*);
int ui3d_start(void*, void*, void*, void*);
int ui3d_sread(void*, void*, void*, void*);
int ui3d_swrite(void*, void*, void*, void*, void*);
//
int bg2d_create(void*, void*);
int bg2d_start(void*, void*, void*, void*);
int bg2d_sread(void*, void*, void*, void*);
int bg2d_swrite(void*, void*, void*, void*, void*);
//
int fg2d_create(void*, void*);
int fg2d_start(void*, void*, void*, void*);
int fg2d_sread(void*, void*, void*, void*);
int fg2d_swrite(void*, void*, void*, void*, void*);
//
int ui2d_create(void*, void*);
int ui2d_start(void*, void*, void*, void*);
int ui2d_sread(void*, void*, void*, void*);
int ui2d_swrite(void*, void*, void*, void*, void*);




int vbonode_sread(struct arena* win, struct style* stack)
{
	struct style* sty;
	struct pinid* pin;
	struct arena* tmp;
	struct relation* rel;
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
				case _bg3d_:bg3d_sread(tmp, pin, win, sty);break;
				case _fg3d_:fg3d_sread(tmp, pin, win, sty);break;
				case _cam3d_:cam3d_sread(tmp, pin, win, sty);break;
				case _lit3d_:lit3d_sread(tmp, pin, win, sty);break;
				case _ui3d_:ui3d_sread(tmp, pin, win, sty);break;
				case _bg2d_:bg2d_sread(tmp, pin, win, sty);break;
				case _fg2d_:fg2d_sread(tmp, pin, win, sty);break;
				case _ui2d_:ui2d_sread(tmp, pin, win, sty);break;
			}
		}

		rel = samesrcnextdst(rel);
	}

	postprocess(win);
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
				case _bg3d_:ret = bg3d_swrite(tmp, pin, win, sty, ev);break;
				case _fg3d_:ret = fg3d_swrite(tmp, pin, win, sty, ev);break;
				case _cam3d_:ret = cam3d_swrite(tmp, pin, win, sty, ev);break;
				case _lit3d_:ret = lit3d_swrite(tmp, pin, win, sty, ev);break;
				case _ui3d_:ret = ui3d_swrite(tmp, pin, win, sty, ev);break;
				case _bg2d_:ret = bg2d_swrite(tmp, pin, win, sty, ev);break;
				case _fg2d_:ret = fg2d_swrite(tmp, pin, win, sty, ev);break;
				case _ui2d_:ret = ui2d_swrite(tmp, pin, win, sty, ev);break;
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
		case _bg3d_:bg3d_start(twig, tf, root, rf);return 1;
		case _fg3d_:fg3d_start(twig, tf, root, rf);return 1;
		case _cam3d_:cam3d_start(twig, tf, root, rf);return 1;
		case _lit3d_:lit3d_start(twig, tf, root, rf);return 1;
		case _ui3d_:ui3d_start(twig, tf, root, rf);return 1;
		case _bg2d_:bg2d_start(twig, tf, root, rf);return 1;
		case _fg2d_:fg2d_start(twig, tf, root, rf);return 1;
		case _ui2d_:ui2d_start(twig, tf, root, rf);return 1;
	}
	return 0;
}




int vbonode_delete(struct arena* win)
{
	return 0;
}
void* vbonode_create(u64 type, void* addr)
{
	struct arena* win;
	struct arena* tmp;

	if(_bg3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _bg3d_;
			bg3d_create(win, 0);
		}
		return win;
	}

	if(_fg3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _fg3d_;
			fg3d_create(win, 0);
		}
		return win;
	}

	if(_cam3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _cam3d_;
			cam3d_create(win, 0);
		}
		return win;
	}

	if(_lit3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _lit3d_;
			lit3d_create(win, 0);
		}
		return win;
	}

	if(_ui3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _ui3d_;
			ui3d_create(win, 0);
		}
		return win;
	}

	if(_bg2d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _bg2d_;
			bg2d_create(win, 0);
		}
		return win;
	}

	if(_fg2d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _fg2d_;
			fg2d_create(win, 0);
		}
		return win;
	}

	if(_ui2d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _ui2d_;
			ui2d_create(win, 0);
		}
		return win;
	}

	if(_vbo_ == type)
	{
		win = addr;
		if(win)
		{
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

			//ui3d
			tmp = vbonode_create(_ui3d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//bg2d
			tmp = vbonode_create(_bg2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//fg2d
			tmp = vbonode_create(_fg2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//ui2d
			tmp = vbonode_create(_ui2d_, 0);
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
