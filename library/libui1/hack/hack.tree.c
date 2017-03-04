#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui
void printdouble(void*,
	int x, int y, int size,
	double z, u32 fgcolor, u32 bgcolor);
void printstring(void*,
	int x, int y, int size,
	char* str, u32 fgcolor, u32 bgcolor);
void printascii(void*,
	int x, int y, int size,
	char ch, u32 fgcolor, u32 bgcolor);
void line(void*,
	int x1, int y1,
	int x2, int y2,
	u32);
void backgroundcolor(void*,
	u32);
//libsoft
double calculator(char* postfix);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
//libboot
void say(char*,...);
void printmemory(char*,int);




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u8 data[0xe0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
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




static void printnode(struct window* win, int x,int y,int num)
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
		printdouble(win,
			x, temp, 0,
			node[num].floatpoint, 0xffffffff, 0
		);
	}
	else if(node[num].type == 0x2f2a2d2b)		//+,-,*,/...
	{
		printascii(win,
			x, temp, 1,
			node[num].integer & 0xff, 0xffffffff, 0
		);
	}
	else
	{
		printascii(win,
			x, temp, 1,
			node[num].type & 0xff, 0xffffffff, 0
		);
	}

	//left
	if(left!=0&&left<128)
	{
		line(win,
			x, 64*y+16,
			x-offset, 64*y+64,
			0xffffffff
		);
		printnode(win, x-offset , y+1 , left );
	}

	//right
	if(right!=0&&right<128)
	{
		line(win,
			x, 64*y+16,
			x+offset, 64*y+64,
			0xffffffff
		);
		printnode(win, x+offset , y+1 , right );
	}
	//say("this=%d,left=%d,right=%d\n",num,left,right);
}
static void tree_read(struct window* win)
{
	backgroundcolor(win, 0);
	printstring(win, 0, 0, 1, buffer, 0xffffffff, 0);
	printstring(win, 0, 16, 1, postfix, 0xffffffff, 0);
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
	struct player* p = addr;
	node=(struct mathnode*)(base+0x200000);

	p->type = 0x6c6f6f74;
	p->name = 0x65657274;

	p->start = (u64)tree_start;
	p->stop = (u64)tree_stop;
	p->list = (u64)tree_list;
	p->choose = (u64)tree_into;
	p->read = (u64)tree_read;
	p->write = (u64)tree_write;
}
void tree_delete()
{
}
