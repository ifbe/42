#include "actor.h"
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




static void printnode(struct arena* win, struct bintree* this, int x, int y,
	int cx, int cy, int ww, int hh)
{
	int j,k;
	struct bintree* left;
	struct bintree* right;
	u8 data[32];

	//0,1,2,3...
	if(this->type == '0')
	{
		j = double2decstr(this->floatpoint, data);
		drawicon_1(win, 0x0000ff,
			x-32, cy-hh+y*64-16,
			x+32, cy-hh+y*64+16,
			data, j
		);
	}

	//+, -, *, /, x, y, z
	else
	{
		drawicon_1(win, 0xff0000,
			x-32, cy-hh+y*64-16,
			x+32, cy-hh+y*64+16,
			(u8*)&(this->type), 1
		);
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
static void bintree_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct bintree* node;
	struct bintree* right;
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);
	drawstring(win, 0xffffff, cx-ww, cy-hh, buffer, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+16, postfix, 0);

	node = act->buf;
	if(node == 0)return;
	right = bintree_getright(node, node);
	if(right == 0)return;

	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bintree_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bintree_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bintree_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"bintree\" style=\"width:50%%;height:100px;float:left;background-color:#457645;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void bintree_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void bintree_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}
static void bintree_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)bintree_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)bintree_read_tui(win, sty, act, pin);
	else if(fmt == _html_)bintree_read_html(win, sty, act, pin);
	else if(fmt == _json_)bintree_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)bintree_read_vbo(win, sty, act, pin);
	else bintree_read_pixel(win, sty, act, pin);
}
static void bintree_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
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
static void bintree_list()
{
}
static void bintree_into()
{
}
static void bintree_start(struct actor* act, struct pinid* pin)
{
}
static void bintree_stop(struct actor* act, struct pinid* pin)
{
}
static void bintree_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void bintree_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = node;
	if(_copy_ == act->type)act->buf = memorycreate(256);
}




void bintree_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('b', 'i', 'n', 't', 'r', 'e', 'e', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)bintree_create;
	p->ondelete = (void*)bintree_delete;
	p->onstart  = (void*)bintree_start;
	p->onstop   = (void*)bintree_stop;
	p->onlist   = (void*)bintree_list;
	p->onchoose = (void*)bintree_into;
	p->onread   = (void*)bintree_read;
	p->onwrite  = (void*)bintree_write;
}
