#include "libuser.h"
void* allocarena();
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);
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
//
int xx2d_create(void*, void*);
int xx2d_start(void*, void*, void*, void*);
int xx2d_sread(void*, void*, void*, void*);
int xx2d_swrite(void*, void*, void*, void*, void*);




int rgbanode_sread(struct arena* win, struct style* stack)
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
				case _bg2d_:bg2d_sread(tmp, pin, win, sty);break;
				case _fg2d_:fg2d_sread(tmp, pin, win, sty);break;
				case _ui2d_:ui2d_sread(tmp, pin, win, sty);break;
				case _xx2d_:xx2d_sread(tmp, pin, win, sty);break;
			}
		}

		rel = samesrcnextdst(rel);
	}

	postprocess(win);
	return 0;
}
int rgbanode_swrite(struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct style* sty;
	struct pinid* pin;
	struct arena* tmp;
	struct relation* rel;
	//printmemory(ev, 16);

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
				case _bg2d_:ret = bg2d_swrite(tmp, pin, win, sty, ev);break;
				case _fg2d_:ret = fg2d_swrite(tmp, pin, win, sty, ev);break;
				case _ui2d_:ret = ui2d_swrite(tmp, pin, win, sty, ev);break;
				case _xx2d_:ret = xx2d_swrite(tmp, pin, win, sty, ev);break;
			}
			if(ret)break;
		}

		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}




int rgbanode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int rgbanode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	if(0 == twig)return 0;
	switch(twig->fmt)
	{
		case _bg2d_:bg2d_start(twig, tf, root, rf);return 1;
		case _fg2d_:fg2d_start(twig, tf, root, rf);return 1;
		case _ui2d_:ui2d_start(twig, tf, root, rf);return 1;
		case _xx2d_:xx2d_start(twig, tf, root, rf);return 1;
	}
	return 0;
}




int rgbanode_delete(struct arena* win)
{
	return 0;
}
void* rgbanode_create(u64 type, void* addr)
{
	struct arena* win;
	struct arena* tmp;

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

	if(_xx2d_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _xx2d_;
			xx2d_create(win, 0);
		}
		return win;
	}

	if(_rgba_ == type)
	{
		win = addr;
		if(win)
		{
			//bg2d
			tmp = rgbanode_create(_bg2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, 0, win, 0, _win_, 0);
				rgbanode_start(tmp, 0, win, 0);
			}

			//fg2d
			tmp = rgbanode_create(_fg2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, 0, win, 0, _win_, 0);
				rgbanode_start(tmp, 0, win, 0);
			}

			//ui2d
			tmp = rgbanode_create(_ui2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, 0, win, 0, _win_, 0);
				rgbanode_start(tmp, 0, win, 0);
			}

			//xx2d
			tmp = rgbanode_create(_xx2d_, 0);
			if(tmp)
			{
				relationcreate(tmp, 0, _win_, 0, win, 0, _win_, 0);
				rgbanode_start(tmp, 0, win, 0);
			}
		}
		return win;
	}

	return 0;
}
