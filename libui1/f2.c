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




void background2();

void line(QWORD point1,QWORD point2,DWORD color);
void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);
void printdouble(int x,int y,double z);
void anscii(int x,int y,int z);

double calculator(char* postfix);
void postfix2binarytree(char* postfix,struct mathnode** out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

void say(char*,...);
void diary(char*,...);
void printmemory(char*,int);
QWORD whereisworld();




//
static int count=0;

static char buffer[128];
static char postfix[128];

static struct mathnode* node=0;




//
static QWORD dirbuffer;
void printfile0()
{
	int x,y;
	char* p=(char*)dirbuffer;

	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0,y+2,p+0x40*y);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
}




void printnode(int x,int y,int num)
{
	int left,right;
	int offset,temp;
	int thisx,thisy;

	//拿
	if(y>13)return;
	left=node[num].left;
	right=node[num].right;

	//偏移
	offset=1024;
	temp=y+1;
	while(1)
	{
		offset >>= 1;

		temp--;
		if(temp==0)break;
	}
	//diary("offset=%d\n",offset);

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
		anscii(thisx,thisy,node[num].integer & 0xff);
	}
	else
	{
		anscii(thisx,thisy,node[num].type & 0xff);
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
	//diary("this=%d,left=%d,right=%d\n",num,left,right);
}




void f2show()
{
	int temp;
	background2();

	string(0,0,buffer);
	string(0,1,postfix);

	if(node!=0)
	{
		temp=node[0].left;
		if(temp!=0&&temp<128)
		{
			printnode(1024/2,1,temp);
		}
	}
}
void f2message(QWORD type,QWORD key)
{
	if(type==0x72616863)		//char
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

			postfix2binarytree(postfix,&node);

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
	}
}








void f2init(QWORD world)
{
	dirbuffer=world+0x200000;
}
