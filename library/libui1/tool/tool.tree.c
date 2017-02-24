#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui
void hexadecimal(
	int x,int y,u64 in);
void decimal(
	int x,int y,u64 in);
void printdouble(
	int x, int y, int size, double z, u32 fgcolor, u32 bgcolor);
void printstring(
	int x, int y, int size, char* str, u32 fgcolor, u32 bgcolor);
void printascii(
	int x, int y, int size, char ch, u32 fgcolor, u32 bgcolor);
void line(
	int,int,int,int,u32);
void backgroundcolor(
	u64, u64, u64, u64,
	u32
);
//libsoft
double calculator(char* postfix);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);
//libboot
void say(char*,...);
void printmemory(char*,int);




//
static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 buffer;
        u64 format;
        u64 width;
        u64 height;
}*haha;

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

//
static struct mathnode* node=0;
static int count=0;

//
static char buffer[128];
static char postfix[128];




static void printnode(int x,int y,int num)
{
	int left,right;
	int offset,temp;

	//拿
	//say("node:%d\n",num);
	if(y>13)return;
	left=node[num].left;
	right=node[num].right;

	//偏移
	offset = (haha->width)/4;
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
		printdouble(
			x,
			temp,
			0,
			node[num].floatpoint,
			0xffffffff,
			0
		);
	}
	else if(node[num].type == 0x2f2a2d2b)		//+,-,*,/...
	{
		printascii(
			x,
			temp,
			1,
			node[num].integer & 0xff,
			0xffffffff,
			0
		);
	}
	else
	{
		printascii(
			x,
			temp,
			1,
			node[num].type & 0xff,
			0xffffffff,
			0
		);
	}

	//left
	if(left!=0&&left<128)
	{
		line(x, 64*y+16, x-offset, 64*y+64, 0xffffffff);
		printnode( x-offset , y+1 , left );
	}

	//right
	if(right!=0&&right<128)
	{
		line(x, 64*y+16, x+offset, 64*y+64, 0xffffffff);
		printnode( x+offset , y+1 , right );
	}
	//say("this=%d,left=%d,right=%d\n",num,left,right);
}








static void tree_write(u64* who, u64* a, u64* b)
{
	u64 type = *a;
	u64 key = *b;

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
static void tree_read()
{
	backgroundcolor(
		haha->buffer, 0, haha->width, haha->height,
		0
	);
	printstring(0, 0, 1, buffer, 0xffffffff, 0);
	printstring(0, 16, 1, postfix, 0xffffffff, 0);
	if(node==0)return;

	//等式
	if(node[0].type==0x3d3d3d3d)
	{
		printnode((haha->width)/2, 1, 0);
	}

	//算式
	else
	{
		printnode((haha->width)/2, 1, node[0].right);
	}
}




static void tree_list()
{
}
static void tree_into()
{
}




static void tree_start()
{
	u64 t = 0x72616863;
	u64 k = 0xd;

	buffer[0]='1';
	buffer[1]='+';
	buffer[2]='1';

	tree_write((void*)0, &t, &k);
}
static void tree_stop()
{
}
void tree_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x65657274;

	this[10]=(u64)tree_start;
	this[11]=(u64)tree_stop;
	this[12]=(u64)tree_list;
	this[13]=(u64)tree_into;
	this[14]=(u64)tree_read;
	this[15]=(u64)tree_write;

	node=(struct mathnode*)(base+0x200000);
}
void tree_delete()
{
}
