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
static struct bintree* node=0;
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
static void bintree_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	struct bintree* right;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;

	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);
	drawstring(win, 0xffffff, cx-ww, cy-hh, buffer, 0);
	drawstring(win, 0xffffff, cx-ww, cy-hh+16, postfix, 0);

	if(node == 0)return;
	right = bintree_getright(node, node);
	if(right == 0)return;

	printnode(win, right, cx, 1, cx, cy, ww, hh);
}
static void bintree_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bintree_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bintree_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void bintree_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("tree(%x,%x,%x)\n",win,act,sty);
}
static void bintree_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)bintree_read_cli(win, act, sty);
	else if(fmt == __tui__)bintree_read_tui(win, act, sty);
	else if(fmt == __html__)bintree_read_html(win, act, sty);
	else if(fmt == __vbo__)bintree_read_vbo(win, act, sty);
	else bintree_read_pixel(win, act, sty);
}
static void bintree_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == __char__)
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
static void bintree_start()
{
}
static void bintree_stop()
{
}
void bintree_create(void* base,void* addr)
{
	struct actor* act = addr;
	node = (struct bintree*)(base+0x300000);

	act->type = hex32('h', 'a', 'c', 'k');
	act->name = hex64('b', 'i', 'n', 't', 'r', 'e', 'e', 0);

	act->start = (void*)bintree_start;
	act->stop = (void*)bintree_stop;
	act->list = (void*)bintree_list;
	act->choose = (void*)bintree_into;
	act->read = (void*)bintree_read;
	act->write = (void*)bintree_write;
}
void bintree_delete()
{
}
