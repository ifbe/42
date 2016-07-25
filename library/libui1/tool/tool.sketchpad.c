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
double sketchpad(struct mathnode*,double,double);
double calculator(char* postfix,double,double);
double beautifulbetween(double first,double second);
void postfix2binarytree(char* postfix,struct mathnode* out);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);
void kexuejishufa(double* haha,int* counter);
//
void printstring(char* str,DWORD xyz,DWORD fgcolor,DWORD bgcolor);
void defaultdouble(int x,int y,double z);
void decimal(int x,int y,QWORD in);
void backgroundcolor(DWORD);
void rectangle(DWORD x1y1z1,DWORD x2y2z2,DWORD color);
//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//
void printmemory(char*,int);
void say(char*,...);




//
static int changed=0;
static int count=0;
static char buffer[128];

//
static char infix[128];
static char postfix[128];
static char result[128];
static char* databuf=0;
static struct mathnode* node=0;

//
static double scale=0.0;
static double centerx=0.0;
static double centery=0.0;
		//databuf 里面存放计算得到的值的符号
		//scale=“屏幕”上两个点对于“世界”上的距离
		//centerxy = “屏幕”对应“世界”哪个点
//
static DWORD* screenbuf=0;
static int width=0;
static int height=0;




//
static void wangge()
{
	int x,y,temp;
	double first,second,res;
	int wanggex,wanggey,wanggedistance;		//只用在"画网格这一步"




	//算屏上两行的间距，交点横坐标，纵坐标
	temp = 0;
	res = scale;	//scale=a*(10^b)	->	distance=250/a(大于25，小于250)
	kexuejishufa( &res , &temp );
	wanggedistance=(int)( 250.00 / res );


	first = centerx - scale * (width/2);
	second = centerx + scale * (width/2);
	res=beautifulbetween( first , second );
	res=(res-first) / (second-first) * width;

	wanggex = ( (int)res ) % wanggedistance;
	if( (wanggex>width-1) | (wanggex<0) )return;


	first = centery - scale * (height/2);
	second = centery + scale * (height/2);
	res=beautifulbetween( first , second );
	res=(second-res) / (second-first) * width;

	wanggey = ( (int)res ) % wanggedistance;
	if( (wanggey>height-1) | (wanggey<0) )return;




	//网格上对应那一行的x,y坐标值,以及画上网格
	defaultdouble( wanggex/8, 0+wanggey/16, centerx-(scale*width/2)+(wanggex*scale) );
	defaultdouble( wanggex/8, 1+wanggey/16, centery+(scale*height/2)-(wanggey*scale) );

	for(x=wanggex;x<width;x+=wanggedistance)
	{//竖线
		for(y=0;y<height;y++)
		{
			screenbuf[y*width+x]=0x44444444;
		}
	}//竖线

	for(y=wanggey;y<height;y+=wanggedistance)
	{//横线
		for(x=0;x<width;x++)
		{
			screenbuf[y*width+x]=0x44444444;
		}
	}//横线

}
static void tuxiang()
{
	int x,y;
	int value1,value2,counter;
	double first,second,haha;




	//带入75万个坐标算结果
	//逻辑(0,0)->(centerx,centery),,,,(1023,767)->(centerx+scale*1023,centery+scale*767)
	for(y=0;y<height;y++)		//只算符号并且保存
	{
		second=centery + (y - (height/2))*scale;
		for(x=0;x<width;x++)
		{
			first=centerx + (x - (width/2))*scale;
			haha=sketchpad(node,first,second);

			if(haha>0)databuf[width*y+x]=1;
			else databuf[width*y+x]=-1;
		}
	}//calculate results




	//由算出的结果得到图像
	//屏幕(0,767)->data[(767-767)*1024+0],,,,(1023,0)->data[(767-0)*1024+1023]
	for(y=1;y<height-1;y++)		//边缘四个点确定中心那一点有没有
	{
		value1=(height-1-y)*width;
		for(x=1;x<width-1;x++)
		{
			counter=0;
			if( databuf[ value1-width-1 + x ] == 1 )counter++;
			else counter--;

			if( databuf[ value1-width+1 + x ] == 1 )counter++;
			else counter--;

			if( databuf[ value1+width-1 + x ] == 1 )counter++;
			else counter--;

			if( databuf[ value1+width+1 + x ] == 1 )counter++;
			else counter--;

			//say("@%d,%d:%d\n",x,y,counter);
			//上下左右四点符号完全一样，说明没有点穿过//否则白色
			if( (counter!=4) && (counter!=-4) )
			{
				screenbuf[y*width+x]=0xffffffff;
			}
		}
	}//result2img
}








