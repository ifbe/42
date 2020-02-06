#include "libuser.h"
#define CTXBUF buf0
#define DATBUF buf1
int copypath(u8* path, u8* data);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);
//
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
static double centerx = 0.0;
static double centery = 0.0;
static double scale = 0.0;




static void sketchpad_ctxforwnd(struct glsrc* src, char* vs, char* fs)
{
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = memorycreate(0x1000, 0);
	openreadclose(vs, 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose(fs, 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}
static void sketchpad_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	float cx = slot->f.vc[0];
	float cy = slot->f.vc[1];
	float dx = slot->f.vr[0];
	float dy = slot->f.vf[1] = dx * vec3_getlen(vf) / vec3_getlen(vr);
	float x0 = cx-dx;
	float y0 = cy-dy;
	float xn = cx+dx;
	float yn = cy+dy;

	struct glsrc* src = act->CTXBUF;
	if(0 == src)return;
	float (*vbuf)[6] = src->vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = x0;
	vbuf[0][4] = y0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = xn;
	vbuf[1][4] = yn;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = x0;
	vbuf[2][4] = yn;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = xn;
	vbuf[3][4] = yn;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = x0;
	vbuf[4][4] = y0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = xn;
	vbuf[5][4] = y0;

	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', act->CTXBUF, 1);
}
/*
static void wangge(struct entity* win)
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
static void tuxiang(struct entity* win)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;

	int width = win->stride;
	int height = win->height;
	u32* buf = win->rawbuf;




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
*/
static void sketchpad_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	double rx,ry,rw;
	int x,y,w,counter;
	int cx, cy, ww, hh;
	int stride = win->stride;
	u32* buf = win->rawbuf;
	u8* databuf = act->DATBUF;

	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
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
static void sketchpad_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sketchpad_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->rawlen;
	u8* buf = win->rawbuf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"sketch\" style=\"width:50%%;height:100px;float:left;background-color:#e712a9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->rawlen = len;
}
static void sketchpad_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x, y;
	int value1, value2, counter;
	double rx, ry, hello;
	u8* databuf = act->DATBUF;

	int width = win->stride;
	int height = win->height;
	u8* p = win->rawbuf;

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
static void sketchpad_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("sketchpad(%x,%x,%x)\n",win,act,sty);
}
static void sketchpad_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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
	}/*
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

			act->target.vr[0] *= 1.1;
			act->target.vf[1] *= 1.1;
		}
		else if(k == 'b')	//back
		{
			centerx += scale * x * (-0.2);
			centery += scale * y * (-0.2);
			scale *= 1.2;

			act->target.vr[0] *= 0.9;
			act->target.vf[1] *= 0.9;
		}
	}*/
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




static void sketchpad_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> sketchpad
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)sketchpad_draw_gl41(act,part, win,geom, wnd,area);
	}
}
static void sketchpad_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//sketchpad_event(act, pin, win, sty, ev, 0);
}
static void sketchpad_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void sketchpad_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void sketchpad_search(struct entity* act)
{
}
static void sketchpad_modify(struct entity* act)
{
}
static void sketchpad_delete(struct entity* act)
{
	if(0 == act)return;
}
static void sketchpad_create(struct entity* act, void* str, int argc, u8** argv)
{
	int j;
	u8 vspath[128];
	u8 fspath[128];
	char* vs = 0;
	char* fs = 0;
	char* stl = 0;
	if(0 == act)return;

	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "vs:", 3)){
			copypath(vspath, argv[j]+3);
			vs = (void*)vspath;
		}
		if(0 == ncmp(argv[j], "fs:", 3)){
			copypath(fspath, argv[j]+3);
			fs = (void*)fspath;
		}
	}
	if(0 == vs)vs = "datafile/shader/sketchpad/vert.glsl";
	if(0 == fs)fs = "datafile/shader/sketchpad/frag.glsl";

	act->CTXBUF = memorycreate(0x200, 0);
	sketchpad_ctxforwnd(act->CTXBUF, vs, fs);

	act->DATBUF = memorycreate(0x100000, 0);
	centerx = 0.00;
	centery = 0.00;
	scale = 1.00;
}




void sketchpad_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'k', 'e', 't', 'c', 'h', 0, 0);

	p->oncreate = (void*)sketchpad_create;
	p->ondelete = (void*)sketchpad_delete;
	p->onsearch = (void*)sketchpad_search;
	p->onmodify = (void*)sketchpad_modify;

	p->onlinkup = (void*)sketchpad_linkup;
	p->ondiscon = (void*)sketchpad_discon;
	p->onread  = (void*)sketchpad_read;
	p->onwrite = (void*)sketchpad_write;
}
