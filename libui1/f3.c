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
static int changed=0;
static struct mathnode* node;

//
static char* datahome;
static double scale;
static double centerx;
static double centery;

//
static int count=0;
static char buffer[128];

//
static char infix[128];
static char postfix[128];
static char result[128];




//
void f3show()
{
	int x,y;
	double haha,first,second;	//只用在"带入坐标算结果"这一步
	int temp,counter;			//只用在"由算出的结果得到图像"这一步
	int wanggex,wanggey;		//只用在"画网格这一步"
	DWORD* screenbuf=(DWORD*)screendata();




	//跳过
	if(changed==0)goto skipthese;
	changed=0;




	//
	if(node[0].integer == 0)	//简单的算式
	{
		//计算器
		haha=calculator(postfix,0,0);
		double2decimalstring(haha,result);
	}
	else	//有等号的式子才要画图
	{
		//带入一百万个坐标算结果
		//逻辑(0,0)->(centerx,centery),,,,(1023,767)->(centerx+scale*1023,centery+scale*767)
		for(y=0;y<768;y++)		//只算符号并且保存
		{
			second=centery + (y-384)*scale;
			for(x=0;x<1024;x++)
			{
				first=centerx + (x-512)*scale;
				haha=sketchpad(node,first,second);

				if(haha>0)datahome[1024*y+x]=1;
				else datahome[1024*y+x]=-1;
			}
		}

		//由算出的结果得到图像
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

		//准备"画网格"
		//centerxy=“屏幕”对应“世界”哪个点
		//scale=“屏幕”上两个点对于“世界”上的距离
		//通过这两个值，得到最靠近屏幕中心的那个”网格上的横竖交汇点“
		wanggex=512;
		wanggey=384;

		for(y=0;y<768;y++)
		{
			for(x=wanggex-500;x<wanggex+500;x+=100)		//10行，100为间隔
			{
				screenbuf[y*1024+x]=0x44444444;
			}
		}
		for(y=wanggey-300;y<wanggey+300;y+=100)		//10行，100为间隔
		{
			for(x=0;x<1024;x++)
			{
				screenbuf[y*1024+x]=0x44444444;
			}
		}

	}//else




	//打印
skipthese:
	background3();
	string(0,0,buffer);
	string(0,1,infix);
	string(0,2,postfix);
	string(0,3,result);
}
void f3message(QWORD type,QWORD key)
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
			//检查buffer，然后给infix
			printmemory(buffer,128);

			//清空输入区
			for(count=0;count<127;count++)
			{
				infix[count]=buffer[count];
				buffer[count]=0;
			}
			count=0;

			//134+95*x+(70*44+f)*g -> 134 95 x *+ 70 44 * f + g *+
			infix2postfix(infix,postfix);
			postfix2binarytree(postfix,&node);

			//告诉打印员
			changed=1;
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
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		scale/=1.2;
		changed=1;
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		scale*=1.2;
		changed=1;
	}
}








void f3init(QWORD world)
{
	datahome=(char*)world+0x300000;
	centerx=0.00;
	centery=0.00;
	scale=1.00;
}
