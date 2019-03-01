#include "libuser.h"
int preprocess(void*);
int postprocess(void*);
void* allocarena();
int actorinput_touch(struct arena* win, struct event* ev);
//
int bg3d_create(void*, void*);
int bg3d_start(void*, void*, void*, void*);
int bg3d_read(void*, void*, void*, void*);
int bg3d_write(void*, void*, void*, void*, void*);
//
int fg3d_create(void*, void*);
int fg3d_start(void*, void*, void*, void*);
int fg3d_read(void*, void*, void*, void*);
int fg3d_write(void*, void*, void*, void*, void*);
//
int ev3d_create(void*, void*);
int ev3d_start(void*, void*, void*, void*);
int ev3d_read(void*, void*, void*, void*);
int ev3d_write(void*, void*, void*, void*, void*);
//
int ui3d_create(void*, void*);
int ui3d_start(void*, void*, void*, void*);
int ui3d_read(void*, void*, void*, void*);
int ui3d_write(void*, void*, void*, void*, void*);
//
int bg2d_create(void*, void*);
int bg2d_start(void*, void*, void*, void*);
int bg2d_read(void*, void*, void*, void*);
int bg2d_write(void*, void*, void*, void*, void*);
//
int fg2d_create(void*, void*);
int fg2d_start(void*, void*, void*, void*);
int fg2d_read(void*, void*, void*, void*);
int fg2d_write(void*, void*, void*, void*, void*);
//
int ev2d_create(void*, void*);
int ev2d_start(void*, void*, void*, void*);
int ev2d_read(void*, void*, void*, void*);
int ev2d_write(void*, void*, void*, void*, void*);
//
int ui2d_create(void*, void*);
int ui2d_start(void*, void*, void*, void*);
int ui2d_read(void*, void*, void*, void*);
int ui2d_write(void*, void*, void*, void*, void*);




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
				case _bg3d_:bg3d_read(tmp, pin, win, sty);break;
				case _fg3d_:fg3d_read(tmp, pin, win, sty);break;
				case _ev3d_:ev3d_read(tmp, pin, win, sty);break;
				case _ui3d_:ui3d_read(tmp, pin, win, sty);break;
				case _bg2d_:bg2d_read(tmp, pin, win, sty);break;
				case _fg2d_:fg2d_read(tmp, pin, win, sty);break;
				case _ev2d_:ev2d_read(tmp, pin, win, sty);break;
				case _ui2d_:ui2d_read(tmp, pin, win, sty);break;
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
	struct style* sty;
	struct pinid* pin;
	struct arena* tmp;
	struct relation* rel;

	ret = 0;
	rel = win->oreln;
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
				case _bg3d_:ret = bg3d_write(tmp, pin, win, sty, ev);break;
				case _fg3d_:ret = fg3d_write(tmp, pin, win, sty, ev);break;
				case _ev3d_:ret = ev3d_write(tmp, pin, win, sty, ev);break;
				case _ui3d_:ret = ui3d_write(tmp, pin, win, sty, ev);break;
				case _bg2d_:ret = bg2d_write(tmp, pin, win, sty, ev);break;
				case _fg2d_:ret = fg2d_write(tmp, pin, win, sty, ev);break;
				case _ev2d_:ret = ev2d_write(tmp, pin, win, sty, ev);break;
				case _ui2d_:ret = ui2d_write(tmp, pin, win, sty, ev);break;
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
int vbonode_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	if(0 == c)return 0;
	switch(c->fmt)
	{
		case _bg3d_:bg3d_start(c, 0, r, 0);return 1;
		case _fg3d_:fg3d_start(c, 0, r, 0);return 1;
		case _ev3d_:ev3d_start(c, 0, r, 0);return 1;
		case _ui3d_:ui3d_start(c, 0, r, 0);return 1;
		case _bg2d_:bg2d_start(c, 0, r, 0);return 1;
		case _fg2d_:fg2d_start(c, 0, r, 0);return 1;
		case _ev2d_:ev2d_start(c, 0, r, 0);return 1;
		case _ui2d_:ui2d_start(c, 0, r, 0);return 1;
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

	if(_light_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _light_;
		}
		return win;
	}

	if(_mirror_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _mirror_;
		}
		return win;
	}

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

	if(_ev3d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _ev3d_;
			ev3d_create(win, 0);
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

	if(_ev2d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _ev2d_;
			ev2d_create(win, 0);
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

			//ev3d
			tmp = vbonode_create(_ev3d_, 0);
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

			//ev2d
			tmp = vbonode_create(_ev2d_, 0);
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