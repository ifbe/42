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




void initobject(struct arena* win)
{
	int j;
	u8* buf;
	struct datapair* mod;
	struct glsrc* src;

	buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->mod = mod = (void*)buf;


//--------------------font3d-------------------
	//[0000,3fff]
	src = &mod[font3d0].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[4000,7fff]
	src = &mod[font3d1].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[8000,bfff]
	src = &mod[font3d2].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[c000,ffff]
	src = &mod[font3d3].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//--------------------font2d-------------------
	//[0000,3fff]
	src = &mod[font2d0].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[4000,7fff]
	src = &mod[font2d1].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[8000,bfff]
	src = &mod[font2d2].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[c000,ffff]
	src = &mod[font2d3].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//--------------------3d-------------------
	//drawarray.point3d
	src = &mod[point3d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;


	//drawelement.line3d
	src = &mod[line3d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//drawelement.trigon3d
	src = &mod[trigon3d].src;

	src->vbuf = memorycreate(0x1000000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x100000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//opaque3d
	src = &mod[opaque3d].src;

	src->vbuf = memorycreate(0x1000000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x100000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//----------------------2d--------------------
	//drawarray.point2d
	src = &mod[point2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;


	//drawelement.line2d
	src = &mod[line2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//drawelement.trigon2d
	src = &mod[trigon2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//opaque2d
	src = &mod[opaque2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
}




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
