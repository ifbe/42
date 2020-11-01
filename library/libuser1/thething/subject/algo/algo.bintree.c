#include "libuser.h"
int double2decstr(double, void*);
void infix2postfix(void*, void*);
void postfix2binarytree(void*, void*);
void* bintree_getleft(void*, void*);
void* bintree_getright(void*, void*);




struct bintree
{
	u16 left;
	u16 right;
	u16 up;
	u16 type;
	union
	{
		u64 integer;
		double floatpoint;
	};
};
static struct bintree node[16];
static int count=0;
static u8 buffer[128];
static u8 postfix[128];




static void printnode(struct entity* win, struct bintree* this, int x, int y,
	int cx, int cy, int ww, int hh)
{
	int j,k;
	int x0, y0, x1, y1;
	struct bintree* left;
	struct bintree* right;
	u8 data[32];

	//0,1,2,3...
	if(this->type == '0')
	{
		x0 = x-32;
		y0 = cy-hh+y*64-16;
		x1 = x+32;
		y1 = cy-hh+y*64+16;
		j = double2decstr(this->floatpoint, data);
		drawicon_1(win, 0x7fffffff, x0, y0, x1, y1);
		drawstring_fit(win, 0x0000ff, x0, y0, x1, y1, data, j);
	}

	//+, -, *, /, x, y, z
	else
	{
		x0 = x-32;
		y0 = cy-hh+y*64-16;
		x1 = x+32;
		y1 = cy-hh+y*64+16;
		drawicon_1(win, 0x7fffffff, x0, y0, x1, y1);
		drawstring_fit(win, 0xff0000, x0, y0, x1, y1, (u8*)&(this->type), 1);
	}

	k = ww;
	for(j=y;j>0;j--)k = k>>1;

	left = bintree_getleft(node, this);
	if(left != 0)
	{
		drawline(win, 0xffffff,
			x, cy-hh+y*64+16,
			x-k, cy-hh+y*64+48
		);
		printnode(win, left, x-k, y+1,
			cx, cy, ww, hh
		);
	}

	right = bintree_getright(node, this);
	if(right != 0)
	{
		drawline(win, 0xffffff,
			x, cy-hh+y*64+16,
			x+k, cy-hh+y*64+48
		);
		printnode(win, right, x+k, y+1,
			cx, cy, ww, hh
		);
	}

/*
	offset = (win->w)/4;
	temp = y;
	while(1)
	{
		temp--;
		if(temp<=0)break;

		offset >>= 1;
	}
	//say("offset=%d\n",offset);

	//left
	if(left!=0&&left<128)
	{
		drawline(win, 0xffffff, x, 64*y+16, x-offset, 64*y+64);
		printnode(win, x-offset , y+1 , left );
	}

	//right
	if(right!=0&&right<128)
	{
		drawline(win, 0xffffff, x, 64*y+16, x+offset, 64*y+64);
		printnode(win, x+offset , y+1 , right );
	}
	//say("this=%d,left=%d,right=%d\n",num,left,right);
*/
}
static void bintree_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct bintree* node;
	struct bintree* right;
	int cx = sty->fs.vc[0];
	int cy = sty->fs.vc[1];
	int ww = sty->fs.vr[0];
	int hh = sty->fs.vf[1];

	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);
	drawstring(win, 0xffffff, cx-ww, cy-hh, buffer, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+16, postfix, 0);

	node = act->buf0;
	if(node == 0)return;
	right = bintree_getright(node, node);
	if(right == 0)return;

	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bintree_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void bintree_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void bintree_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void bintree_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void bintree_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}




static void bintree_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _char_)
	{
		if(key==0x8)			//backspace
		{
			if(count!=0)count--;
			buffer[count]=0;
		}
		else if(key==0xd)		//enter
		{
			int i;
			char* p=(char*)node;
			for(i=0;i<0x1000;i++)p[i]=0;
			
			say("infix2postfix:%s\n",postfix);
			infix2postfix(buffer,postfix);

			say("postfix2binarytree......\n");
			postfix2binarytree(postfix,node);

			for(count=0;count<127;count++) buffer[count]=0;
			count=0;
		}
		else
		{
			if(count<128)
			{
				buffer[count]=key;
				count++;
			}
		}
	}//'char'
}




static void bintree_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void bintree_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void bintree_linkup(struct halfrel* self, struct halfrel* peer)
{
}
static void bintree_discon(struct halfrel* self, struct halfrel* peer)
{
}




static void bintree_search(struct entity* act)
{
}
static void bintree_modify(struct entity* act)
{
}
static void bintree_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void bintree_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = node;
	if(_copy_ == act->type)act->buf0 = memorycreate(256, 0);
}




void bintree_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'i', 'n', 't', 'r', 'e', 'e', 0);

	p->oncreate = (void*)bintree_create;
	p->ondelete = (void*)bintree_delete;
	p->onsearch = (void*)bintree_search;
	p->onmodify = (void*)bintree_modify;

	p->onlinkup = (void*)bintree_linkup;
	p->ondiscon = (void*)bintree_discon;
	p->ontaking = (void*)bintree_taking;
	p->ongiving = (void*)bintree_giving;
}
