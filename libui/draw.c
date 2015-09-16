#define QWORD unsigned long long
#define DWORD unsigned int




QWORD whereisscreen();
//void point(int x,int y,DWORD color);




/*
void point(int x,int y,int color)
{
	DWORD* screenbuf=(DWORD*)whereisscreen();
	screenbuf[ x + (y<<10) ] = color;
}
void draw(int x,int y,int color)
{
	DWORD* screenbuf=(DWORD*)whereisscreen();
	screenbuf[ (x+512) + ((767-y)<<10) ] = color;
}*/




void line(int x1,int y1,int x2,int y2)
{
	int x,y,temp;
	DWORD* screenbuf=(DWORD*)whereisscreen();
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
			screenbuf[ (y<<10) + x1 ] = 0xffffffff;
		}
	}

	else
	{
		double k=(double)(y1-y2) / (double)(x1-x2);
		if(x1<=x2){x=x1;temp=x2;}
		if(x1>x2){x=x2;temp=x1;}

		for(;x<temp;x++)
		{
			y=y1+ (double)(x-x1)*k;
			if(y<300&&y> -300) screenbuf[ (y<<10) + x ] = 0xffffffff;
		}
	}
}




void rectangle(int x1,int y1,int x2,int y2)
{
	int x,y;
	int startx,endx,starty,endy;
	DWORD* screenbuf=(DWORD*)whereisscreen();




	if(x1<=x2){startx=x1;endx=x2;}
	else{startx=x2;endx=x1;}
	if(y1<=y2){starty=y1;endy=y2;}
	else{starty=y2;endy=y1;}




	for(y=starty;y<endy;y++)
	{
		for(x=startx;x<endx;x++)
		{
			screenbuf[ (y<<10) + x ] = 0x53840273;
		}
	}
}