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
void background3();

void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);
void draw(int x,int y,DWORD color);

double sketchpad(struct mathnode*,int,int);
double calculator(char* postfix,int,int);
void postfix2binarytree(char* postfix,struct mathnode** out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

void printmemory(char*,int);
QWORD whereisworld();








//
static int count=0;
static char buffer[128];

static char postfix[128];
static char result[128];




void f3show()
{
	background3();

	string(0,0,buffer);
	string(0,1,postfix);

	string(0,2,result);
}
void f3message(QWORD type,QWORD key)
{
	int x,y;
	double haha;
	struct mathnode* node;

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

			//计算器
			haha=calculator(postfix,0,0);
			double2decimalstring(haha,result);

			//几何画板
			postfix2binarytree(postfix,&node);
			if(node[0].integer != 0)	//有等号的式子才要画图
			{
				for(y=-384;y<384;y++)
				{
				for(x=-512;x<512;x++)
				{
					haha=sketchpad(node,x,y);
					if( (haha<0.01) && (haha>-0.01) )
					{
						draw(x,y,0xffffffff);
					}
					else draw(x,y,0);
				}
				}
			}

			//清空输入区
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








void f3init(QWORD world)
{
}
