#include "libuser.h"
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
		u64 integer;
		double floatpoint;
	};
};
static struct mathnode node[0x100];

//
static u8 buffer[128];
static int count=0;
//
static u8 infix[128];
static u8 postfix[128];
static u8 result[128];
//
static u8* databuf=0;
static double centerx = 0.0;
static double centery = 0.0;
static double scale = 0.0;




/*
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
*/
static void tuxiang(struct arena* win)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = win->stride;
	int height = win->height;
	u32* buf = (u32*)(win->buf);




	//带入75万个坐标算结果
	//逻辑(0,0)->(centerx,centery),,,,(1023,767)->(centerx+scale*1023,centery+scale*767)
	for(y=0;y<height;y++)		//只算符号并且保存
	{
		ry = centery + (y - (height/2))*scale;
		for(x=0;x<width;x++)
		{
			rx = centerx + (x - (width/2))*scale;
			hello = sketchpad(node, rx, ry);

			if(hello > 0.0)databuf[width*y+x]=1;
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
static void sketchpad_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	double rx,ry,rw;
	int x,y,w,counter;
	int cx, cy, ww, hh;
	int stride = win->stride;
	u32* buf = (u32*)(win->buf);

	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	for(y=0;y<hh*2;y++)
	{
		ry = centery + (y-hh)*scale;
		for(x=0;x<ww*2;x++)
		{
			rx = centerx + (x-ww)*scale;
			rw = sketchpad(node, rx, ry);

			if(rw > 0)w = 1;
			else w = -1;
			databuf[2*ww*y+x] = w;
		}
	}

	for(y=1;y<2*hh-1;y++)
	{
		w = (2*hh-1-y)*ww*2;
		for(x=1;x<2*ww-1;x++)
		{
//say("%d,%d\n",x,y);
			counter=0;

			if(databuf[w+x - 1] == 1)counter++;
			else counter--;

			if(databuf[w+x + 1] == 1)counter++;
			else counter--;

			if(databuf[w+x - ww*2] == 1)counter++;
			else counter--;

			if(databuf[w+x + ww*2] == 1)counter++;
			else counter--;

			if( (counter!=4) && (counter!=-4) )
			{
				buf[(cy-hh+y)*stride + (cx-ww+x)]=0xffffffff;
			}
		}
	}//result2img

skipthese:
	drawstring(win, 0xcccccc, cx-ww, cy-hh+ 0, buffer, 0);
	drawstring(win, 0xcccccc, cx-ww, cy-hh+16, infix, 0);
	drawstring(win, 0xcccccc, cx-ww, cy-hh+32, postfix, 0);
	drawstring(win, 0xcccccc, cx-ww, cy-hh+48, result, 0);
}
static void sketchpad_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sketchpad_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sketchpad_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"sketch\" style=\"width:50%%;height:100px;float:left;background-color:#e712a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void sketchpad_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = win->stride;
	int height = win->height;
	u8* p = (u8*)(win->buf);

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
static void sketchpad_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("sketchpad(%x,%x,%x)\n",win,act,sty);
}
static void sketchpad_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sketchpad_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)sketchpad_read_tui(win, sty, act, pin);
	else if(fmt == _html_)sketchpad_read_html(win, sty, act, pin);
	else if(fmt == _json_)sketchpad_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)sketchpad_read_vbo(win, sty, act, pin);
	else sketchpad_read_pixel(win, sty, act, pin);
}




static void sketchpad_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == _kbd_)		//'kbd'
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
	else if(type == _char_)	//'char'
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
		if(k == 'f')	//front
		{
			//centerx+scale*pointx = x = newcenterx+scale/1.2*pointx
			//newcenterx=centerx+scale*pointx*(1-1/1.2)
			centerx += scale * x * (1-1/1.2);
			centery += scale * y * (1-1/1.2);
			scale /= 1.2;
		}
		else if(k == 'b')	//back
		{
			centerx += scale * x * (-0.2);
			centery += scale * y * (-0.2);
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
static void sketchpad_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sketchpad_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	centerx = 0.00;
	centery = 0.00;
	scale = 1.00;
}
static void sketchpad_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void sketchpad_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = databuf = memorycreate(0x100000);
}




void sketchpad_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'k', 'e', 't', 'c', 'h', 0, 0);

	p->oncreate = (void*)sketchpad_create;
	p->ondelete = (void*)sketchpad_delete;
	p->onstart  = (void*)sketchpad_start;
	p->onstop   = (void*)sketchpad_stop;
	p->onlist   = (void*)sketchpad_list;
	p->onchoose = (void*)sketchpad_change;
	p->onread   = (void*)sketchpad_read;
	p->onwrite  = (void*)sketchpad_write;
}
