#include "libuser.h"
#define nbuf buf0
#define wbuf buf1
#define vbuf buf2
#define ibuf buf3
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{/*
	struct entity* aa;
	struct entity* bb;
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
			if(orig[i].type == _sup_)
			{
				aa = orig[i].addr;
				p = &(aa->fmt);
			}
			else if(orig[i].type == _ent_)
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
static void graph_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float n;
	u8 buf[0x1000];
	vec3 tc, tr, tf, tu;
	int rgb;
	int i,j,k;
	struct entity* tmp;
	void* str;
	struct node* node = act->nbuf;
	struct pair* wire = act->wbuf;
	float* vbuf = act->vbuf;
	float* ibuf = act->ibuf;
	//gl41line_prism4(win, 0x00ff00, sty->vc, sty->vr, sty->vf, sty->vu);

	if(vbuf)
	{
		for(j=0;j<100;j++)
		{
			forcedirected_3d(buf, act->vlen, act->vbuf, act->vlen, act->wbuf, act->wlen);
			vbuf[0] = sty->fs.vc[0];
			vbuf[1] = sty->fs.vc[1];
			vbuf[2] = sty->fs.vc[2];
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

		gl41line_arrow(win, 0xffffff, &vbuf[j*3], &vbuf[k*3], sty->fs.vt);
	}

	//tr[0] = win->camera.vr[0];
	//tr[1] = win->camera.vr[1];
	//tr[2] = win->camera.vr[2];
	tr[0] = 1.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	n = 10.0 / squareroot(tr[0]*tr[0] + tr[1]*tr[1] + tr[2]*tr[2]);
	tr[0] *= n;
	tr[1] *= n;
	tr[2] *= n;

	//tf[0] = win->camera.vt[0];
	//tf[1] = win->camera.vt[1];
	//tf[2] = win->camera.vt[2];
	tf[0] = 0.0;
	tf[1] = 1.0;
	tf[2] = 0.0;
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
			case _sys_:rgb = 0x0000ff;str = (void*)&tmp->type;break;
			case _art_:rgb = 0xff0000;str = (void*)&tmp->type;break;
			case _sup_:rgb = 0xffff00;str = (void*)&tmp->fmt;break;
			case _ent_:rgb = 0x00ffff;str = (void*)&tmp->fmt;break;
			default:rgb = 0xff00ff;str = (void*)"????????";
		}
		gl41string_center(win, rgb, &vbuf[j*3], tr, tf, str, 8);
	}
}
static void graph_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graph_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graph_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graph_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




int graph_addnode(struct entity* act, u64 type, void* addr)
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
int graph_addpair(struct entity* act, int parent, int child)
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
		if(	(wire[j].parent == child) &&
			(wire[j].child == parent) )
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
static void graph_traverse(struct entity* act, struct entity* this)
{/*
	int j,k,ret;
	int nlen0, nlenx;
	int wlen0, wlenx;
	struct node* node = act->nbuf;
	struct pair* wire = act->wbuf;

	//first node
	nlen0 = act->nlen = 0;
	wlen0 = act->wlen = 0;
*/
	struct relation* irel;
	struct relation* orel;
	int parent, child;

	parent = graph_addnode(act, _sup_, this);

	irel = this->irel0;
	while(1){
		if(0 == irel)break;

		child = graph_addnode(act, irel->srctype, (void*)(irel->srcchip));
		if(parent != child)graph_addpair(act, parent, child);

		irel = samedstnextsrc(irel);
	}

	orel = this->orel0;
	while(1){
		if(0 == orel)break;

		child = graph_addnode(act, orel->dsttype, (void*)(orel->dstchip));
		if(parent != child)graph_addpair(act, parent, child);

		orel = samesrcnextdst(orel);
	}
}
static void graph_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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
			graph_traverse(act, act);

			//generate vbuf, ibuf;
			act->vlen = act->nlen;
			act->ilen = act->wlen;
			for(j=0;j<act->vlen;j++)
			{
				vbuf[j*3 + 0] = sty->fs.vc[0] + (getrandom() & 0xffff) / 16.0;
				vbuf[j*3 + 1] = sty->fs.vc[1] + (getrandom() & 0xffff) / 16.0;
				vbuf[j*3 + 2] = sty->fs.vc[2] + (getrandom() & 0xffff) / 16.0;
				say("%f,%f,%f\n", vbuf[j*3 + 0], vbuf[j*3 + 1], vbuf[j*3 + 2]);
			}
say("%d,%d,%d,%d\n",act->nlen, act->wlen, act->vlen, act->ilen);
		}
	}
}




static void graph_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void graph_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void graph_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void graph_linkup(struct halfrel* self, struct halfrel* peer)
{
	int j;
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	float* vbuf = act->vbuf;
	float* wbuf = act->wbuf;

	//generate node, wire
	graph_traverse(act, act);

	//generate vbuf, ibuf;
	act->vlen = act->nlen;
	act->ilen = act->wlen;
	for(j=0;j<act->vlen;j++)
	{
		vbuf[j*3 + 0] = sty->fs.vc[0] + (getrandom() & 0xffff) / 16.0;
		vbuf[j*3 + 1] = sty->fs.vc[1] + (getrandom() & 0xffff) / 16.0;
		vbuf[j*3 + 2] = sty->fs.vc[2] + (getrandom() & 0xffff) / 16.0;
		say("%f,%f,%f\n", vbuf[j*3 + 0], vbuf[j*3 + 1], vbuf[j*3 + 2]);
	}
	say("%d,%d,%d,%d\n", act->nlen, act->wlen, act->vlen, act->ilen);
}




static void graph_search(struct entity* act)
{
}
static void graph_modify(struct entity* act)
{
}
static void graph_delete(struct entity* act)
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
static void graph_create(struct entity* act, void* str)
{
	if(0 == act)return;

	//node
	act->nbuf = memorycreate(0x1000, 0);
	act->wbuf = memorycreate(0x1000, 0);

	//geom
	act->vbuf = memorycreate(0x1000, 0);
	act->ibuf = memorycreate(0x1000, 0);
}




void graph_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->oncreate = (void*)graph_create;
	p->ondelete = (void*)graph_delete;
	p->onsearch = (void*)graph_search;
	p->onmodify = (void*)graph_modify;

	p->onlinkup = (void*)graph_linkup;
	p->ondiscon = (void*)graph_discon;
	p->ontaking = (void*)graph_taking;
	p->ongiving = (void*)graph_giving;
}
