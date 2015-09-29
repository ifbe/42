#define QWORD unsigned long long
#define DWORD unsigned int




QWORD screendata();
QWORD screenresolution();
//void point(int x,int y,DWORD color);




/*
void point(int x,int y,int color)
{
	DWORD* screenbuf=(DWORD*)screendata();
	screenbuf[ x + (y<<10) ] = color;
}
void draw(int x,int y,int color)
{
	DWORD* screenbuf=(DWORD*)screendata();
	screenbuf[ (x+512) + ((767-y)<<10) ] = color;
}*/




void line(QWORD y1x1,QWORD y2x2,DWORD color)
{
	int x,y,temp;
	int x1,y1,x2,y2;
	DWORD* screenbuf=(DWORD*)screendata();

	x1=y1x1&0xffff;
	y1=(y1x1>>16)&0xffff;
	x2=y2x2&0xffff;
	y2=(y2x2>>16)&0xffff;

	if(x1<0)return;
	if(x1>1024)return;
	if(x2<0)return;
	if(x2>1024)return;
	if(y1<0)return;
	if(y1>767)return;
	if(y2<0)return;
	if(y2>767)return;




	if(x1 == x2)
	{
		if(y1<=y2){y=y1;temp=y2;}
		else{y=y2;temp=y1;}

		for(;y<=temp;y++)
		{
			screenbuf[ (y<<10) + x1 ] = color;
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
			if(y<640) screenbuf[ (y<<10) + x ] = color;
			else screenbuf[ x ] = color;
		}
	}
}




void rectangle(QWORD y1x1,QWORD y2x2,DWORD color)
{
	int x,y;
	int x1,y1,x2,y2;
	int startx,endx,starty,endy;
	DWORD* screenbuf=(DWORD*)screendata();




	x1=y1x1&0xffff;
	y1=(y1x1>>16)&0xffff;
	x2=y2x2&0xffff;
	y2=(y2x2>>16)&0xffff;

	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}




	for(y=starty;y<endy;y++)
	{
		for(x=startx;x<endx;x++)
		{
			screenbuf[ (y<<10) + x ] = color;		//0x53840273;
		}
	}
}