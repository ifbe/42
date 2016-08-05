#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
struct mathnode{

        DWORD type;
        DWORD up;
        DWORD left;
        DWORD right;
        union{
                char datasize[16];
                double floatpoint;
                unsigned long long integer;
        };
};




//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//libui
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);
void printdouble(double z,DWORD xyz,DWORD fgcolor,DWORD bgcolor);
void printstring(char* str,DWORD xyz,DWORD fgcolor,DWORD bgcolor);
void printascii(char ch,DWORD xyz,DWORD fgcolor,DWORD bgcolor);
void line(int,int,int,int,DWORD);
void backgroundcolor(DWORD);
//libsoft
double calculator(char* postfix);
void postfix2binarytree(char* postfix,struct mathnode* out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);
//libboot
void say(char*,...);
void printmemory(char*,int);




//
static int width;
static int height;

//
static int count=0;
static struct mathnode* node=0;

//
static char buffer[128];
static char postfix[128];




//
static void printfile0()
{
	int x,y;
	char* p=(char*)node;

	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		printstring(p+0x40*y, 0+(y+2)<<8, 0xcccccc, 0);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
}
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
	offset=width/4;
	temp=y;
	while(1)
	{
		temp--;
		if(temp<=0)break;

		offset >>= 1;
	}
	//say("offset=%d\n",offset);

	//位置
	temp=y*4;
	if(node[ node[num].up ].left == num)	//是左边
	{
		if(node[num].left==0&&node[num].right==0)	//而且是叶子
		{
			if(y>=7)		//而且放不下了
			{
				temp+=1;
			}
		}
	}

	//self
say("%d,%d\n",x,temp);
	if(node[num].type == 0x33323130)	//0,1,2,3...
	{
		printdouble(node[num].floatpoint, (x/8)+(temp<<8), 0xffffffff, 0);
	}
	else if(node[num].type == 0x2f2a2d2b)		//+,-,*,/...
	{
		printascii(node[num].integer & 0xff, (x/8)+(temp<<8), 0xffffffff, 0);
	}
	else
	{
		printascii(node[num].type & 0xff , (x/8)+(temp<<8), 0xffffffff, 0);
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








static void tree_write(QWORD type,QWORD key)
{
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
	backgroundcolor(0);
	printstring(buffer, 0+(0<<8), 0xffffffff, 0);
	printstring(postfix, 0+(1<<8), 0xffffffff, 0);
	if(node==0)return;

	//等式
	if(node[0].type==0x3d3d3d3d)
	{
		printnode(width/2, 1, 0);
	}

	//算式
	else
	{
		printnode(width/2, 1, node[0].right);
	}
}




static void tree_list()
{
}
static void tree_into()
{
	buffer[0]='1';
	buffer[1]='+';
	buffer[2]='1';
	tree_write(0x72616863, 0xd);
}




static void tree_start(QWORD size,void* addr)
{
	width=size&0xffff;
	height=(size>>16)&0xffff;

	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);
}
static void tree_stop()
{
}
void tree_init(char* base,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x65657274;
	this[2]=(QWORD)tree_start;
	this[3]=(QWORD)tree_stop;
	this[4]=(QWORD)tree_list;
	this[5]=(QWORD)tree_into;
	this[6]=(QWORD)tree_read;
	this[7]=(QWORD)tree_write;

	node=(struct mathnode*)(base+0x200000);
}
void tree_kill()
{
}
