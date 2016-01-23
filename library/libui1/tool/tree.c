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




//libui
void line(QWORD point1,QWORD point2,DWORD color);
void colorstring(int x,int y,char* str,unsigned int color);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);
void printdouble(int x,int y,double z);
void colorascii(int x,int y,int z,unsigned int color);
void background2();
void cleanscreen();
QWORD whereischaracter();
//libsoft
double calculator(char* postfix);
void postfix2binarytree(char* postfix,struct mathnode* out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);
//libboot
void say(char*,...);
void printmemory(char*,int);




//
static int count=0;

static char buffer[128];
static char postfix[128];

static struct mathnode* node=0;




//
static void printfile0()
{
	int x,y;
	char* p=(char*)node;

	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		colorstring(0,y+2,p+0x40*y,0xcccccc);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
}
static void printnode(int x,int y,int num)
{
	int left,right;
	int offset,temp;
	int thisx,thisy;

	//拿
	say("%d\n",num);
	if(y>13)return;
	left=node[num].left;
	right=node[num].right;

	//偏移
	offset=256;
	temp=y;
	while(1)
	{
		offset >>= 1;

		temp--;
		if(temp==0)break;
	}
	//say("offset=%d\n",offset);

	//位置
	thisx=x/8;
	thisy=y*4;
	if(node[ node[num].up ].left == num)	//是左边
	{
		if(node[num].left==0&&node[num].right==0)	//而且是叶子
		{
			if(y>=7)		//而且放不下了
			{
				thisy+=1;
			}
		}
	}

	//self
	if(node[num].type == 0x33323130)	//0,1,2,3...
	{
		printdouble(thisx,thisy,node[num].floatpoint);
	}
	else if(node[num].type == 0x2f2a2d2b)		//+,-,*,/...
	{
		colorascii(thisx , thisy , node[num].integer & 0xff , 0xffffff);
	}
	else
	{
		colorascii(thisx , thisy , node[num].type & 0xff , 0xffffff);
	}

	//left
	if(left!=0&&left<128)
	{
		line( x+((64*y+16)<<16) , (x-offset)+( (64*y+64)<<16) , 0xffffffff);
		printnode( x-offset , y+1 , left );
	}

	//right
	if(right!=0&&right<128)
	{
		line( x+((64*y+16)<<16) , (x+offset)+( (64*y+64)<<16) , 0xffffffff);
		printnode( x+offset , y+1 , right );
	}
	//say("this=%d,left=%d,right=%d\n",num,left,right);
}








static void writetree(QWORD type,QWORD key)
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
			//134+95*x+(70*44+f)*g -> 134 95 x *+ 70 44 * f + g *+
			printmemory(buffer,128);
			infix2postfix(buffer,postfix);

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
static void readtree()
{
	int left,right;
	background2();

	colorstring(0,0,buffer,0xcccccc);
	colorstring(0,1,postfix,0xcccccc);

	if(node!=0)
	{
		//等式左边
		left=node[0].left;
		if(left!=0&&left<128)
		{
			printnode(256,1,left);
		}

		//等式右边
		right=node[0].right;
		if(right!=0&&right<128)
		{
			printnode(768,1,right);
		}
	}
}
static void intotree()
{
	if(node==0)
	{
		node=(struct mathnode*)(whereischaracter()+0x200000);
	}
	else
	{
		cleanscreen();
	}
}
static void listtree(QWORD* this)
{
	this[0]=0x776f646e6977;
	this[1]=0x65657274;
	this[2]=(0<<16)+0;		//left,up
	this[3]=(768<<16)+1024;		//write,down
	this[4]=(QWORD)listtree;
	this[5]=(QWORD)intotree;
	this[6]=(QWORD)readtree;
	this[7]=(QWORD)writetree;
}








void registertree(char* in)
{
	listtree( (QWORD*)in );
}
