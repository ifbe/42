#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printstring(int x, int y, int size, char* str, u32 fgcolor, u32 bgcolor);
void defaultdouble(int x,int y,double z);
void decimal(int x,int y,u64 in);
void backgroundcolor(u32);
void rectangle(u32 x1y1z1,u32 x2y2z2,u32 color);
//
double calculator(char* postfix, u64 x, u64 y);
double sketchpad(void*, double, double);
double closestvalue(double first,double second);
void double2decimalstring(double,char*);
void kexuejishufa(double* haha,int* counter);
void postfix2binarytree(char* postfix,void* out);
void infix2postfix(char* infix,char* postfix);
//
void printmemory(char*,int);
void say(char*,...);




static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 pixelbuffer;
	u64 pixelformat;
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
static struct mathnode* node=0;

//
static int changed=0;
static int count=0;
static char buffer[128];

//
static char infix[128];
static char postfix[128];
static char result[128];
static char* databuf=0;

//
static double scale=0.0;
static double centerx=0.0;
static double centery=0.0;
		//databuf 里面存放计算得到的值的符号
		//scale=“屏幕”上两个点对于“世界”上的距离
		//centerxy = “屏幕”对应“世界”哪个点




//
static void wangge()
{
	int temp;
	int x,y;
	int wanggex,wanggey,wanggedistance;		//只用在"画网格这一步"
	double first,second,res;

	u32* screenbuf = (u32*)(haha->pixelbuffer);
	int width = haha->width;
	int height= haha->height;

	//算屏上两行的间距，交点横坐标，纵坐标
	temp = 0;
	res = scale;	//scale=a*(10^b)	->	distance=250/a(大于25，小于250)
	kexuejishufa( &res , &temp );
	wanggedistance=(int)( 250.00 / res );


	first = centerx - scale * (width/2);
	second = centerx + scale * (width/2);
	res = closestvalue(first, second);
	res = (res-first) / (second-first) * width;

	wanggex = ( (int)res ) % wanggedistance;
	if( (wanggex>width-1) | (wanggex<0) )return;


	first = centery - scale * (height/2);
	second = centery + scale * (height/2);
	res = closestvalue(first, second);
	res = (second-res) / (second-first) * width;

	wanggey = ( (int)res ) % wanggedistance;
	if( (wanggey>height-1) | (wanggey<0) )return;




	//网格上对应那一行的x,y坐标值,以及画上网格
	defaultdouble( wanggex, 0+wanggey, centerx-(scale*width/2)+(wanggex*scale) );
	defaultdouble( wanggex, 16+wanggey, centery+(scale*height/2)-(wanggey*scale) );

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
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = haha->width;
	int height = haha->height;
	u32* screenbuf = (u32*)(haha->pixelbuffer);




	//带入75万个坐标算结果
	//逻辑(0,0)->(centerx,centery),,,,(1023,767)->(centerx+scale*1023,centery+scale*767)
	for(y=0;y<height;y++)		//只算符号并且保存
	{
		ry = centery + (y - (height/2))*scale;
		for(x=0;x<width;x++)
		{
			rx = centerx + (x - (width/2))*scale;
			hello=sketchpad(node, rx, ry);

			if(hello>0)databuf[width*y+x]=1;
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








static void sketchpad_read_pixel()
{
	double hello;

	//跳过
	if(node[0].type!=0x3d3d3d3d)goto skipthese;
	if(changed==0)goto skipthese;
	changed=0;



	backgroundcolor(0);
	if(node[0].integer == 0)
	{
		//计算器
		hello=calculator(postfix,0,0);
		double2decimalstring(hello,result);
	}
	else
	{
		//网格，函数图
		wangge();
		tuxiang();
	}//else





skipthese:		//打印
	printstring(0, 0, 1, buffer, 0xcccccc, 0xff000000);
	printstring(0, 16, 1, infix, 0xcccccc, 0xff000000);
	printstring(0, 32, 1, postfix, 0xcccccc, 0xff000000);
	printstring(0, 48, 1, result, 0xcccccc, 0xff000000);
	return;
}
static void sketchpad_read_html()
{
	u32* screenbuf = (u32*)(haha->pixelbuffer);

	sketchpad_read_pixel();
	screenbuf[0]=0;
}
static void sketchpad_read_text()
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width=haha->width;
	int height=haha->height;
	u8* p = (u8*)(haha->pixelbuffer);
	if(node[0].type!=0x3d3d3d3d)return;

	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<height;y++)
	{
		ry = centery + (y - (height/2))*scale;
		for(x=0;x<width;x++)
		{
			rx = centerx + (x-width)/2*scale;
			hello = sketchpad(node, rx, ry);

			if(hello>0)databuf[width*y+x]=1;
			else databuf[width*y+x]=-1;
		}
	}
	for(y=1;y<height-1;y++)
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

			if( (counter!=4) && (counter!=-4) )
			{
				p[(y*width+x)<<2] = 0x35+counter;
			}
		}
	}

	//
	for(x=0;x<count;x++)
	{
		p[x*4] = buffer[x];
	}
}
static void sketchpad_read()
{
	u32 temp = (haha->pixelformat)&0xffffffff;

	//text
	if(temp == 0x74786574)
	{
		sketchpad_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		sketchpad_read_html();
	}

	//pixel
	else
	{
		sketchpad_read_pixel();
	}
}




