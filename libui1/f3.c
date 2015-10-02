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

double sketchpad(struct mathnode*,double,double);
double calculator(char* postfix,double,double);
void postfix2binarytree(char* postfix,struct mathnode** out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

QWORD screendata();
QWORD screenresolution();

void printmemory(char*,int);
QWORD whereisworld();




//
static char* datahome;
static double scale;
static double areax;
static double areay;

//
static int count=0;
static char buffer[128];

static char postfix[128];
static char result[128];




//
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
	int temp,counter;
	double haha,first,second;
	struct mathnode* node;
	DWORD* screenbuf=(DWORD*)screendata();

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

			//几何画板
			postfix2binarytree(postfix,&node);
			if(node[0].integer == 0)	//简单的算式
			{
				//计算器
				haha=calculator(postfix,0,0);
				double2decimalstring(haha,result);
			}
			else	//有等号的式子才要画图
			{
				//逻辑(0,0)->(areax,areay),,,,(1023,767)->(areax+scale*1023,areay+scale*767)
				for(y=0;y<768;y++)		//只算符号并且保存
				{
					second=areay + y*scale;
					for(x=0;x<1024;x++)
					{
						first=areax + x*scale;
						haha=sketchpad(node,first,second);

						if(haha>0)datahome[1024*y+x]=1;
						else datahome[1024*y+x]=-1;
					}
				}

				//屏幕(0,767)->data[(767-767)*1024+0],,,,(1023,0)->data[(767-0)*1024+1023]
				for(y=1;y<767;y++)		//边缘四个点确定中心那一点有没有
				{
					temp=(767-y)<<10;
					for(x=1;x<1023;x++)
					{
						counter=0;
						if( datahome[ temp-1025 + x ] > 0 )counter--;
						else counter++;

						if( datahome[ temp-1023 + x ] > 0 )counter--;
						else counter++;

						if( datahome[ temp+1023 + x ] > 0 )counter--;
						else counter++;

						if( datahome[ temp+1025 + x ] > 0 )counter--;
						else counter++;

						//上下左右四点符号完全一样，说明没有点穿过
						if( (counter==4) | (counter==-4) )screenbuf[y*1024+x]=0;
						else screenbuf[y*1024+x]=0xffffffff;		//否则白色
					}
				}
			}

			//清空输入区
			//for(count=0;count<127;count++) buffer[count]=0;
			//count=0;
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
	datahome=(char*)world+0x300000;
	areax=-512;
	areay=-383;
	scale=1.00;
}
