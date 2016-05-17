#define QWORD unsigned long long
#define DWORD unsigned int




static DWORD* winbuf=0;
static int width=0;
static int height=0;




void point(int x,int y,int color)
{
	winbuf[ x + (y<<10) ] = color;
}
void draw(int x,int y,int color)
{
	winbuf[ (x+512) + ((383-y)<<10) ] = color;
}




void line(QWORD z1y1x1,QWORD z2y2x2,DWORD color)
{
	int x,y,temp;
	int x1,y1,x2,y2;

	x1=z1y1x1&0xffff;
	y1=(z1y1x1>>16)&0xffff;
	x2=z2y2x2&0xffff;
	y2=(z2y2x2>>16)&0xffff;

	if(x1<0)return;
	if(x1>width)return;
	if(x2<0)return;
	if(x2>width)return;
	if(y1<0)return;
	if(y1>height-1)return;
	if(y2<0)return;
	if(y2>height-1)return;




	if(x1 == x2)
	{
		if(y1<=y2){y=y1;temp=y2;}
		else{y=y2;temp=y1;}

		for(;y<=temp;y++)
		{
			winbuf[ (y<<10) + x1 ] = color;
		}
	}

	else
	{
		double dx;
		double k=(double)(y1-y2) / (double)(x1-x2);
		if(x1<=x2){x=x1;temp=x2;}
		if(x1>x2){x=x2;temp=x1;}

		for(;x<temp;x++)
		{
			dx=(double)(x-x1);
			y=y1+ (DWORD)(k*dx);
			if(y<height) winbuf[ (y*width) + x ] = color;
			else winbuf[ x ] = color;
		}
	}
}




void squareframe(QWORD z1y1x1,QWORD z2y2x2,DWORD color)
{
	int t;
	int x,y;
	int x1,y1,x2,y2;
	int startx,endx,starty,endy;

	x1=z1y1x1&0xffff;
	y1=(z1y1x1>>16)&0xffff;
	x2=z2y2x2&0xffff;
	y2=(z2y2x2>>16)&0xffff;

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




void rectangle(QWORD z1y1x1,QWORD z2y2x2,DWORD color)
{
	int x,y;
	int x1,y1,x2,y2;
	int startx,endx,starty,endy;

	x1=z1y1x1&0xffff;
	y1=(z1y1x1>>16)&0xffff;
	x2=z2y2x2&0xffff;
	y2=(z2y2x2>>16)&0xffff;

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}

	for(y=starty;y<endy;y++)
	{
		for(x=startx;x<endx;x++)
		{
			winbuf[ (y*width) + x ] = color;		//0x53840273;
		}
	}
}




void shape_start(DWORD size,void* addr)
{
say(">>>>%x\n",size);
	//how
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//where
	winbuf=addr;
}
void shape_stop()
{
}
void shape_init()
{
}
void shape_kill()
{
}