static void sketchpad_write(u64* who, u64* a, u64* b)
{
	int ret;
	int width = haha->width;
	int height = haha->height;
	u64 type = *a;
	u64 key = *b;

	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			centerx += scale*10;
			changed=1;
		}
		else if(key==0x27)		//right	0x4d
		{
			centerx -= scale*10;
			changed=1;
		}
		else if(key==0x26)		//up	0x4b
		{
			centery -= scale*10;
			changed=1;
		}
		else if(key==0x28)		//down	0x4d
		{
			centery += scale*10;
			changed=1;
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==0x8)			//backspace
		{
			if(count == 0)return;

			count--;
			buffer[count] = 0x20;
		}
		else if(key==0xd)		//enter
		{
			//清空输入区
			for(ret=0;ret<count;ret++)
			{
				infix[ret] = buffer[ret];
				buffer[ret] = 0x20;
			}
			infix[count] = 0;
			count = 0;
			say("infix:%s\n", infix);

			//
			infix2postfix(infix, postfix);
			say("postfix:%s\n", postfix);

			//
			postfix2binarytree(postfix, node);
			say("node:%x,%x\n", node[0].left, node[0].right);

			//告诉打印员
			changed=1;
		}
		else
		{
			if(count<128)
			{
				buffer[count] = key;
				count++;
			}
		}
	}
	else if(type==0x406d)		//'xyz move'
	{
		int dx=(int)(short)(key&0xffff);
		int dy=(int)(short)((key>>16)&0xffff);
		//say("%d,%d\n",dx,dy);

		centerx -= scale*dx;
		centery += scale*dy;
		changed=1;
	}
	else if(type==0x2b6d)
	{
		key >>= 48;
		if(key == 4)	//front
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
		else if(key == 5)	//back
		{
			int x,y;
			x=(key&0xffff) - (width/2);
			y=(height/2) - ( (key>>16) & 0xffff );
			centerx += ((double)x) * scale * (-0.2);
			centery += ((double)y) * scale * (-0.2);

			scale*=1.2;
			changed=1;
		}
	}//2d6d
}




static void sketchpad_list()
{
}
static void sketchpad_change()
{
}
static void sketchpad_start()
{
	u64 haha = 0x72616863;
	u64 hehe = 0xd;
	buffer[0] = 'y';
	buffer[1] = '=';
	buffer[2] = 'x';
	count = 3;
	sketchpad_write((void*)0, &haha, &hehe);

	//
	centerx=0.00;
	centery=0.00;
	scale=1.00;
	backgroundcolor(0);
}
static void sketchpad_stop()
{
}
void sketchpad_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x686374656b73;

	this[10]=(u64)sketchpad_start;
	this[11]=(u64)sketchpad_stop;
	this[12]=(u64)sketchpad_list;
	this[13]=(u64)sketchpad_change;
	this[14]=(u64)sketchpad_read;
	this[15]=(u64)sketchpad_write;

	node=(struct mathnode*)(base+0x200000);
	databuf=base+0x300000;
}
void sketchpad_delete()
{
}
