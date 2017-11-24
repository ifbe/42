#include "actor.h"
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
double calculator(char* postfix);




struct mathnode{

	u32 type;
	u32 up;
	u32 left;
	u32 right;
	union{
		char datasize[16];
		double floatpoint;
		unsigned long long integer;
	};
};
static struct mathnode* node=0;
static int count=0;
static char buffer[128];
static char postfix[128];




static void printnode(struct arena* win, int x,int y,int num)
{
	int left,right;
	int offset,temp;

	//拿
	//say("node:%d\n",num);
	if(y>13)return;
	left=node[num].left;
	right=node[num].right;

	//偏移
	offset = (win->w)/4;
	temp = y;
	while(1)
	{
		temp--;
		if(temp<=0)break;

		offset >>= 1;
	}
	//say("offset=%d\n",offset);

	//位置
	temp=y*64;
	if(node[ node[num].up ].left == num)	//是左边
	{
		if(node[num].left==0&&node[num].right==0)	//而且是叶子
		{
			if(y>=7)		//而且放不下了
			{
				temp+=16;
			}
		}
	}

	//self
	if(node[num].type == 0x33323130)	//0,1,2,3...
	{
		drawdouble(
			win, node[num].floatpoint, 1,
			x, temp, 0xffffffff, 0
		);
	}
	else if(node[num].type == 0x2f2a2d2b)		//+,-,*,/...
	{
		drawascii(
			win, node[num].integer & 0xff, 1,
			x, temp, 0xffffffff, 0
		);
	}
	else
	{
		drawascii(
			win, node[num].type & 0xff, 1,
			x, temp, 0xffffffff, 0
		);
	}

	//left
	if(left!=0&&left<128)
	{
		drawline(win,
			x, 64*y+16,
			x-offset, 64*y+64,
			0xffffffff
		);
		printnode(win, x-offset , y+1 , left );
	}

	//right
	if(right!=0&&right<128)
	{
		drawline(win,
			x, 64*y+16,
			x+offset, 64*y+64,
			0xffffffff
		);
		printnode(win, x+offset , y+1 , right );
	}
	//say("this=%d,left=%d,right=%d\n",num,left,right);
}
static void tree_read(struct arena* win)
{
	drawstring(
		win, buffer, 1,
		0, 0, 0xffffffff, 0);
	drawstring(
		win, postfix, 1,
		0, 16, 0xffffffff, 0);
	if(node==0)return;

	//等式
	if(node[0].type==0x3d3d3d3d)
	{
		printnode(win, (win->w)/2, 1, 0);
	}

	//算式
	else
	{
		printnode(win, (win->w)/2, 1, node[0].right);
	}
}
static void tree_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type==0x72616863)		//'char'
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




static void tree_list()
{
}
static void tree_into()
{
}
static void tree_start()
{
}
static void tree_stop()
{
}
void tree_create(void* base,void* addr)
{
	struct actor* p = addr;
	node=(struct mathnode*)(base+0x200000);

	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('t', 'r', 'e', 'e');

	p->start = (void*)tree_start;
	p->stop = (void*)tree_stop;
	p->list = (void*)tree_list;
	p->choose = (void*)tree_into;
	p->read = (void*)tree_read;
	p->write = (void*)tree_write;
}
void tree_delete()
{
}
