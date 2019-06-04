#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void forcedirected_2d(void*, int, void*, int, void*, int);
void forcedirected_3d(void*, int, void*, int, void*, int);




struct node
{
	u64 type;
	void* addr;
	u8 padd[8-sizeof(void*)];
};
struct pair
{
	u16 parent;
	u16 child;
};




static void graph_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{/*
	struct arena* aa;
	struct actor* bb;
	void* p;
	int i,j,k;
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

	forcedirected_2d(obuf, vlen, vbuf, vlen, pair, plen);
	vbuf[0][0] = vbuf[0][1] = vbuf[0][2] = 0.0;

	drawsolid_rect(win, 0, cx-ww/2, cy-hh/2, cx+ww/2, cy+hh/2);
	for(i=0;i<plen;i++)
	{
		j = pair[i].parent;
		k = pair[i].child;
		drawline(
			win, 0xffffff,
			cx + ww*(vbuf[j][0]),
			cy - hh*(vbuf[j][1]),
			cx + ww*(vbuf[k][0]),
			cy - hh*(vbuf[k][1])
		);
	}
	for(i=0;i<vlen;i++)
	{
		j = cx + ww*(vbuf[i][0]);
		k = cy - hh*(vbuf[i][1]);

		p = "?";
		if(i < olen)
		{
			if(orig[i].type == _win_)
			{
				aa = orig[i].addr;
				p = &(aa->fmt);
			}
			else if(orig[i].type == _act_)
			{
				bb = orig[i].addr;
				p = &(bb->name);
			}
		}
		drawicon_1(
			win, 0xffffff,
			j-16, k-12,
			j+16, k+12,
			p, 8
		);
	}*/
}
static void graph_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	float n;
	u8 buf[0x1000];
	vec3 tc, tr, tf, tu;
	int rgb;
	int i,j,k;
	struct arena* tmp;
	void* str;
	struct node* node = act->nbuf;
	struct pair* wire = act->wbuf;
	float* vbuf = act->vbuf;
	float* ibuf = act->ibuf;
	//carveline_prism4(win, 0x00ff00, sty->vc, sty->vr, sty->vf, sty->vu);

	if(vbuf)
	{
		for(j=0;j<100;j++)
		{
			forcedirected_3d(buf, act->vlen, act->vbuf, act->vlen, act->wbuf, act->wlen);
			vbuf[0] = sty->f.vc[0];
			vbuf[1] = sty->f.vc[1];
			vbuf[2] = sty->f.vc[2];
		}
/*
		for(j=0;j<act->vlen;j++)
		{
			say("%f,%f,%f\n", vbuf[j*3 + 0], vbuf[j*3 + 1], vbuf[j*3 + 2]);
		}
		say("%d,%d,%d,%d\n",act->nlen, act->wlen, act->vlen, act->ilen);
*/
	}


	for(i=0;i<act->wlen;i++)
	{
		j = wire[i].parent;
		k = wire[i].child;

		carveline_arrow(win, 0xffffff, &vbuf[j*3], &vbuf[k*3]);
	}

	tr[0] = win->camera.vr[0];
	tr[1] = win->camera.vr[1];
	tr[2] = win->camera.vr[2];
	n = 10.0 / squareroot(tr[0]*tr[0] + tr[1]*tr[1] + tr[2]*tr[2]);
	tr[0] *= n;
	tr[1] *= n;
	tr[2] *= n;

	tf[0] = win->camera.vt[0];
	tf[1] = win->camera.vt[1];
	tf[2] = win->camera.vt[2];
	n = 10.0 / squareroot(tf[0]*tf[0] + tf[1]*tf[1] + tf[2]*tf[2]);
	tf[0] *= n;
	tf[1] *= n;
	tf[2] *= n;

	//billboard
	for(j=0;j<act->vlen;j++)
	{
		tmp = node[j].addr;
		switch(tmp->tier)
		{
			case  _fd_:rgb = 0x0000ff;str = (void*)&tmp->type;break;
			case _art_:rgb = 0xff0000;str = (void*)&tmp->type;break;
			case _win_:rgb = 0xffff00;str = (void*)&tmp->fmt;break;
			case _act_:rgb = 0x00ffff;str = (void*)&tmp->fmt;break;
			default:rgb = 0xff00ff;str = (void*)"????????";
		}
		carvestring_center(win, rgb, &vbuf[j*3], tr, tf, str, 8);
	}
}
static void graph_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void graph_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"graph\" style=\"width:50%%;height:100px;float:left;background-color:#c8a7d9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void graph_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void graph_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void graph_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)graph_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)graph_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)graph_draw_html(act, pin, win, sty);
	else if(fmt == _json_)graph_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)graph_draw_vbo(act, pin, win, sty);
	else graph_draw_pixel(act, pin, win, sty);
}




