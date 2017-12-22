#include "actor.h"
double calculator(void* postfix, u64 x, u64 y);
double sketchpad(void*, double, double);
double closestvalue(double first, double second);
void kexuejishufa(double* haha, int* counter);
void postfix2binarytree(void* postfix, void* out);
void infix2postfix(void* infix, void* postfix);



//
struct mathnode
{
	u16 left;
	u16 right;
	u16 up;
	u16 type;
	union
	{
		double floatpoint;
		unsigned long long integer;
	};
};
static struct mathnode* node=0;

//
static int count=0;
static u8 buffer[128];

//
static u8 infix[128];
static u8 postfix[128];
static u8 result[128];
static u8* databuf=0;

//
static double scale=0.0;
static double centerx=0.0;
static double centery=0.0;
		//databuf 里面存放计算得到的值的符号
		//scale=“屏幕”上两个点对于“世界”上的距离
		//centerxy = “屏幕”对应“世界”哪个点




//
static void wangge(struct arena* win)
{
	int temp;
	int x,y;
	int wanggex,wanggey,wanggedistance;		//只用在"画网格这一步"
	double first,second,res;

	u32* buf = (u32*)(win->buf);
	int width = win->w;
	int height= win->h;

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
	drawdouble(win, 0xffffff,
		wanggex, 0+wanggey,
		centerx-(scale*width/2)+(wanggex*scale)
	);
	drawdouble(win, 0xffffff,
		wanggex, 16+wanggey,
		centery+(scale*height/2)-(wanggey*scale)
	);

	for(x=wanggex;x<width;x+=wanggedistance)
	{//竖线
		for(y=0;y<height;y++)
		{
			buf[y*width+x]=0x44444444;
		}
	}//竖线

	for(y=wanggey;y<height;y+=wanggedistance)
	{//横线
		for(x=0;x<width;x++)
		{
			buf[y*width+x]=0x44444444;
		}
	}//横线

}
static void tuxiang(struct arena* win)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = win->w;
	int height = win->h;
	u32* buf = (u32*)(win->buf);




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
				buf[y*width+x]=0xffffffff;
			}
		}
	}//result2img
}
static void sketchpad_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	wangge(win);
	tuxiang(win);

skipthese:
	drawstring(win, 0xcccccc, 0, 0, buffer, 0);
	drawstring(win, 0xcccccc, 0, 16, infix, 0);
	drawstring(win, 0xcccccc, 0, 32, postfix, 0);
	drawstring(win, 0xcccccc, 0, 48, result, 0);
}
static void sketchpad_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void sketchpad_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void sketchpad_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);
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
static void sketchpad_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("sketchpad(%x,%x,%x)\n",win,act,sty);
}
static void sketchpad_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)sketchpad_read_cli(win, act, sty);
	else if(fmt == __tui__)sketchpad_read_tui(win, act, sty);
	else if(fmt == __vbo__)sketchpad_read_vbo(win, act, sty);
	else if(fmt == __html__)sketchpad_read_html(win, act, sty);
	else sketchpad_read_pixel(win, act, sty);
}




static void sketchpad_write(struct event* ev)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == __kbd__)		//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
			centerx += scale*10;
		}
		else if(key==0x27)		//right	0x4d
		{
			centerx -= scale*10;
		}
		else if(key==0x26)		//up	0x4b
		{
			centery -= scale*10;
		}
		else if(key==0x28)		//down	0x4d
		{
			centery += scale*10;
		}
	}
	else if(type == __char__)	//'char'
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
	else if(type==0x2b70)		//p+
	{
		int k = (key>>48)&0xffff;
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
		x = x - 0x8000;
		y = 0x8000 - y;
		if(k == 'f')	//front
		{
			//centerx+scale*pointx = x = newcenterx+scale/1.2*pointx
			//newcenterx=centerx+scale*pointx*(1-1/1.2)
			centerx += scale * (x*512.0/65536) * (1-1/1.2);
			centery += scale * (y*512.0/65536) * (1-1/1.2);

			scale /= 1.2;
		}
		else if(k == 'b')	//back
		{
			centerx += scale * (x*512.0/65536) * (-0.2);
			centery += scale * (y*512.0/65536) * (-0.2);

			scale *= 1.2;
		}
	}
	else if(type==0x4070)		//p@
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(type==0x2d70)		//p-
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
}




static void sketchpad_list()
{
}
static void sketchpad_change()
{
}
static void sketchpad_start()
{
	centerx=0.00;
	centery=0.00;
	scale=1.00;
}
static void sketchpad_stop()
{
}
void sketchpad_create(void* base,void* addr)
{
	struct actor* p = addr;
	node = (struct mathnode*)(base+0x200000);
	databuf = base+0x300000;

	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('s', 'k', 'e', 't', 'c', 'h', 0, 0);

	p->start = (void*)sketchpad_start;
	p->stop = (void*)sketchpad_stop;
	p->list = (void*)sketchpad_list;
	p->choose = (void*)sketchpad_change;
	p->read = (void*)sketchpad_read;
	p->write = (void*)sketchpad_write;
}
void sketchpad_delete()
{
}
