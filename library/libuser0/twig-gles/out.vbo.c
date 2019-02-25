#include "libuser.h"
int preprocess(void*);
int postprocess(void*);
void* allocarena();
int windowcreate(void*);
void arenavertex(struct arena* win);
//gl
int bg3d_create(void*, void*);
int bg3d_start(void*, void*, void*, void*);
int bg3d_read(void*, void*, void*, void*);
int fg3d_create(void*, void*);
int fg3d_start(void*, void*, void*, void*);
int fg3d_read(void*, void*, void*, void*);
int bg2d_create(void*, void*);
int bg2d_start(void*, void*, void*, void*);
int bg2d_read(void*, void*, void*, void*);
int fg2d_create(void*, void*);
int fg2d_start(void*, void*, void*, void*);
int fg2d_read(void*, void*, void*, void*);
int menu_create(void*, void*);
int menu_start(void*, void*, void*, void*);
int menu_read(void*, void*, void*, void*);
int vkbd_create(void*, void*);
int vkbd_start(void*, void*, void*, void*);
int vkbd_read(void*, void*, void*, void*);




int vbonode_sread(struct arena* win)
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
				case _bg2d_:bg2d_read(tmp, pin, win, sty);break;
				case _fg2d_:fg2d_read(tmp, pin, win, sty);break;
				case _menu_:menu_read(tmp, pin, win, sty);break;
				case _vkbd_:vkbd_read(tmp, pin, win, sty);break;
			}
		}

		rel = samesrcnextdst(rel);
	}

	postprocess(win);
	return 0;
}
int vbonode_swrite(struct arena* win)
{
	return 0;
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
		case _bg2d_:bg2d_start(c, 0, r, 0);return 1;
		case _fg2d_:fg2d_start(c, 0, r, 0);return 1;
		case _menu_:menu_start(c, 0, r, 0);return 1;
		case _vkbd_:vkbd_start(c, 0, r, 0);return 1;
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

	if(_menu_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _menu_;
			menu_create(win, 0);
		}
		return win;
	}

	if(_vkbd_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _vkbd_;
			vkbd_create(win, 0);
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

			//menu
			tmp = vbonode_create(_menu_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, win, 0, _win_);
				vbonode_start(tmp, 0, win, 0);
			}

			//vkbd
			tmp = vbonode_create(_vkbd_, 0);
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