int graph_addnode(struct actor* act, u64 type, void* addr)
{
	int j;
	struct node* node = act->nbuf;

	for(j=0;j<act->nlen;j++)
	{
		if(	(node[j].type == type) &&
			(node[j].addr == addr) )
		{
			return j;
		}
	}

	j = act->nlen;
	act->nlen += 1;

	node[j].type = type;
	node[j].addr = addr;
	return j;
}
int graph_addpair(struct actor* act, int parent, int child)
{
	int j;
	struct pair* wire = act->wbuf;

	for(j=0;j<act->wlen;j++)
	{
		if(	(wire[j].parent == parent) &&
			(wire[j].child == child) )
		{
			return j;
		}
	}

	j = act->wlen;
	act->wlen += 1;

	wire[j].parent = parent;
	wire[j].child = child;
	return j;
}
static void graph_traverse(struct actor* act, struct arena* this)
{
	int j,k,ret;
	int nlen0, nlenx;
	int wlen0, wlenx;
	struct relation* irel;
	struct relation* orel;
	struct node* node = act->nbuf;
	struct pair* wire = act->wbuf;

	//first node
	nlen0 = act->nlen = 0;
	wlen0 = act->wlen = 0;
	graph_addnode(act, _win_, this);

	//start traverse
	for(j=0;j<6;j++)
	{
		//this.tail = 
		nlenx = act->nlen;
		wlenx = act->wlen;

		//for each
		for(k=nlen0;k<nlenx;k++)
		{
			this = node[k].addr;

			irel = this->irel0;
			while(1)
			{
				if(0 == irel)break;

				ret = graph_addnode(act, irel->srctype, (void*)(irel->srcchip));
				if(ret >= 0)graph_addpair(act, ret, k);

				irel = samedstnextsrc(irel);
			}

			orel = this->orel0;
			while(1)
			{
				if(0 == orel)break;

				ret = graph_addnode(act, orel->dsttype, (void*)(orel->dstchip));
				if(ret >= 0)graph_addpair(act, k, ret);

				orel = samesrcnextdst(orel);
			}
		}

		//next.start = this.tail
		nlen0 = nlenx;
		wlen0 = wlenx;
	}

/*	int j, k;
	struct arena* child;
	struct relation* rel;

	j = graph_add(_win_, this);
	rel = this->orel0;
	while(1)
	{
		if(rel == 0)return;

		child = (void*)(rel->dstchip);
		k = graph_add(rel->dsttype, child);

		graph_traverse(act, child);
		graph_pair(j, k);

		rel = samesrcnextdst(rel);
	}*/
}
static void graph_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j;
	float* vbuf = act->vbuf;
	float* wbuf = act->wbuf;

	if(_char_ == ev->what)
	{
		if('g' == ev->why)
		{
			//generate node, wire
			graph_traverse(act, win);

			//generate vbuf, ibuf;
			act->vlen = act->nlen;
			act->ilen = act->wlen;
			for(j=0;j<act->vlen;j++)
			{
				vbuf[j*3 + 0] = sty->f.vc[0] + (getrandom() & 0xffff) / 16.0;
				vbuf[j*3 + 1] = sty->f.vc[1] + (getrandom() & 0xffff) / 16.0;
				vbuf[j*3 + 2] = sty->f.vc[2] + (getrandom() & 0xffff) / 16.0;
				say("%f,%f,%f\n", vbuf[j*3 + 0], vbuf[j*3 + 1], vbuf[j*3 + 2]);
			}
say("%d,%d,%d,%d\n",act->nlen, act->wlen, act->vlen, act->ilen);
		}
	}
}




static void graph_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	graph_draw(act, pin, win, sty);
}
static void graph_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	graph_event(act, pin, win, sty, ev, 0);
}
static void graph_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void graph_start(struct halfrel* self, struct halfrel* peer)
{
	int j;
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	float* vbuf = act->vbuf;
	float* wbuf = act->wbuf;

	//generate node, wire
	graph_traverse(act, win);

	//generate vbuf, ibuf;
	act->vlen = act->nlen;
	act->ilen = act->wlen;
	for(j=0;j<act->vlen;j++)
	{
		vbuf[j*3 + 0] = sty->f.vc[0] + (getrandom() & 0xffff) / 16.0;
		vbuf[j*3 + 1] = sty->f.vc[1] + (getrandom() & 0xffff) / 16.0;
		vbuf[j*3 + 2] = sty->f.vc[2] + (getrandom() & 0xffff) / 16.0;
		say("%f,%f,%f\n", vbuf[j*3 + 0], vbuf[j*3 + 1], vbuf[j*3 + 2]);
	}
	say("%d,%d,%d,%d\n", act->nlen, act->wlen, act->vlen, act->ilen);
}




static void graph_search(struct actor* act)
{
}
static void graph_modify(struct actor* act)
{
}
static void graph_delete(struct actor* act)
{
	if(0 == act)return;

	//geom
	if(act->ibuf){
		memorydelete(act->ibuf);
		act->ibuf = 0;
	}
	if(act->vbuf){
		memorydelete(act->vbuf);
		act->vbuf = 0;
	}

	//node
	if(act->ibuf){
		memorydelete(act->ibuf);
		act->ibuf = 0;
	}
	if(act->nbuf){
		memorydelete(act->nbuf);
		act->nbuf = 0;
	}
}
static void graph_create(struct actor* act, void* str)
{
	if(0 == act)return;

	//node
	act->nbuf = memorycreate(0x1000);
	act->wbuf = memorycreate(0x1000);

	//geom
	act->vbuf = memorycreate(0x1000);
	act->ibuf = memorycreate(0x1000);
}




void graph_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->oncreate = (void*)graph_create;
	p->ondelete = (void*)graph_delete;
	p->onsearch = (void*)graph_search;
	p->onmodify = (void*)graph_modify;

	p->onstart = (void*)graph_start;
	p->onstop  = (void*)graph_stop;
	p->onread  = (void*)graph_read;
	p->onwrite = (void*)graph_write;
}
