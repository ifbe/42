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
void postfix2binarytree(char* postfix,struct mathnode** out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

QWORD screendata();
QWORD screenresolution();

void printmemory(char*,int);
void diary(char*,...);
QWORD whereisworld();




//
static int changed=0;
static struct mathnode* node=0;

//
static char* datahome=0;
static double scale;
static double centerx;
static double centery;
		//datahome 里面存放计算得到的值的符号
		//scale=“屏幕”上两个点对于“世界”上的距离
		//centerxy = “屏幕”对应“世界”哪个点

//
static int count=0;
static char buffer[128];

//
static char infix[128];
static char postfix[128];
static char result[128];




//得到haha：1<haha<10
//得到counter：十的多少次方
void kexuejishufa(double* haha,int* counter)
{
	if( (*haha) < 1.00 )
	{
		while( (*haha) < 1.00 )
		{
			(*haha) *= 10.00;
			(*counter) --;
		}
	}
	else
	{
		while( (*haha) > 10.00 )
		{
			(*haha) /= 10.00;
			(*counter) ++;
		}
	}
}
//
void printcalc()
{
	int x,y,value,counter;
	double first,second,haha;

	int wanggex,wanggey,wanggedistance;		//只用在"画网格这一步"
	DWORD* screenbuf=(DWORD*)screendata();




	//跳过
	if(node==0)goto skipthese;
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
		//背景
		background3();




		//准备"画网格",必须确定3个量：哪个点，间距
		//wantge:x,y,size = 屏幕上的某个交汇点的x,	交汇点的y,	两行的间距
		haha=scale;
		counter=0;
		kexuejishufa(&haha,&counter);
		wanggedistance=(int)(250.00/haha);

		wanggex=512 % wanggedistance;
		wanggey=384 % wanggedistance;




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
		printdouble(wanggex/8,wanggey/16,1.2233);




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
			value=(767-y)<<10;
			for(x=1;x<1023;x++)
			{
				counter=0;
				if( datahome[ value-1025 + x ] > 0 )counter--;
				else counter++;

				if( datahome[ value-1023 + x ] > 0 )counter--;
				else counter++;

				if( datahome[ value+1023 + x ] > 0 )counter--;
				else counter++;

				if( datahome[ value+1025 + x ] > 0 )counter--;
				else counter++;

				//上下左右四点符号完全一样，说明没有点穿过
				if( (counter!=4) && (counter!=-4) )screenbuf[y*1024+x]=0xffffffff;		//否则白色
			}
		}//result2img

	}//else




	//打印
skipthese:
	string(0,0,buffer);
	string(0,1,infix);
	string(0,2,postfix);
	string(0,3,result);
}








void f3show()
{
	/*
	if math->printcalc
	if filesystem->printfilesystem
	if netpackage->printnet
	if mmio->printmmio
	if what->printwhat
	if ...->...
	*/
	printcalc();
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
		diary("%d,%d\n",dx,dy);

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
		datahome=(char*)whereisworld()+0x300000;
		centerx=0.00;
		centery=0.00;
		scale=1.00;
	}
	else	//不是第一次进来了
	{
		cleanscreen();
	}
}
