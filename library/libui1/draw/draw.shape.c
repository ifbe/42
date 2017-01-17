#define u64 unsigned long long
#define u32 unsigned int
double squareroot(double);
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




void linesegment(int x1, int y1, int x2, int y2, u32 color)
{
}
void halfline(int x1, int y1, int x2, int y2, u32 color)
{
}
void line(int x1, int y1, int x2, int y2, u32 color)
{
	int temp;
	int x,y;
	int width,height;
	u32* winbuf;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;
	color |= 0xff000000;

	if(x1<0)x1=0;
	if(x1>width)x1=width-1;
	if(x2<0)x2=0;
	if(x2>width)x2=width-1;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;
//say("(%d,%d)->(%d,%d)\n",x1,y1,x2,y2);




	if(x1 == x2)
	{
		if(y1<=y2){y=y1;temp=y2;}
		else{y=y2;temp=y1;}

		for(;y<=temp;y++)
		{
			winbuf[ (y*width) + x1 ] = color;
		}
	}

	else
	{
		double dx;
		double k=(double)(y1-y2) / (double)(x1-x2);
		if(x1<x2){x=x1;temp=x2;}
		else{x=x2;temp=x1;}

		for(;x<temp;x++)
		{
			dx=(double)(x-x1);
			y=y1+ (int)(k*dx);
//say("%d,%d\n",x,y);
			if(y<height)
			{
				winbuf[ (y*width) + x ] = color;
			}
			else say("wrong\n");
		}
	}
}




void rectframe(int x1, int y1, int x2, int y2, u32 color)
{
	int t;
	int x,y;
	int width,height;
	int startx,endx,starty,endy;
	u32* winbuf;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;
	color |= 0xff000000;

	if(x1<x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}
	//say("(%x,%x),(%x,%x)\n",startx,starty,endx,endy);

	for(t=0;t<1;t++)
	{
		for(x=startx;x<endx;x++)winbuf[((starty+t)*width) + x] = color;
		for(x=startx;x<endx;x++)winbuf[((endy-t)*width) + x] = color;
		for(y=starty;y<endy;y++)winbuf[(y*width) + startx+t] = color;
		for(y=starty;y<endy;y++)winbuf[(y*width) + endx-t] = color;
	}
}
void rectbody(int x1, int y1, int x2, int y2, u32 color)
{
	int x,y;
	int width,height;
	int startx,endx,starty,endy;
	u32* winbuf;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;
	color |= 0xff000000;

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}

	for(y=starty;y<=endy;y++)
	{
		for(x=startx;x<=endx;x++)
		{
			winbuf[ (y*width) + x ] = color;
		}
	}
}
void rect(int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor)
{
	rectbody(x1, y1, x2, y2, bodycolor);
	rectframe(x1, y1, x2, y2, framecolor);
}




void trianglebody(int x1, int y1, int x2, int y2, int x3, int y3, u32 color)
{
}
void triangleframe(int x1, int y1, int x2, int y2, int x3, int y3, u32 color)
{
	line(x1, y1, x2, y2, color);
	line(x1, y1, x3, y3, color);
	line(x2, y2, x3, y3, color);
}
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, u32 bodycolor, u32 framecolor)
{
	trianglebody(x1, y1, x2, y2, x3, y3, bodycolor);
	triangleframe(x1, y1, x2, y2, x3, y3, framecolor);
}




void circleframe(int cx, int cy, int radius, u32 color)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* winbuf;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;
	color |= 0xff000000;

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		winbuf[ (y*width) + x1 ] = color;
		winbuf[ (y*width) + x2 ] = color;
	}
}
void circlebody(int cx, int cy, int radius, u32 color)
{
	int ret;
	int x,y;
	int x1,x2;
	int y1,y2;
	int width,height;
	u32* winbuf;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;
	color |= 0xff000000;

	y1=cy-radius;
	if(y1<0)y1=0;
	if(y1>=height)y1=height-1;

	y2=cy+radius;
	if(y2<0)y2=0;
	if(y2>=height)y2=height-1;

	for(y=y1;y<=y2;y++)
	{
		ret = (int)squareroot(radius*radius - (y-cy)*(y-cy));

		x1 = cx - ret;
		if(x1<0)x1=0;
		if(x1>=width)x1=width-1;

		x2 = cx + ret;
		if(x2<0)x2=0;
		if(x2>=width)x2=width-1;

		for(x=x1;x<=x2;x++)
		{
			winbuf[ (y*width) + x ] = color;
		}
	}
}




void ovalbody(int x1, int y1, int x2, int y2, u32 color)
{
}
void ovalframe(int x1, int y1, int x2, int y2, u32 color)
{
}
void oval(int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor)
{
	ovalbody(x1, y1, x2, y2, bodycolor);
	ovalbody(x1, y1, x2, y2, framecolor);
}




void sectorbody(int cx, int cy, int radius, int start, int end, u32 color)
{
}
void sectorframe(int cx, int cy, int radius, int start, int end, u32 color)
{
}
void sector(int cx, int cy, int radius, int start, int end, u32 bodycolor, u32 framecolor)
{
}




void bezier(int ax, int ay, int bx, int by, int cx, int cy, u32 color)
{
	u32* winbuf;
	int width;
	int height;
	int x,y,t;

	winbuf=(u32*)(haha->pixelbuffer);
	width=haha->width;
	height=haha->height;

	for(t=0;t<1000;t++)
	{
		x = (1000-t)*(1000-t)*ax + 2*t*(1000-t)*cx + t*t*bx;
		x /= 1000*1000;
		if(x<0|x>=width)continue;

		y = (1000-t)*(1000-t)*ay + 2*t*(1000-t)*cy + t*t*by;
		y /= 1000*1000;
		if(y<0|y>=height)continue;

		winbuf[y*width + x] = 0xff00;
	}
}




void shape_start()
{
}
void shape_stop()
{
}
void shape_create(void* home,void* me)
{
	haha=me;
	haha->type=0;
	haha->id=0x6570616873;
}
void shape_delete()
{
}
