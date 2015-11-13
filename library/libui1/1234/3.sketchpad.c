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

void printdouble(int x,int y,double z);
void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);
void draw(int x,int y,DWORD color);

void background3();
void cleanscreen();

double sketchpad(struct mathnode*,double,double);
double calculator(char* postfix,double,double);
double beautifulbetween(double first,double second);

void postfix2binarytree(char* postfix,struct mathnode* out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);
void kexuejishufa(double* haha,int* counter);

void printmemory(char*,int);
void diary(char*,...);
QWORD screendata();
QWORD screenresolution();
QWORD whereisworld();




//
static int changed=0;
static int count=0;
static char buffer[128];

//
static char infix[128];
static char postfix[128];
static char result[128];
static char* datahome=0;
static struct mathnode* node=0;

//
static double scale;
static double centerx;
static double centery;
		//datahome 里面存放计算得到的值的符号
		//scale=“屏幕”上两个点对于“世界”上的距离
		//centerxy = “屏幕”对应“世界”哪个点




//
void wangge()
{
	int x,y,temp;
	double first,second,res;

	int wanggex,wanggey,wanggedistance;		//只用在"画网格这一步"
	DWORD* screenbuf=(DWORD*)screendata();




	//算屏上两行的间距，交点横坐标，纵坐标
	temp = 0;
	res = scale;	//scale=a*(10^b)	->	distance=250/a(大于25，小于250)
	kexuejishufa( &res , &temp );
	wanggedistance=(int)( 250.00 / res );

	first = centerx - scale * 512;
	second = centerx + scale * 512;
	res=beautifulbetween( first , second );
	res=(res-first) / (second-first) * 1024;
	wanggex = ( (int)res ) % wanggedistance;

	first = centery - scale * 512;
	second = centery + scale * 512;
	res=beautifulbetween( first , second );
	res=(res-first) / (second-first) * 1024;
	wanggey = ( (int)res ) % wanggedistance;




	//画上网格,以及网格上对应那一行的x,y坐标值
	for(x=wanggex;x<1024;x+=wanggedistance)
	{//竖线
		for(y=0;y<768;y++)
		{
			screenbuf[y*1024+x]=0x44444444;
		}
	}//竖线

	for(y=wanggey;y<768;y+=wanggedistance)
	{//横线
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0x44444444;
		}
	}//横线

	//diary("%lf\n",scale);
	printdouble(wanggex/8,wanggey/16,scale);
}
void tuxiang()
{
	int x,y;
	int value1,value2,counter;
	double first,second,haha;

	DWORD* screenbuf=(DWORD*)screendata();




	//带入75万个坐标算结果
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
	}//calculate results




	//由算出的结果得到图像
	//屏幕(0,767)->data[(767-767)*1024+0],,,,(1023,0)->data[(767-0)*1024+1023]
	for(y=1;y<767;y++)		//边缘四个点确定中心那一点有没有
	{
		value1=(767-y)<<10;
		for(x=1;x<1023;x++)
		{
			counter=0;
			if( datahome[ value1-1025 + x ] > 0 )counter--;
			else counter++;

			if( datahome[ value1-1023 + x ] > 0 )counter--;
			else counter++;

			if( datahome[ value1+1023 + x ] > 0 )counter--;
			else counter++;

			if( datahome[ value1+1025 + x ] > 0 )counter--;
			else counter++;

			//上下左右四点符号完全一样，说明没有点穿过//否则白色
			if( (counter!=4) && (counter!=-4) )screenbuf[y*1024+x]=0xffffffff;
		}
	}//result2img
}
void f3show()
{
	//跳过
	if(node[0].type!=0x3d3d3d3d)goto skipthese;
	if(changed==0)goto skipthese;
	changed=0;




	background3();
	if(node[0].integer == 0)
	{
		//计算器
		double haha=calculator(postfix,0,0);
		double2decimalstring(haha,result);
	}
	else
	{
		//网格，函数图
		wangge();
		tuxiang();
	}//else





skipthese:		//打印
	string(0,0,buffer);
	string(0,1,infix);
	string(0,2,postfix);
	string(0,3,result);
	return;
}








void f3message(QWORD type,QWORD key)
{
	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			centerx -= scale*100;
			changed=1;
		}
		else if(key==0x27)		//right	0x4d
		{
			centerx += scale*100;
			changed=1;
		}
		else if(key==0x26)		//up	0x4b
		{
			centery += scale*100;
			changed=1;
		}
		else if(key==0x28)		//down	0x4d
		{
			centery -= scale*100;
			changed=1;
		}
	}
	else if(type==0x72616863)		//'char'
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
			postfix2binarytree(postfix,node);

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
	else if(type==0x7466656C207A7978)		//'xyz left'
	{
		//浮动框以外的
		//px=x/(1024/0x40);
		//py=y/(640/40);
	}
	else if(type==0x65766F6D207A7978)		//'xyz move'
	{
		int dx=(int)(short)(key&0xffff);
		int dy=(int)(short)((key>>16)&0xffff);
		//diary("%d,%d\n",dx,dy);

		centerx -= scale*dx;
		centery += scale*dy;
		changed=1;
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		//保证鼠标之前指着哪儿(x,y)，之后就指着哪儿(x,y)
		//centerx+scale*pointx = x = newcenterx+scale/1.2*pointx -> newcenterx=centerx+scale*pointx*(1-1/1.2)
		int x,y;
		x=(key&0xffff)-512;
		y=384-((key>>16)&0xffff);
		centerx += ((double)x) * scale * (1-1/1.2);
		centery += ((double)y) * scale * (1-1/1.2);
		//diary("%d,%lf\n",x,centerx);

		scale/=1.2;
		changed=1;
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		int x,y;
		x=(key&0xffff)-512;
		y=384-((key>>16)&0xffff);
		centerx += ((double)x) * scale * (-0.2);
		centery += ((double)y) * scale * (-0.2);

		scale*=1.2;
		changed=1;
	}
	//else if(type==0x6B636162207A7978)		//'xyz ++++'
	//else if(type==0x6B636162207A7978)		//'xyz ----'
}








void f3init()
{
	if(datahome==0)
	{
		char* world=(char*)whereisworld();
		node=(struct mathnode*)(world+0x200000);
		datahome=world+0x300000;

		centerx=0.00;
		centery=0.00;
		scale=1.00;
	}
	else	//不是第一次进来了
	{
		cleanscreen();
	}
}