static void sketchpad_write(QWORD type,QWORD key)
{
	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			centerx += scale*100;
			changed=1;
		}
		else if(key==0x27)		//right	0x4d
		{
			centerx -= scale*100;
			changed=1;
		}
		else if(key==0x26)		//up	0x4b
		{
			centery -= scale*100;
			changed=1;
		}
		else if(key==0x28)		//down	0x4d
		{
			centery += scale*100;
			changed=1;
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==0x8)			//backspace
		{
			if(count!=0)count--;
			buffer[count]=0x20;
		}
		else if(key==0xd)		//enter
		{
			//检查buffer，然后给infix
			say("buffer:%s\n",buffer);

			//清空输入区
			for(count=0;count<127;count++)
			{
				infix[count]=buffer[count];
				buffer[count]=0x20;
			}
			count=0;

			say("infix2postfix:%s\n",postfix);
			infix2postfix(infix,postfix);

			say("postfix2binarytree......\n");
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
		//say("%d,%d\n",dx,dy);

		centerx -= scale*dx;
		centery += scale*dy;
		changed=1;
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		//保证鼠标之前指着哪儿(x,y)，之后就指着哪儿(x,y)
		//centerx+scale*pointx = x = newcenterx+scale/1.2*pointx -> newcenterx=centerx+scale*pointx*(1-1/1.2)
		int x,y;
		x=(key&0xffff) - (width/2);
		y=(height/2) - ( (key>>16) & 0xffff );
		centerx += ((double)x) * scale * (1-1/1.2);
		centery += ((double)y) * scale * (1-1/1.2);
		//say("%d,%lf\n",x,centerx);

		scale/=1.2;
		changed=1;
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		int x,y;
		x=(key&0xffff) - (width/2);
		y=(height/2) - ( (key>>16) & 0xffff );
		centerx += ((double)x) * scale * (-0.2);
		centery += ((double)y) * scale * (-0.2);

		scale*=1.2;
		changed=1;
	}
	//else if(type==0x6B636162207A7978)		//'xyz ++++'
	//else if(type==0x6B636162207A7978)		//'xyz ----'
}
static void sketchpad_read()
{
	//跳过
	if(node[0].type!=0x3d3d3d3d)goto skipthese;
	if(changed==0)goto skipthese;
	changed=0;



	backgroundcolor(0);
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
	printstring(buffer,  0 + ( 0<<8), 0xcccccc , 0 );
	printstring(infix,   0 + (16<<8), 0xcccccc , 0 );
	printstring(postfix, 0 + (32<<8), 0xcccccc , 0 );
	printstring(result,  0 + (48<<8), 0xcccccc , 0 );
	return;
}




static void sketchpad_list()
{
}
static void sketchpad_change()
{
	//
	centerx=0.00;
	centery=0.00;
	scale=1.00;

	//
	buffer[0]='y';
	buffer[1]='=';
	buffer[2]='x';
	sketchpad_write(0x72616863, 0xd);
}








static void sketchpad_start(QWORD size,void* addr)
{
	int x;

	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	screenbuf=addr;

	for(x=0;x<width*height;x++)
	{
		screenbuf[x]=0;
	}
}
static void sketchpad_stop()
{
}
void sketchpad_init(void* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x686374656b73;
	this[2]=(QWORD)sketchpad_start;
	this[3]=(QWORD)sketchpad_stop;
	this[4]=(QWORD)sketchpad_list;
	this[5]=(QWORD)sketchpad_change;
	this[6]=(QWORD)sketchpad_read;
	this[7]=(QWORD)sketchpad_write;

	node=(struct mathnode*)(base+0x100000);
	databuf=base+0x200000;
}
void sketchpad_kill()
{
}
